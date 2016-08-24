#include <Arduino.h>
#include <Time.h>
#include "date.h"
#include "../options.h"
#include "../../lcd.h"
#include "../../events.h"

byte selectedDay, selectedMonth, selectedField;
unsigned short selectedYear;

enum field {DAY = 0, MONTH, YEAR, HOUR, MINUTE};

static void printEditableDate(void)
{
	printLcd(0, 1, selectedDay);
	printLcd(2, 1, "/");
	lcd.print(selectedMonth);
	printLcd(5, 1, "/");
	lcd.print(selectedYear);

	lcd.setCursor(selectedField * 3, 1);
}

static void changeValue(byte up)
{
	if (up) {
		switch (selectedField) {
			case DAY:
				selectedDay++;
				break;
			case MONTH:
				selectedMonth++;
				break;
			case YEAR:
				selectedYear++;
		}
	} else {
		switch (selectedField) {
			case DAY:
				selectedDay--;
				break;
			case MONTH:
				selectedMonth--;
				break;
			case YEAR:
				selectedYear--;
		}
	}

	printEditableDate();
}

static void changeField(byte next)
{
	if (next) {
		selectedField++;
	} else {
		selectedField--;
	}

	lcd.setCursor(selectedField * 3, 1);
}

static void quit(byte data)
{
	lcd.noBlink();
	setTime(hour(), minute(), second(), selectedDay, selectedMonth, selectedYear);
	launchOptions();
}

void launchDate(void)
{
	lcd.clear();
	drawTitle(PSTR("DATE & HEURE"));

	selectedField = 0;
	selectedDay = day();
	selectedMonth = month();
	selectedYear = year();
	printEditableDate();

	lcd.blink();

	setSingleClickHandler(UP, changeValue, true);
	setSingleClickHandler(DOWN, changeValue, false);
	setSingleClickHandler(LEFT, changeField, false);
	setSingleClickHandler(RIGHT, changeField, true);
	setSingleClickHandler(ENTER, quit, true);
}
