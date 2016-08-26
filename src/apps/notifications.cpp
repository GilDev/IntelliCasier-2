#include <Arduino.h>
#include "menu.h"
#include "../events.h"
#include "../lcd.h"

byte numberOfNotifications = 0;
byte selectedNotification;

static void quit(byte data) {
	numberOfNotifications = 0;
	launchMenu();
}

void newNotification(void)
{
	if (numberOfNotifications < 3)
		numberOfNotifications++;
	printLcd(18, 3, numberOfNotifications);
}

static void printNotification(void) {
	clearLcdRow(1, 0, 20);
	clearLcdRow(2, 0, 20);
	clearLcdRow(3, 0, 20);

	switch (selectedNotification) {
		case 0:
			printLcd(16, 0, "1");
			printLcdFromFlash(0, 1, PSTR("Cours de maths de"));
			printLcdFromFlash(0, 2, PSTR("jeudi 17h decale a"));
			printLcdFromFlash(0, 3, PSTR("18h."));
			break;

		case 1:
			printLcd(16, 0, "2");
			printLcdFromFlash(0, 1, PSTR("Le cours d'histoire"));
			printLcdFromFlash(0, 2, PSTR("de mardi sera assure"));
			printLcdFromFlash(0, 3, PSTR("par M. Latouche."));
			break;

		case 2:
			printLcd(16, 0, "3");
			printLcdFromFlash(0, 1, PSTR("Vous etes convoque"));
			printLcdFromFlash(0, 2, PSTR("au secretariat lundi"));
			printLcdFromFlash(0, 3, PSTR("a 13h."));
	}
}

static void changeNotification(byte next)
{
	if (next) {
		if (selectedNotification == numberOfNotifications - 1) {
			selectedNotification = 0;
		} else {
			selectedNotification++;
		}
	} else {
		if (selectedNotification == 0)
			selectedNotification = numberOfNotifications - 1;
		else
			selectedNotification--;
	}

	printNotification();
}

static void none(byte data) {}

void launchNotifications(void)
{
	lcd.clear();
	drawTitle(PSTR("NOTIFICATION  "));

	selectedNotification = 0;

	if (numberOfNotifications == 0) {
		printLcdFromFlash(6, 1, PSTR("Pas de"));
		printLcdFromFlash(4, 2, PSTR("notifications"));
		setSingleClickHandler(UP, none, 0);
		setSingleClickHandler(DOWN, none, 0);
		setSingleClickHandler(LEFT, none, 0);
		setSingleClickHandler(RIGHT, none, 0);
	} else {
		printNotification();

		setSingleClickHandler(UP, changeNotification, false);
		setSingleClickHandler(DOWN, changeNotification, true);
		setSingleClickHandler(LEFT, changeNotification, false);
		setSingleClickHandler(RIGHT, changeNotification, true);
	}

	setSingleClickHandler(ENTER, quit, 0);
}
