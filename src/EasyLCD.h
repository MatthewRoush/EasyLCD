#pragma once

#include <LiquidCrystal.h>

#define RIGHT -1
#define LEFT 1

struct row {
  char* text; // The text to display on the row.
  uint16_t length; // The length of the text, without the null terminator.
  uint16_t startIndex = 0; // The index to start the text slice.
  unsigned long prevScrollTime = 0; // The last time the text scrolled.
  uint16_t scrollPause = 600; // The time (in milliseconds) to pause between each scroll.
  int8_t scrollDirection = 1; // The direction to scroll the text.
  bool scrollWithin = false; // Scroll text from one side to the other.
  bool rightToLeft = false; // Print the text right to left on the LCD.
  bool printTextNow = true; // Print the text now instead of waiting for scrollPause milliseconds to pass.
};

class EasyLCD: public LiquidCrystal {
  public:
    EasyLCD(uint8_t rs, uint8_t enable,
            uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
            uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    EasyLCD(uint8_t rs, uint8_t rw, uint8_t enable,
            uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
            uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
    EasyLCD(uint8_t rs, uint8_t rw, uint8_t enable,
            uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    EasyLCD(uint8_t rs, uint8_t enable,
            uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    ~EasyLCD();
    void begin(uint8_t cols, uint8_t rows);
    void setRow(uint8_t rowIndex, char* text, uint16_t textLen);
    void setRow(uint8_t rowIndex, String text);
    void scrollLeft(uint8_t rowIndex);
    void scrollRight(uint8_t rowIndex);
    void scrollStop(uint8_t rowIndex);
    void scrollWithin(uint8_t rowIndex);
    void setScrollPause(uint8_t rowIndex, uint16_t pauseDuration);
    void refresh();
    void refresh(uint8_t rowIndex);
    void leftToRight(uint8_t rowIndex);
    void rightToLeft(uint8_t rowIndex);
  private:
    void sliceText(char* text, uint16_t textLen, uint16_t start, char* dest);
    void copyCharArr(char* text, uint16_t textLen, char* dest);
    void iterateIndex(uint8_t rowIndex);
    char* stringToCharArray(String text);

    uint8_t _cols; // Columns on the LCD.
    uint8_t _rows; // Rows on the LCD.

    char* _displayText; // The text to display on the current row.

    row* _rowData; // Data for each row.
};
