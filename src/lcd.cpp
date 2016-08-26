#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "lcd.h"
#include "events.h"
#include "apps/options.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

static byte blackSquare[] = {
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
};

static byte upArrow[] = {
	B00100,
	B01110,
	B11111,
	B00100,
	B00100,
	B00100,
	B00100,
	B00100
};

static byte rightArrow[] = {
	B00000,
	B00100,
	B00110,
	B11111,
	B00110,
	B00100,
	B00000,
	B00000
};

static byte downArrow[] = {
	B00100,
	B00100,
	B00100,
	B00100,
	B00100,
	B11111,
	B01110,
	B00100
};

static byte eAccentAigu[] = {
	B00010,
	B00100,
	B01110,
	B10001,
	B11111,
	B10000,
	B01110,
	B00000
};

static char *dayNames[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
static char *monthNames[] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"};

void drawDate(byte row, time_t t)
{
	clearLcdRow(row, 0, 20);
	lcd.setCursor(0, row);
	lcd.print(dayNames[weekday(t) - 2]);
	lcd.write(' ');
	lcd.print(day(t));
	lcd.write(' ');
	lcd.print(monthNames[month(t) - 1]);
}

/* ----- Start of the area you've never seen ----- */
void drawDateForSchedule(byte row, byte weekdayGiven) // That's ugly, I know. It's temporary, I hope.
{
	clearLcdRow(row, 0, 20);
	lcd.setCursor(0, row);
	lcd.print(dayNames[weekdayGiven]);
	lcd.write(' ');
	lcd.print(day() + weekdayGiven - (weekday() - 2));
	lcd.write(' ');
	lcd.print(monthNames[month() - 1]);
}
/* ----- End of the area you've never seen ----- */

void printLcdFromFlash(byte x, byte y, const char *str) {
	char buffer[21];
	strcpy_P(buffer, (PGM_P) str);
	lcd.setCursor(x, y);
	lcd.print(buffer);
}

void drawTitle(const char *title)
{
	char buffer[21];
	strcpy_P(buffer, (PGM_P) title);

	byte titleLength = strlen(buffer);

	lcd.home();

	byte i;
	for (i = 0; i < (20 - (titleLength + 2)) / 2; i++) {
		lcd.write(LCD_BLACK_SQUARE);
	}

	lcd.write(' ');
	lcd.print(buffer);
	lcd.write(' ');
	
	for (i += titleLength + 2; i < 20; i++) {
		lcd.write(LCD_BLACK_SQUARE);
	}
}

byte menuSelectedItem;
byte menuNumberOfItems;
char **menuItemNames;
void (**menuItemCallbacks)(void);

static void displayMenuItems(void)
{
	char buffer[20];

	byte i;
	for (i = 1; i < 4; i++)
		clearLcdRow(i, 2, 19);

	// Items
	if (menuSelectedItem <= 1)
		i = 0;
	else if (menuSelectedItem >= menuNumberOfItems - 1)
		i = menuNumberOfItems - 3;
	else
		i = menuSelectedItem - 1;

	byte row;
	for (row = 1; row < ((menuNumberOfItems > 3) ? 4 : menuNumberOfItems + 1); row++) {
		strcpy_P(buffer, (char *) pgm_read_word(&(menuItemNames[i++])));
		printLcd(2, row, buffer);
	}

	// Scrolling arrows
	if (menuNumberOfItems > 3) {
		lcd.setCursor(19, 1);
		if (menuSelectedItem > 1)
			lcd.write(LCD_UP_ARROW);
		else
			lcd.write(' ');

		lcd.setCursor(19, 3);
		if (menuSelectedItem < menuNumberOfItems - 2)
			lcd.write(LCD_DOWN_ARROW);
		else
			lcd.write(' ');
	}
}

static void changeSelection(byte down)
{
	if (down) {
		if (menuSelectedItem < menuNumberOfItems - 1) {
			menuSelectedItem++;
		}
	} else {
		if (menuSelectedItem > 0) {
			menuSelectedItem--;
		}
	}

	// Selector
	if (menuSelectedItem == 0) {
		lcd.setCursor(0, 2);
		lcd.write(' ');
		lcd.setCursor(0, 1);
		lcd.write(LCD_RIGHT_ARROW);
	} else if (menuSelectedItem == menuNumberOfItems - 1 && menuNumberOfItems != 2) {
		lcd.setCursor(0, 2);
		lcd.write(' ');
		lcd.setCursor(0, 3);
		lcd.write(LCD_RIGHT_ARROW);
	} else {
		lcd.setCursor(0, 1);
		lcd.write(' ');
		lcd.setCursor(0, 3);
		lcd.write(' ');
		lcd.setCursor(0, 2);
		lcd.write(LCD_RIGHT_ARROW);
	}

	if (down && menuSelectedItem > 1 &&
		menuSelectedItem != menuNumberOfItems - 1 ||
		!down && menuSelectedItem != 0 &&
		menuSelectedItem < menuNumberOfItems - 2)
		displayMenuItems();
}

static void launchApp(byte data)
{
	(*menuItemCallbacks[menuSelectedItem])();
}

void displayMenu(byte numberOfItems, const char* const itemNames[], void (*itemCallbacks[])(void))
{
	menuItemNames = (char **) itemNames;
	menuNumberOfItems = numberOfItems;
	menuItemCallbacks = itemCallbacks;

	menuSelectedItem = 0;

	displayMenuItems();

	lcd.setCursor(0, 1);
	lcd.write(LCD_RIGHT_ARROW);

	setRepeatClickHandler(UP, 300, changeSelection, false);
	setRepeatClickHandler(DOWN, 300, changeSelection, true);
	setSingleClickHandler(ENTER, launchApp, 0);
	setSingleClickHandler(RIGHT, launchApp, 0);
}

static TimerId lcdTimers[2] = {-1, -1};
static char *scrollingLcdTexts[2];
static unsigned short speeds[2];
static signed char startPos[2], stopPos[2];

static void lcdScroll(unsigned char line)
{
	clearLcdRow(line, 0, 20);

	startPos[line]--;
	stopPos[line]--;

	if (stopPos[line] == 0) {
		startPos[line] = 20;
		stopPos[line] = 20 + strlen(scrollingLcdTexts[line]);
	}

	byte start = (startPos[line] < 0) ? 0  : startPos[line];
	byte end = (stopPos[line] > 20) ? 20 : stopPos[line];
	lcd.setCursor(start , line);

	byte i;
	for (i = start; i < end; i++)
		lcd.write(scrollingLcdTexts[line][((startPos[line] >= 0) ? 0 : -startPos[line]) + i - start]);

	lcdTimers[line] = registerTimerEvent(speeds[line], lcdScroll, line);
}

void newLcdScroll(const char *text, byte line, unsigned short speed)
{
	stopLcdScroll(line);

	scrollingLcdTexts[line] = (char *) text;
	startPos[line] = 20;
	stopPos[line]  = 20 + strlen(scrollingLcdTexts[line]);

	speeds[line] = speed;

	lcdTimers[line] = registerTimerEvent(speed, lcdScroll, line);
}

void stopLcdScroll(byte line)
{
	clearLcdRow(line, 0, 20);
	cancelTimerEvent(&lcdTimers[line]);
}

void clearLcdRow(byte line, byte start, byte end)
{
	lcd.setCursor(start, line);
	byte i;
	for (i = start; i < end; i++)
		lcd.write(' ');
}

void lcdInit(void)
{
	Wire.begin();
	lcd.begin(20, 4);
	lcd.clear();
	lcd.display();
	lcd.backlight();
	lcd.home();

	lcd.createChar(LCD_BLACK_SQUARE, blackSquare);
	lcd.createChar(LCD_UP_ARROW, upArrow);
	lcd.createChar(LCD_RIGHT_ARROW, rightArrow);
	lcd.createChar(LCD_DOWN_ARROW, downArrow);
	lcd.createChar(LCD_E_ACCENT_AIGU, eAccentAigu);
}
