#include <Arduino.h>
#include "menu.h"
#include "../events.h"
#include "../lcd.h"


static void quit(byte data)
{
	launchMenu();
}

void launchAbout(void)
{
	lcd.clear();
	drawTitle(PSTR("A PROPOS"));
	printLcdFromFlash(1, 1, PSTR("IntelliCasier v1.0"));
	printLcdFromFlash(1, 2, PSTR("R"));
	lcd.write(LCD_E_ACCENT_AIGU);
	printLcdFromFlash(3, 2, PSTR("alise par GilDev"));
	printLcdFromFlash(2, 3, PSTR("http://gildev.tk"));

	setSingleClickHandler(UP, quit, 0);
	setSingleClickHandler(DOWN, quit, 0);
	setSingleClickHandler(LEFT, quit, 0);
	setSingleClickHandler(RIGHT, quit, 0);
	setSingleClickHandler(ENTER, quit, 0);
}
