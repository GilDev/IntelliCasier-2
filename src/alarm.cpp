#include <Arduino.h>
#include "alarm.h"
#include "config.h"
#include "events.h"

unsigned short lightMax;
bool ringing = false;
bool alarmActivated;

static void ring(byte numberOfTime)
{
	ringing = true;
	if (numberOfTime % 2) {
		pinMode(BUZZER_PIN, OUTPUT);
		analogWrite(BUZZER_PIN, 128);
	} else {
		pinMode(BUZZER_PIN, INPUT);
	}

	if (numberOfTime == 0) {
		ringing = false;
		alarmOn();
	} else {
		registerTimerEvent(200, ring, --numberOfTime);
	}
}

static void checkAlarm(byte data)
{
	unsigned short light = analogRead(PHOTORESISTOR_PIN);
	if (light > lightMax) {
		lightMax = light;
	} else if (light <= lightMax - ALARM_SENSIVITY && !ringing && alarmActivated) {
		ring(10);
	}

	registerTimerEvent(2000, checkAlarm, 0);
}

void alarmOn(void)
{
	lightMax = analogRead(PHOTORESISTOR_PIN);
	alarmActivated = true;
}

void alarmOff(void)
{
	alarmActivated = false;
}

void alarmScreensaver(void)
{
	alarmOn();
	registerTimerEvent(2000, checkAlarm, 0);
}

void alarmInit(void)
{
	#ifdef ALARM
	pinMode(BUZZER_PIN, INPUT);
	pinMode(PHOTORESISTOR_PIN, INPUT);
	alarmOn();
	registerTimerEvent(1000, checkAlarm, 0);
	#endif
}
