#include <Arduino.h>
#include "config.h"
#include "events.h"
#include "screensaver.h"
#include "apps/menu.h"

#define NUMBER_OF_TIMER_EVENTS 10

// BUTTONS
enum { // Click types
	SINGLE,
	HOLD,
	REPEAT
};

static struct {
	byte type:2;                  // Click type
	byte oldState:1;              // Used with SINGLE and HOLD click types to know when an input change occurs
	byte clicked:1;               // True when HOLD trigger has occured
	unsigned short delay;         // Used with HOLD and REPEAT click types
	unsigned long lastActivation; // Used with SINGLE click type to avoid bouncing inputs
	void (*callback)(byte data); // Callback to call when input triggers
	byte data;           // Data to give to the callback. Could be changed to a pointer to void?
} buttons[5];

// TIMER EVENTS
static struct {
	bool activated;
	unsigned long activationTime;
	void (*callback)(byte data);
	byte data;
} timers[NUMBER_OF_TIMER_EVENTS];

unsigned long screensaverTimer = millis();

void eventsInit(void)
{
	byte i;
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++)
		timers[i].activated = false;
}

void eventsUpdateLoop(void)
{
	unsigned long actualTime;

	// INPUTS
	byte i;
	for (i = 0; i < 5; i++) {
		actualTime = millis();

		bool state = digitalRead(buttonsPins[i]);

		// SCREENSAVER
		if (state == !BUTTON_OPEN) // When a button is pressed, reset screensaver launch delay
			screensaverTimer = actualTime;
		else if (!displayingScreensaver && actualTime - screensaverTimer > screensaverDelay) {
			showScreensaver();
		}

		// Single click
		if (buttons[i].type == SINGLE) {
			if (state != buttons[i].oldState) {
				if (actualTime - buttons[i].lastActivation > DEBOUNCE_TIME) {
					buttons[i].lastActivation = actualTime;
					if (state == !BUTTON_OPEN) {
						#ifdef DEBUG
							Serial.print(F("Single click: "));
							Serial.println(i);
						#endif
						(*buttons[i].callback)(buttons[i].data);
					}
				}
			}
			buttons[i].oldState = state;
		// Hold click
		} else if (buttons[i].type == HOLD) {
			if (state == !BUTTON_OPEN) {
				if (state != buttons[i].oldState) {
					buttons[i].lastActivation = actualTime;
				} else {
					if (!buttons[i].clicked && actualTime - buttons[i].lastActivation > buttons[i].delay) {
						#ifdef DEBUG
							Serial.print(F("Hold click: "));
							Serial.println(i);
						#endif
						buttons[i].clicked = true;
						(*buttons[i].callback)(buttons[i].data);
					}
				}
			} else {
				buttons[i].clicked = false;
			}
			buttons[i].oldState = state;
		// Repeat click
		} else {
			if (state == !BUTTON_OPEN) {
				if (actualTime - buttons[i].lastActivation > buttons[i].delay) {
					#ifdef DEBUG
						Serial.print(F("Repeat click: "));
						Serial.println(i);
					#endif
					buttons[i].lastActivation = actualTime;
					(*buttons[i].callback)(buttons[i].data);
				}
			}
		}
	}

	// TIMERS
	actualTime = millis();
	// This loop could be optimized with a queue and a counter
	// of active timers instead of testing all timers
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++) { 
		if (timers[i].activated && actualTime >= timers[i].activationTime) {
			timers[i].activated = false;

			(*timers[i].callback)(timers[i].data);

			#ifdef DEBUG
				Serial.print(F("Timer: "));
				Serial.println(i);
			#endif
		}
	}
}

// BUTTONS
void setSingleClickHandler(ButtonId button, void (*callback)(byte data), byte data)
{
	buttons[button].type = SINGLE;
	buttons[button].oldState = BUTTON_OPEN;
	buttons[button].lastActivation = 0;
	buttons[button].callback = callback;
	buttons[button].data = data;
}

void setHoldClickHandler(ButtonId button, unsigned short delay, void (*callback)(byte data), byte data)
{
	buttons[button].type = HOLD;
	buttons[button].oldState = BUTTON_OPEN;
	buttons[button].delay = delay;
	buttons[button].clicked = false;
	buttons[button].callback = callback;
	buttons[button].data = data;
}

void setRepeatClickHandler(ButtonId button, unsigned short delay, void (*callback)(byte data), byte data)
{
	buttons[button].type = REPEAT;
	buttons[button].delay = delay;
	buttons[button].lastActivation = 0;
	buttons[button].callback = callback;
	buttons[button].data = data;
}

// TIMERS
TimerId registerTimerEvent(unsigned short delay, void (*callback)(byte data), byte data)
{
	byte i;
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++)
		if (!timers[i].activated) {
			timers[i].activated = true;
			timers[i].activationTime = millis() + delay;
			timers[i].callback = callback;
			timers[i].data = data;
			return i;
		}
}

void cancelTimerEvent(TimerId *id)
{
	if (*id >= 0) {
		timers[*id].activated = false;
		*id = -1;
	}
}

void cancelAllTimerEvents(void)
{
	byte i;
	for (i = 0; i < NUMBER_OF_TIMER_EVENTS; i++)
		timers[i].activated = false;
}
