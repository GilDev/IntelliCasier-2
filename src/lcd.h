#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include <Time.h>

enum {
	LCD_BLACK_SQUARE,
	LCD_UP_ARROW,
	LCD_RIGHT_ARROW,
	LCD_DOWN_ARROW,
	LCD_E_ACCENT_AIGU
};

extern LiquidCrystal_I2C lcd;

/**
 * Initialize LCD display
 */
void lcdInit(void);

/**
 * Print date
 * @param row   Row to print on
 * @param day   Day to print
 * @param month Month to print
 */
void drawDate(byte row, time_t t);

/**
 * Just do as this wasn't here.
 * I'm too ashamed of it.
 */
void drawDateForSchedule(byte row, byte day);

/**
 * Draw title at the top of the display
 * @param title Text to be printed (PROGMEM)
 */
void drawTitle(const char *title);

/**
 * Create and display a menu
 * @param numberOfItems Number of items in itemNames
 * @param itemNames     Array with the menu's title then item's names (18 characters max)
 * @param itemCallbacks Array with the corresponding functions to call when a menu item is entered
 */
void displayMenu(byte numberOfItems, const char* const itemNames[], void (*itemCallbacks[])(void));

/**
 * Print text on the LCD display from flash memory
 * @param x   Horizontal coordinate
 * @param y   Vertical coordinate
 * @param str PROGMEM string to print
 */
void printLcdFromFlash(byte x, byte y, const char *str);

/**
 * Print text on the LCD display
 * @param x   Horizontal coordinate
 * @param y   Vertical coordinate
 * @param str String to print
 */
#define printLcd(x, y, str) {lcd.setCursor(x, y); lcd.print(str);}

 /**
  * Clear text on an LCD display's row
  * @param row   Row to erase
  * @param start X position to start erasing from
  * @param end   X position to stop erasing from
  */
void clearLcdRow(byte row, byte start, byte end);

/**
 * Scroll a text on an LCD display's row
 * @param text Text to scroll
 * @param row row to scroll the text on
 * @param speed Time between individual scrolls in milliseconds
 */
void newLcdScroll(const char *text, byte row, unsigned short speed);

/**
 * Stop scrolling text on an LCD display's row
 * @param row row to stop scrolling on
 */
void stopLcdScroll(byte row);

#endif
