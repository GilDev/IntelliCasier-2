#include <Arduino.h>
#include <avr/pgmspace.h>
#include "about.h"
#include "homework.h"
#include "informations.h"
#include "login.h"
#include "notifications.h"
#include "options.h"
#include "schedule.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"
#include "../screensaver.h"
#include <avr/pgmspace.h>

/* ----- Student Menu ----- */

// Notifications handling is really ugly
static const char menuStudentItemNotificationsNone[] PROGMEM = "Notifications";
static const char menuStudentItemNotifications[] PROGMEM = "! Notifications !";
static const char menuStudentItem2[] PROGMEM = "Emploi du temps";
static const char menuStudentItem3[] PROGMEM = "Informations";
static const char menuStudentItem4[] PROGMEM = "Devoirs";
static const char menuStudentItem5[] PROGMEM = "A propos";
static const char menuStudentItem6[] PROGMEM = "Deconnexion";
static const char* const menuStudentNoNotifications[] PROGMEM = {
	menuStudentItemNotificationsNone,
	menuStudentItem2,
	menuStudentItem3,
	menuStudentItem4,
	menuStudentItem5,
	menuStudentItem6
};
static const char* const menuStudentWithNotifications[] PROGMEM = {
	menuStudentItemNotifications,
	menuStudentItem2,
	menuStudentItem3,
	menuStudentItem4,
	menuStudentItem5,
	menuStudentItem6
};
static void (*menuStudentCallbacks[])(void) = {
	launchNotifications,
	launchSchedule,
	launchInformations,
	launchHomework,
	launchAbout,
	launchLogin
};

#define NUMBER_MENU_STUDENT_ITEM (sizeof menuStudentWithNotifications / sizeof *menuStudentWithNotifications)


/* ----- Operator Menu ----- */

static const char menuOperatorItem1[] PROGMEM = "Reglages";
static const char menuOperatorItem2[] PROGMEM = "A propos";
static const char menuOperatorItem3[] PROGMEM = "Deconnexion";
static const char* const menuOperator[] PROGMEM = {
	menuOperatorItem1,
	menuOperatorItem2,
	menuOperatorItem3
};
static void (*menuOperatorCallbacks[])(void) = {
	launchOptions,
	launchAbout,
	launchLogin
};

#define NUMBER_MENU_OPERATOR_ITEM (sizeof menuOperator / sizeof *menuOperator)


void launchMenu()
{
	lcd.clear();
	drawTitle(PSTR("MENU PRINCIPAL"));
	if (connectedUser == STUDENT) {
		displayMenu(NUMBER_MENU_STUDENT_ITEM, (numberOfNotifications > 0) ? menuStudentWithNotifications : menuStudentNoNotifications, menuStudentCallbacks);
	} else {
		displayMenu(NUMBER_MENU_OPERATOR_ITEM, menuOperator, menuOperatorCallbacks);
	}
}
