#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* ----- General settings ----- */
//#define DEBUG            // Activate serial messages & internal LED blinking every 1000 loop cycle
#define SERIAL_BAUD 9600 // Serial communication speed

#define LANG 1 // 0 = English, 1 = French

#define USE_INTERNAL_PULLUPS true // Use the Arduino's builtin pullups on inputs
#define BUTTON_OPEN HIGH          // Default value when button is not pressed
#define DEBOUNCE_TIME 10          // Delay where no input can change after changing state in ms

#define DELAY_BEFORE_SCREENSAVER_WHILE_UNLOGGED 15 // In s
#define DELAY_BEFORE_SCREENSAVER_WHILE_LOGGED   60 // Disconnect user. In s

#define BUZZER_PIN 5
#define PHOTORESISTOR_PIN A7

//#define ALARM // Define to activate alarm
#define ALARM_SENSIVITY 100

static const unsigned char buttonsPins[] = {
	A3, // UP
	A2, // RIGHT
	13, // DOWN
	A0, // LEFT
	A1  // ENTER
};


/* ----- App settings ----- */


#endif