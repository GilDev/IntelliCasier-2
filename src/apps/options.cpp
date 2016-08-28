#include <Arduino.h>
#include "options.h"
#include "options/date.h"
#include "options/alarmSensivity.h"
#include "menu.h"
#include "notifications.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"

static const char menuItem1[] PROGMEM = "Retour";
static const char menuItem2[] PROGMEM = "Date & Heure";
static const char menuItem3[] PROGMEM = "Notification";
static const char menuItem4[] PROGMEM = "Sensi. alarme";
static const char* const menu[] PROGMEM = {
	menuItem1,
	menuItem2,
	menuItem3,
	menuItem4
};
static void (*menuCallbacks[])(void) = {
	launchMenu,
	launchDate,
	newNotification,
	launchAlarmSensivity
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
}
