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
	printLcd(1, 1, PSTR("IntelliCasier v1.0"));
	printLcd(1, 2, PSTR("R"));
	lcd.write(LCD_E_ACCENT_AIGU);
	lcd.print(PSTR("alise par GilDev"));
	printLcd(2, 3, PSTR("http://gildev.tk"));

	setSingleClickHandler(UP, quit, 0);
	setSingleClickHandler(DOWN, quit, 0);
	setSingleClickHandler(LEFT, quit, 0);
	setSingleClickHandler(RIGHT, quit, 0);
	setSingleClickHandler(ENTER, quit, 0);
}
