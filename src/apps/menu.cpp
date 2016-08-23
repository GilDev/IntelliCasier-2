#include <Arduino.h>
#include <avr/pgmspace.h>
#include "login.h"
#include "schedule.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"
#include "../screensaver.h"
#include <avr/pgmspace.h>

static void exitMenu(void);

struct selection {
	byte text;
	byte icon[8];
	byte menuToOpen; // 0xFF = call callback() instead
	void (*callback)(void);
};

byte currentMenu;
byte currentSelection;

static const char item1[] PROGMEM = "Emploi du temps";
static const char item2[] PROGMEM = "Informations";
static const char item3[] PROGMEM = "Devoirs";
static const char item4[] PROGMEM = "Twitter";
static const char item5[] PROGMEM = "A propos";
static const char item6[] PROGMEM = "Deconnexion";
static const char* const menuItemNames[] PROGMEM = {
	item1,
	item2,
	item3,
	item4,
	item5,
	item6
};

static void (*menuItemCallbacks[])(void) = {
	launchSchedule,
	NULL,
	NULL,
	NULL,
	NULL,
	launchLogin
};

#define NUMBER_MENU_ITEM (sizeof menuItemNames / sizeof *menuItemNames) // Divide by two because pointers are 16 bits

void launchMenu(void)
{
	lcd.clear();
	drawTitle(PSTR("MENU PRINCIPAL"));
	displayMenu(NUMBER_MENU_ITEM, menuItemNames, menuItemCallbacks);
}

static void exitMenu(void)
{
	//screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_APP * 1000;
}
