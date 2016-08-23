#include <Arduino.h>
#include "alarm.h"
#include "config.h"
#include "lcd.h"
#include "events.h"
#include "screensaver.h"
#include "apps/menu.h"
#include "apps/options.h"

bool displayingScreensaver = false;
unsigned short screensaverDelay;

void exitScreensaver(byte data)
{

	displayingScreensaver = false;
	launchMenu();
}

void showScreensaver(void)
{
	#ifdef DEBUG
		Serial.print(F("Displaying screensaver\n"));
	#endif

	displayingScreensaver = true;
}
