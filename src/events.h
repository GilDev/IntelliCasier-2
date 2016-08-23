#ifndef EVENTS_H
#define EVENTS_H

typedef enum {
  UP,
  RIGHT,
  DOWN,
  LEFT,
  ENTER
} ButtonId;

typedef signed char TimerId;

/**
 * Initialize event handler
 */
void eventsInit(void);

/**
 * Check and trigger events
 */
void eventsUpdateLoop(void);

/**
 * Call a function when pressing a button
 * @param button   Button to check
 * @param callback Function to call when the event triggers
 * @param data     Data to pass to the callback
 */
void setSingleClickHandler(ButtonId button, void (*callback)(byte data), byte data);

/**
 * Call a function after holding a button for a certain amount of time
 * @param button   Button to check
 * @param delay    Amount of time between pressing the button and trigerring the event in milliseconds
 * @param callback Function to call when the event triggers
 * @param data     Data to pass to the callback
 */
void setHoldClickHandler(ButtonId button, unsigned short delay, void (*callback)(byte data), byte data);

/**
 * Call a function every some amount of time while holding a button
 * @param button   Button to check
 * @param delay    Amount of time between event triggers in milliseconds
 * @param callback Function to call when the event triggers
 * @param data     Data to pass to the callback
 */
void setRepeatClickHandler(ButtonId button, unsigned short delay, void (*callback)(byte data), byte data);

/**
 * Call a function after a certain amount of time
 * @param delay    Time to wait before trigerring the event in milliseconds
 * @param callback Function to call when the event triggers
 * @param data     Data to pass to the callback
 */
TimerId registerTimerEvent(unsigned short delay, void (*callback)(byte data), byte data);

/**
 * Cancel a Timer event
 * @param id Timer to cancel
 */
void cancelTimerEvent(TimerId *id);

/**
 * Cancel all Timer events
 */
void cancelAllTimerEvents(void);

#endif
