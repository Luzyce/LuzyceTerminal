#include "keypad.hpp"

constexpr byte MatrixKeypad::rowPins[4];
constexpr byte MatrixKeypad::colPins[4];
constexpr char MatrixKeypad::keys[4][4];

MatrixKeypad::MatrixKeypad()
    : keypad(makeKeymap(keys), const_cast<byte*>(rowPins),
             const_cast<byte*>(colPins), 4, 4, 0x20, PCF8574) {}

void MatrixKeypad::init() { keypad.begin(makeKeymap(keys)); }

char MatrixKeypad::read() {
  while (true) {
    char key = keypad.getKey();

    if (key) {
        return key;
    } 
  }
}