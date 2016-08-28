#include "Arduino.h"
#include "../options.h"
#include "../../alarm.h"
#include "../../events.h"
#include "../../lcd.h"

static void printSensivity(void)
{
	clearLcdRow(2, 16, 19);
	printLcd(16, 2, sensivity);
}

static void changeValue(byte more)
{
	if (more)
		sensivity += 10;
	else
		sensivity -= 10;

	printSensivity();
}

static void quit(byte data)
{
	launchOptions();
}

void launchAlarmSensivity(void)
{
	lcd.clear();
	drawTitle(PSTR("SENSI. ALARME"));

	printLcdFromFlash(2, 2, PSTR("Sensibilit"));
	lcd.write(LCD_E_ACCENT_AIGU);
	lcd.print(" :");
	printSensivity();

	setSingleClickHandler(UP, changeValue, true);
	setSingleClickHandler(DOWN, changeValue, false);
	setSingleClickHandler(LEFT, changeValue, false);
	setSingleClickHandler(RIGHT, changeValue, true);
	setSingleClickHandler(ENTER, quit, 0);
}
