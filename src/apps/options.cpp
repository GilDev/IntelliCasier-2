/*#include <Arduino.h>
#include "options.h"
#include "menu.h"
#include "../config.h"
#include "../lcd.h"
#include "../events.h"

#define NUMBER_OF_OPTIONS (sizeof options / sizeof *options)

static byte selectedOption;

unsigned short options[] = { // Default values
	MATRIX_BRIGHTNESS,
	MATRIX_SCREENSAVER_BRIGHTNESS,
	MATRIX_SCROLL_DELAY,
	SNAKE_MOVE_DELAY,
	FLAPPY_BIRD_FALL_DELAY,
	FLAPPY_BIRD_START_DELAY,
	FLAPPY_BIRD_MIN_DELAY,
	FLAPPY_BIRD_SPEED_INCREASES_DELAY,
	PONG_PADDLE_DELAY,
	PONG_START_DELAY,
	PONG_MIN_DELAY,
	PONG_SPEED_INCREASES_DELAY
};

static void updateValue(void)
{
	switch (selectedOption) {
		case MATRIX_BRIGHTNESS_O:
			matrix.setIntensity(0, options[MATRIX_BRIGHTNESS_O]);
			break;
	}

	clearLcdLine(1);
	printLcd(7, 1, options[selectedOption]);
}

static void print(void)
{
	stopLcdScroll(0);
	clearLcdLine(0);

	strcpy_P(buffer, (char *) pgm_read_word(&(strings[OPTION_1 + selectedOption])));

	if (stringsSizes[OPTION_1 + selectedOption] > 16)
		newLcdScroll(buffer, 0, 200);
	else
		printLcd(8 - stringsSizes[OPTION_1 + selectedOption], 0, buffer);

	updateValue();
}

static void changeValue(byte plus)
{
	char multiplier = (plus == 0) ? -1 : 1;

	switch (selectedOption) { // Different increment depending on value
		case MATRIX_BRIGHTNESS_O:
		case MATRIX_SCREENSAVER_BRIGHTNESS_O:
			options[selectedOption] += 1 * multiplier;
			break;

		case PONG_SPEED_INCREASES_DELAY_O:
		case FLAPPY_BIRD_SPEED_INCREASES_DELAY_O:
			options[selectedOption] += 100 * multiplier;
			break;

		default:
			options[selectedOption] += 10 * multiplier;
	}
	
	updateValue();
}

static void next(byte data)
{
	if (selectedOption == NUMBER_OF_OPTIONS - 1)
		selectedOption = 0;
	else
		selectedOption++;

	print();
}

static void previous(byte data)
{
	if (selectedOption == 0)
		selectedOption = NUMBER_OF_OPTIONS - 1;
	else
		selectedOption--;

	print();
}

static void menu(byte data)
{
	stopLcdScroll(0);
	stopMatrixScroll();
	showMenu();
}

void showOptions(void)
{
	clearDisplays();

	setSingleClickHandler(PLAYER1_LEFT, previous, 0);
	setSingleClickHandler(PLAYER1_RIGHT, next, 0);
	setRepeatClickHandler(PLAYER2_LEFT, 100, changeValue, 1);
	setRepeatClickHandler(PLAYER2_RIGHT, 100, changeValue, 0);
	setSingleClickHandler(MENU, menu, 0);

	selectedOption = 0;

	newMatrixScroll("SECRET");

	print();
}
*/