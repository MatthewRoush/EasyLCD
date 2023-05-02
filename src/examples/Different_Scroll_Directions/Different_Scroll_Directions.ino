// Include the EasyLCD library.
#include <EasyLCD.h>

// You give it the pins you're using, just like LiquidCrystal.
EasyLCD lcd(7, 8, 9, 10, 11, 12);

// Put a space at the end so there's a space between the first and last word.
char scrollTextRight[] = "Scroll This Message To The Right ";
// - 1 because the null terminator adds a byte of length.
uint16_t scrollTextRightLength = sizeof(scrollTextRight) / sizeof(char) - 1;

char scrollTextWithin[] = "Scroll This Message Within Its Length";
uint16_t scrollTextWithinLength = sizeof(scrollTextWithin) / sizeof(char) - 1;

void setup() {
  // Set the number of columns and rows.
  lcd.begin(16, 2);

  lcd.setRow(0, scrollTextRight, scrollTextRightLength);
  lcd.setRow(1, scrollTextWithin, scrollTextWithinLength);

  lcd.scrollRight(0);
  lcd.scrollWithin(1);
}

void loop() {
  lcd.refresh(); // Should be called once every cycle.
}
