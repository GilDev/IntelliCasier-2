#include <Arduino.h>
#include "options.h"
#include "options/date.h"
#include "menu.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"

static const char menuItem1[] PROGMEM = "Date & Heure";
static const char* const menu[] PROGMEM = {
	menuItem1,
};
static void (*menuCallbacks[])(void) = {
	launchDate
};

static void quit(byte data)
{
	launchMenu();
}

void launchOptions(void)
{
	lcd.clear();
	drawTitle(PSTR("REGLAGES"));

	displayMenu((sizeof menu / sizeof *menu), menu, menuCallbacks);

	setSingleClickHandler(ENTER, quit, 0);
}
