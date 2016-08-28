#include <Arduino.h>
#include <TimeLib.h>
#include "alarm.h"
#include "config.h"
#include "events.h"
#include "lcd.h"
#include "apps/login.h"

void setup() {
	#ifdef DEBUG or SERIAL
		Serial.begin(SERIAL_BAUD);
	#endif

	Serial.begin(9600);
	Serial.println(A1);

	// Inputs
	byte i;
	for (i = 0; i < 5; i++) {
		if (buttonsPins[i] == 13) {
			pinMode(buttonsPins[i], INPUT);
			digitalWrite(buttonsPins[i], HIGH);
		} else {
			pinMode(buttonsPins[i], INPUT_PULLUP);
		}
	}

	lcdInit();
	eventsInit();
	alarmInit();
	launchLogin();
	setTime(10, 0, 0, 6, 9, 2016);

	#ifdef DEBUG
		Serial.print(F("Ready"));
	#endif
	digitalWrite(13, LOW);
}

void loop() {
	eventsUpdateLoop();

	#ifdef DEBUG
		// Internal LED change state every 1000 loop cycle
		static bool ledState = LOW;
		static unsigned short cycleCounter = 0;

		if (++cycleCounter == 1000) {
			cycleCounter = 0;
			if (ledState)
				ledState = LOW;
			else
				ledState = HIGH;
		}

		digitalWrite(13, ledState);
	#endif
}
