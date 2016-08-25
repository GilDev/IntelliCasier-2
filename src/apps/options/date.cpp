#include <Arduino.h>
#include <Time.h>
#include "date.h"
#include "../options.h"
#include "../../lcd.h"
#include "../../events.h"

byte selectedDay, selectedMonth, selectedHour, selectedMinute, selectedField;
unsigned short selectedYear;

enum field {DAY = 0, MONTH, YEAR, HOUR, MINUTE};

static void printEditableDate(void)
{
	clearLcdRow(2, 4, 14);

	printLcd(4, 2, selectedDay);
	printLcd(6, 2, "/");
	lcd.print(selectedMonth);
	printLcd(9, 2, "/");
	lcd.print(selectedYear);

	clearLcdRow(3, 4, 14);

	printLcd(6, 3, selectedHour);
	printLcd(8, 3, "h");
	lcd.print(selectedMinute);

	lcd.setCursor((selectedField > 2) ? 6 + 3 * (selectedField - 3) : 4 + selectedField * 3, (selectedField > 2) ? 3 : 2);
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
				break;
			case HOUR:
				selectedHour++;
				break;
			case MINUTE:
				selectedMinute++;
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
				break;
			case HOUR:
				selectedHour--;
				break;
			case MINUTE:
				selectedMinute--;
		}
	}

	printEditableDate();
}

static void changeField(byte next)
{
	if (next) {
		if (selectedField == 4)
			selectedField = 0;
		else
			selectedField++;
	} else {
		if (selectedField == 0)
			selectedField = 4;
		else
			selectedField--;
	}

	lcd.setCursor((selectedField > 2) ? 6 + 3 * (selectedField - 3) : 4 + selectedField * 3, (selectedField > 2) ? 3 : 2);
}

static void quit(byte data)
{
	lcd.noBlink();
	setTime(selectedHour, selectedMinute, 0, selectedDay, selectedMonth, selectedYear);
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
