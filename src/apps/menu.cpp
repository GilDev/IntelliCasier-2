#include <Arduino.h>
#include <avr/pgmspace.h>
#include "about.h"
#include "login.h"
#include "options.h"
#include "schedule.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"
#include "../screensaver.h"
#include <avr/pgmspace.h>

/* ----- Student Menu ----- */

static const char menuStudentItem1[] PROGMEM = "Emploi du temps";
static const char menuStudentItem2[] PROGMEM = "Informations";
static const char menuStudentItem3[] PROGMEM = "Devoirs";
static const char menuStudentItem4[] PROGMEM = "Twitter";
static const char menuStudentItem5[] PROGMEM = "A propos";
static const char menuStudentItem6[] PROGMEM = "Deconnexion";
static const char* const menuStudent[] PROGMEM = {
	menuStudentItem1,
	menuStudentItem2,
	menuStudentItem3,
	menuStudentItem4,
	menuStudentItem5,
	menuStudentItem6
};
static void (*menuStudentCallbacks[])(void) = {
	launchSchedule,
	NULL,
	NULL,
	NULL,
	launchAbout,
	launchLogin
};

#define NUMBER_MENU_STUDENT_ITEM (sizeof menuStudent / sizeof *menuStudent)


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
		displayMenu(NUMBER_MENU_STUDENT_ITEM, menuStudent, menuStudentCallbacks);
	} else {
		displayMenu(NUMBER_MENU_OPERATOR_ITEM, menuOperator, menuOperatorCallbacks);
	}
}

static void exitMenu(void)
{
	//screensaverDelay = DELAY_BEFORE_SCREENSAVER_IN_APP * 1000;
}
