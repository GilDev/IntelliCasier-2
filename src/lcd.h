#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

#define LCD_BLACK_SQUARE 0
#define LCD_UP_ARROW     1
#define LCD_RIGHT_ARROW  2
#define LCD_DOWN_ARROW   3

extern LiquidCrystal_I2C lcd;

/**
 * Initialize LCD display
 */
void lcdInit(void);

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
 * Print text on the LCD display
 * @param x    Horizontal coordinate
 * @param y    Vertical coordinate
 * @param text Text to print
 */
#define printLcd(x, y, text) {lcd.setCursor(x, y); lcd.print(text);}

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
