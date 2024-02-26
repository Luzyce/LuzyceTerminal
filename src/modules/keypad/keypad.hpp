#pragma once

#include <Arduino.h>
#include <Keypad_I2C.h>

class MatrixKeypad {
  Keypad_I2C keypad;
  static constexpr byte rowPins[4] = { 0, 1, 2, 3 };
  static constexpr byte colPins[4] = { 4, 5, 6, 7 };
  static constexpr char keys[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
  };
public:
  MatrixKeypad();
  void init();
};
