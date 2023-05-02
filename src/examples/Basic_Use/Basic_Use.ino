// Include the EasyLCD library.
#include <EasyLCD.h>

// You give it the pins you're using, just like LiquidCrystal.
EasyLCD lcd(7, 8, 9, 10, 11, 12);

// Put a space at the end so there's a space between the first and last word.
char longMessage[] = "Long Test Message That Will Scroll ";
// - 1 because the null terminator adds a byte of length.
uint16_t longMessageLength = sizeof(longMessage) / sizeof(char) - 1;

// Won't scroll, so no need for a space at the end.
String shortMessage = "Short Message";

void setup() {
  // Set the number of columns and rows.
  lcd.begin(16, 2);
  // Set the text on row 1 (index 0) using a char array.
  lcd.setRow(0, longMessage, longMessageLength);
  // Set the text on row 2 (index 1) using a String.
  lcd.setRow(1, shortMessage);
}

void loop() {
  lcd.refresh(); // Should be called once every cycle.
}
