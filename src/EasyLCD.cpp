#include "EasyLCD.h"
#include "Arduino.h"

// Public methods.

EasyLCD::EasyLCD(uint8_t rs, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7
) : LiquidCrystal(rs, enable,
                  d0, d1, d2, d3,
                  d4, d5, d6, d7) {}

EasyLCD::EasyLCD(uint8_t rs, uint8_t rw, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7
) : LiquidCrystal(rs, rw, enable,
                  d0, d1, d2, d3,
                  d4, d5, d6, d7) {}

EasyLCD::EasyLCD(uint8_t rs, uint8_t rw, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3
) : LiquidCrystal(rs, rw, enable,
                  d0, d1, d2, d3) {}

EasyLCD::EasyLCD(uint8_t rs, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3
) : LiquidCrystal(rs, enable,
                  d0, d1, d2, d3) {}

// Liberate some memory.
EasyLCD::~EasyLCD() {
  delete [] _displayText;
  delete [] _rowData;
}

void EasyLCD::begin(uint8_t cols, uint8_t rows) {
  _cols = cols;
  _rows = rows;

  _rowData = new row[_rows];

  // char array for the text to display on the current row.
  // +1 for the null terminator.
  _displayText = new char[_cols + 1];

  LiquidCrystal::begin(_cols, _rows);
}

// Set the text for the given row.
void EasyLCD::setRow(uint8_t rowIndex, char* text, uint16_t textLen) {
  _rowData[rowIndex].text = text;
  _rowData[rowIndex].length = textLen;
  _rowData[rowIndex].startIndex = 0;
  _rowData[rowIndex].printTextNow = true;
}

// Set the text for the given row.
void EasyLCD::setRow(uint8_t row, String text) {
  setRow(row, stringToCharArray(text), text.length());
}

// Make the text scroll left.
void EasyLCD::scrollLeft(uint8_t rowIndex) {
  _rowData[rowIndex].scrollDirection = LEFT;
  _rowData[rowIndex].scrollWithin = false;
}

// Make the text scroll Right.
void EasyLCD::scrollRight(uint8_t rowIndex) {
  _rowData[rowIndex].scrollDirection = RIGHT;
  _rowData[rowIndex].scrollWithin = false;
}

// Stop the text from scrolling.
void EasyLCD::scrollStop(uint8_t rowIndex) {
  _rowData[rowIndex].scrollDirection = 0;
}

// Scroll to the right end of the text, then to the left
void EasyLCD::scrollWithin(uint8_t rowIndex) {
  _rowData[rowIndex].scrollWithin = true;
  // Make sure the text is within bounds.
  if (_rowData[rowIndex].length - _rowData[rowIndex].startIndex - (unsigned) 1 <= _cols) {
    _rowData[rowIndex].startIndex = _rowData[rowIndex].length - _cols;
  }
}

// Set the time (milliseconds) to pause between each scroll movement.
void EasyLCD::setScrollPause(uint8_t rowIndex, uint16_t pauseDuration) {
  _rowData[rowIndex].scrollPause = pauseDuration;
  _rowData[rowIndex].prevScrollTime = millis();
}

// Update all the rows of the LCD.
// Should be called once every loop of the loop() function.
void EasyLCD::refresh() {
  for (uint8_t i = 0; i < _rows; ++i) {
    refresh(i);
  }
}

// Update the specified row on the LCD.
// Should be called once every loop of the loop() function.
void EasyLCD::refresh(uint8_t rowIndex) {
  row data = _rowData[rowIndex];
  if (millis() - data.prevScrollTime > data.scrollPause || data.printTextNow) {
    _rowData[rowIndex].prevScrollTime = millis(); // Update the previous scroll time.
    _rowData[rowIndex].printTextNow = false;

    if (data.scrollDirection != 0) {
      // Get the text to display on row 1.
      if (data.length > _cols) {
        sliceText(data.text, data.length, data.startIndex, _displayText);
        iterateIndex(rowIndex);
      } else {
        copyCharArr(data.text, data.length, _displayText);
      }
    } else {
      copyCharArr(data.text, data.length, _displayText);
    }

    if (data.text != NULL){
      if (data.rightToLeft) {
        LiquidCrystal::rightToLeft();
        LiquidCrystal::setCursor(_cols - 1, rowIndex);
      } else {
        LiquidCrystal::leftToRight();
        LiquidCrystal::setCursor(0, rowIndex);
      }
      LiquidCrystal::print(_displayText);
    }
  }
}

// Text will print left to right for the given row.
void EasyLCD::leftToRight(uint8_t rowIndex) {
  _rowData[rowIndex].rightToLeft = false;
}

// // Text will print right to left for the given row.
void EasyLCD::rightToLeft(uint8_t rowIndex) {
  _rowData[rowIndex].rightToLeft = true;
}

// Private methods.

// Slice out _cols number of characters from text starting at start and
// copy them into dest.
void EasyLCD::sliceText(char* text, uint16_t textLen, uint16_t start, char* dest) {
  char slice[_cols + 1]; // +1 for null terminator.

  // If there aren't enough characters in text starting at start
  // to fill _cols.
  if (start + _cols >= textLen) {
    // First half = text starting at start and going to the end of text.
    // Second half = the remaining characters to fill _cols starting at
    // the beginning of text.
    uint16_t firstHalfLen = textLen - start;
    uint16_t secondHalfLen = _cols - firstHalfLen;

    memcpy(slice, text + start, firstHalfLen);
    // Manually copy characters into slice because I don't know of another way.
    for (uint8_t i = 0; i < secondHalfLen; ++i) {
      slice[firstHalfLen + i] = text[i];
    }
  } else {
    memcpy(slice, text + start, _cols);
  }

  slice[_cols] = '\0'; // Insert null terminator.
  memcpy(dest, slice, _cols + 1);
}

// Copy text into dest while trying to avoid undefined behavior.
void EasyLCD::copyCharArr(char* text, uint16_t textLen, char* dest) {
  // If NULL then behavior will be undefined, so don't do that.
  if (text != NULL) {
    // Use textLen or _cols, for the number of characters to copy
    // from text into dest, depending on which one is smaller.
    memcpy(dest, text, min(textLen, _cols));

    // For every character that _cols is longer than textLen',
    // add a ' ' character to the end of dest.
    // This is so that any previously displayed characters on the LCD are
    // properly printed over.
    for (uint16_t i = textLen; i <  _cols; ++i) {
      dest[i] = ' ';
    }
    dest[_cols] = '\0'; // Insert null terminator.
  }
}

// Interate index based on scroll and keep within 0 and rowLen - 1.
void EasyLCD::iterateIndex(uint8_t rowIndex) {
  uint16_t start = _rowData[rowIndex].startIndex;
  uint16_t textLen = _rowData[rowIndex].length;
  int8_t direction = _rowData[rowIndex].scrollDirection;
  bool scrollWithin = _rowData[rowIndex].scrollWithin;
  
  if (direction == RIGHT) {
    if (start == 0) {
      if (scrollWithin) {
        scrollLeft(rowIndex);
        _rowData[rowIndex].scrollWithin = true;
        start += LEFT;
      } else {
        start = textLen - 1;
      }
    } else {
      // index += -1, so don't do if index == 0 because index is unsigned.
      start += direction;
    }
  } else {
    if (scrollWithin && textLen - start <= _cols) {
      scrollRight(rowIndex);
      _rowData[rowIndex].scrollWithin = true;
      start += RIGHT;
    } else if (start >= textLen - 1) {
      start = 0;
    } else {
      start += direction;
    }
  }
  _rowData[rowIndex].startIndex = start;
}

// Return a char array with the same content as text.
char* EasyLCD::stringToCharArray(String text) {
  uint16_t length = text.length() + 1;
  char* charArray = new char[length];
  memcpy(charArray, text.c_str(), length);
  return charArray;
}
