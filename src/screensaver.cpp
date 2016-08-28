#include <Arduino.h>
#include "alarm.h"
#include "config.h"
#include "lcd.h"
#include "events.h"
#include "screensaver.h"
#include "apps/login.h"
#include "apps/notifications.h"

bool displayingScreensaver = false;
unsigned short screensaverDelay;

void exitScreensaver(byte data)
{
	displayingScreensaver = false;
	stopLcdScroll(3);
	launchLogin();
}

void showScreensaver(void)
{
	#ifdef DEBUG
		Serial.print(F("Displaying screensaver\n"));
	#endif

	alarmOn();

	lcd.noBlink();
	lcd.clear();

	drawTitle(PSTR("IntelliCasier"));
	printLcdFromFlash(2, 2, PSTR("Gilles DEVILLERS"));
	switch (numberOfNotifications) {
		case 1:
			newLcdScroll("1 nouvelle notification", 3, 200);
			break;
		case 2:
			newLcdScroll("2 nouvelles notifications", 3, 200);
			break;
		case 3:
			newLcdScroll("3 nouvelles notifications", 3, 200);
	}

	setSingleClickHandler(UP, exitScreensaver, 0);
	setSingleClickHandler(DOWN, exitScreensaver, 0);
	setSingleClickHandler(LEFT, exitScreensaver, 0);
	setSingleClickHandler(RIGHT, exitScreensaver, 0);
	setSingleClickHandler(ENTER, exitScreensaver, 0);

	displayingScreensaver = true;
}
