#include <Arduino.h>
#include "menu.h"
#include "../events.h"
#include "../lcd.h"

static byte selectedHomework;

static const char homework1[] PROGMEM = "Mathematiques";
static const char homework2[] PROGMEM = "Demain: Exercices 5,";
static const char homework3[] PROGMEM = "7 et 12 page 86.";
static const char homework4[] PROGMEM = "Histoire";
static const char homework5[] PROGMEM = "Finir la redaction";
static const char homework6[] PROGMEM = "sur le Moyen-Orient";
static const char homework7[] PROGMEM = "Sciences Ingenieur";
static const char homework8[] PROGMEM = "Apres-Demain : Finir";
static const char homework9[] PROGMEM = "l'IntelliCasier";
static const char* const homeworks[] PROGMEM = {
	homework1,
	homework2,
	homework3,
	homework4,
	homework5,
	homework6,
	homework7,
	homework8,
	homework9
};

void printHomework(void)
{
	clearLcdRow(1, 0, 20);
	clearLcdRow(2, 0, 20);
	clearLcdRow(3, 0, 20);

	char buffer[21];
	// Title
	strcpy_P(buffer, (char *) pgm_read_word(&(homeworks[selectedHomework * 3])));
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
	strcpy_P(buffer, (char *) pgm_read_word(&(homeworks[selectedHomework * 3 + 1])));
	printLcd(0, 2, buffer);

	// Second line
	strcpy_P(buffer, (char *) pgm_read_word(&(homeworks[selectedHomework * 3 + 2])));
	printLcd(0, 3, buffer);
}

void changeHomework(byte next)
{
	if (next) {
		if (selectedHomework == (sizeof homeworks / sizeof *homeworks) / 3 - 1)
			selectedHomework = 0;
		else
			selectedHomework++;
	} else {
		if (selectedHomework == 0)
			selectedHomework = (sizeof homeworks / sizeof *homeworks) / 3 - 1;
		else
			selectedHomework--;
	}

	printHomework();
}

static void quit(byte data)
{
	launchMenu();
}

void launchHomework(void)
{
	lcd.clear();
	drawTitle(PSTR("DEVOIRS"));

	selectedHomework = 0;
	printHomework();

	setSingleClickHandler(UP, changeHomework, false);
	setSingleClickHandler(DOWN, changeHomework, true);
	setSingleClickHandler(LEFT, changeHomework, false);
	setSingleClickHandler(RIGHT, changeHomework, true);
	setSingleClickHandler(ENTER, quit, 0);
}
