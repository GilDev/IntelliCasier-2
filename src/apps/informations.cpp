#include <Arduino.h>
#include "menu.h"
#include "../events.h"
#include "../lcd.h"

static byte selectedInformation;

static const char info1[] PROGMEM = "Carnaval";
static const char info2[] PROGMEM = "  Le carnaval aura";
static const char info3[] PROGMEM = " lieu le 20 fevrier";
static const char info4[] PROGMEM = "Election CVL";
static const char info5[] PROGMEM = "  Venez voter dans";
static const char info6[] PROGMEM = "      le hall !";
static const char info7[] PROGMEM = "Fete de la science";
static const char info8[] PROGMEM = " Venez visiter les";
static const char info9[] PROGMEM = "stands des eleves !";
static const char* const infos[] PROGMEM = {
	info1,
	info2,
	info3,
	info4,
	info5,
	info6,
	info7,
	info8,
	info9
};

void printInformation(void)
{
	clearLcdRow(1, 0, 20);
	clearLcdRow(2, 0, 20);
	clearLcdRow(3, 0, 20);

	char buffer[21];
	// Title
	strcpy_P(buffer, (char *) pgm_read_word(&(infos[selectedInformation * 3])));
	byte length = strlen(buffer);

	lcd.setCursor(0, 1);
	byte i;
	for (i = 0; i < (20 - (length)) / 2; i++) {
		lcd.write('_');
	}

	lcd.print(buffer);
	
	for (i += length; i < 20; i++) {
		lcd.write('_');
	}

	// First line
	strcpy_P(buffer, (char *) pgm_read_word(&(infos[selectedInformation * 3 + 1])));
	printLcd(0, 2, buffer);

	// Second line
	strcpy_P(buffer, (char *) pgm_read_word(&(infos[selectedInformation * 3 + 2])));
	printLcd(0, 3, buffer);
}

void changeInformation(byte next)
{
	if (next) {
		if (selectedInformation == (sizeof infos / sizeof *infos) / 3 - 1)
			selectedInformation = 0;
		else
			selectedInformation++;
	} else {
		if (selectedInformation == 0)
			selectedInformation = (sizeof infos / sizeof *infos) / 3 - 1;
		else
			selectedInformation--;
	}

	printInformation();
}

static void quit(byte data)
{
	launchMenu();
}

void launchInformations(void)
{
	lcd.clear();
	drawTitle(PSTR("INFORMATIONS"));

	selectedInformation = 0;
	printInformation();

	setSingleClickHandler(UP, changeInformation, false);
	setSingleClickHandler(DOWN, changeInformation, true);
	setSingleClickHandler(LEFT, changeInformation, false);
	setSingleClickHandler(RIGHT, changeInformation, true);
	setSingleClickHandler(ENTER, quit, 0);
}
