// Include the EasyLCD library.
#include <EasyLCD.h>

// You give it the pins you're using, just like LiquidCrystal.
EasyLCD lcd(7, 8, 9, 10, 11, 12);

// Put a space at the end so there's a space between the first and last word.
char fastScroll[] = "This Message Will Scroll Fast ";
// - 1 because the null terminator adds a byte of length.
uint16_t fastScrollLength = sizeof(fastScroll) / sizeof(char) - 1;

char slowScroll[] = "This Message Will Scroll Slow ";
uint16_t slowScrollLength = sizeof(slowScroll) / sizeof(char) - 1;

void setup() {
  // Set the number of columns and rows.
  lcd.begin(16, 2);

  lcd.setRow(0, fastScroll, fastScrollLength);
  lcd.setRow(1, slowScroll, slowScrollLength);

  lcd.setScrollPause(0, 300);
  lcd.setScrollPause(1, 1000);
}

void loop() {
  lcd.refresh(); // Should be called once every cycle.
}
