#include <Arduino.h>
#include "menu.h"
#include "../alarm.h"
#include "../config.h"
#include "../events.h"
#include "../lcd.h"
#include "../screensaver.h"

byte connectedUser;
static byte code[4];
static byte currentColumn;
static TimerId eraseWrongPasswordTimer = -1;

static void changeNumber(byte lower)
{
	if (lower) {
		if (code[currentColumn] == 0)
			code[currentColumn] = 9;
		else
			code[currentColumn]--;
	} else {
		if (code[currentColumn] == 9)
			code[currentColumn] = 0;
		else
			code[currentColumn]++;
	}

	lcd.write(code[currentColumn] + 48);
	lcd.setCursor(currentColumn + 7, 2);
}

static void changeColumn(byte right)
{
	if (right) {
		if (currentColumn < 3) {
			currentColumn++;
			lcd.setCursor(currentColumn + 7, 2);
		}
	} else {
		if (currentColumn > 0) {
			currentColumn--;
			lcd.setCursor(currentColumn + 7, 2);
		}
	}
}

static void writeBlankCode(void)
{
	printLcd(7, 2, "____");
	lcd.setCursor(7, 2);

	byte i;
	for (i = 0; i < 4; i++)
		code[i] = 0;
	currentColumn = 0;
}

static void eraseWrongPassword(byte data)
{
	clearLcdRow(3, 0, 20);
	lcd.setCursor(7 + currentColumn, 2);
}

static void login(byte data)
{
	unsigned short codeInt = 0;
	unsigned short multiplier;
	byte i;
	for (i = 3, multiplier = 1; i < 4; i--, multiplier *=
		10)
		codeInt += code[i] * multiplier;

	cancelTimerEvent(&eraseWrongPasswordTimer);
	if (codeInt == STUDENT_CODE) {
		connectedUser = STUDENT;
		screensaverDelay = DELAY_BEFORE_SCREENSAVER_WHILE_LOGGED * 1000;
		lcd.noBlink();
		alarmOff();
		launchMenu();
	} else if (codeInt == OPERATOR_CODE) {
		connectedUser = OPERATOR;
		screensaverDelay = DELAY_BEFORE_SCREENSAVER_WHILE_LOGGED * 1000;
		lcd.noBlink();
		alarmOff();
		launchMenu();
	} else {
		printLcdFromFlash(0, 3, PSTR("Mauvais mot de passe"));
		writeBlankCode();
		eraseWrongPasswordTimer = registerTimerEvent(2000, eraseWrongPassword, 0);
	}
}

void launchLogin(void)
{
	screensaverDelay = DELAY_BEFORE_SCREENSAVER_WHILE_UNLOGGED * 1000;

	lcd.clear();
	//static const char connexion[] PROGMEM = "CONNEXION";
	drawTitle(PSTR("CONNEXION"));
	printLcdFromFlash(3, 1, PSTR("Mot de passe :"));

	writeBlankCode();
	lcd.blink();

	alarmOn();

	setSingleClickHandler(UP, changeNumber, false);
	setSingleClickHandler(DOWN, changeNumber, true);
	setSingleClickHandler(LEFT, changeColumn, false);
	setSingleClickHandler(RIGHT, changeColumn, true);
	setSingleClickHandler(ENTER, login, 0);
}
