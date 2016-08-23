#include <Arduino.h>
#include <Time.h>
#include "menu.h"
#include "options.h"
#include "../alarm.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"
#include "../screensaver.h"

static byte selectedDay, selectedLesson;

static char *dayNames[] = {"", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"};
static char *monthNames[] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};

static char *lessonNames[] = {"Histoire-Geo", "Anglais", "Physique-Chimie", "SI", "ISN", "Allemand", "Mathematiques", "Philosophie", "EPS"};
static char *roomNames[] = {"E205", "E218", "E187", "D143", "E215", "E188", "E206", "E303", "P14", "C158", "TP", "E86", "P21", "E305"};
struct hour {
	byte lesson:4;
	byte room:4;
	byte hours:2;
};
static struct hour schedule[6][10] = {
	{
		// Monday
		{0, 0, 0},
		{15, 15, 0},
		{1, 1, 0},
		{2, 2, 0},
		{15, 15, 0},
		{3, 3, 3},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Tuesday
		{3, 3, 3},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{4, 3, 1},
		{15, 15, 0},
		{5, 4, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Wednesday
		{3, 3, 0},
		{2, 5, 1},
		{15, 15, 0},
		{6, 6, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Thursday
		{0, 7, 0},
		{6, 8, 0},
		{7, 9, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{2, 10, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	},
	{
		// Friday
		{1, 1, 0},
		{5, 4, 0},
		{8, 15, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{7, 11, 0},
		{2, 12, 0},
		{0, 13, 0},
		{2, 12, 0}
	},
	{
		// Saturday
		{6, 8, 1},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0},
		{15, 15, 0}
	}
};

static void drawDay(void)
{
	clearLcdRow(1, 0, 20);
	lcd.setCursor(0, 1);
	lcd.print(dayNames[day()]);
	lcd.write(' ');
	lcd.print(day());
	lcd.write(' ');
	lcd.print(monthNames[month()]);
}

static void drawSchedule(void)
{
	clearLcdRow(2, 0, 20);
	clearLcdRow(3, 0, 20);

	lcd.setCursor(13, 2);
	lcd.print(selectedLesson + 8);
	lcd.print("h-");
	lcd.print(selectedLesson + 8 + schedule[selectedDay][selectedLesson].hours + 1);
	lcd.print("h");

	lcd.setCursor(0, 2);
	lcd.print("Salle ");
	lcd.print(roomNames[schedule[selectedDay][selectedLesson].room]);

	lcd.setCursor(0, 3);
	lcd.print(lessonNames[schedule[selectedDay][selectedLesson].lesson]);
}

static void quit(byte data)
{
	launchMenu();
}

void launchSchedule(void)
{
	lcd.clear();
	drawTitle(PSTR("EMPLOI DU TEMPS"));
	drawDay();
	drawSchedule();

	setSingleClickHandler(UP, NULL, 0);
	setSingleClickHandler(DOWN, NULL, 0);
	setSingleClickHandler(LEFT, NULL, 0);
	setSingleClickHandler(RIGHT, NULL, 0);
	setSingleClickHandler(ENTER, quit, 0);
}
