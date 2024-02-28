#include "lcd.hpp"

Lcd::Lcd() : lcd(0x27, 20, 4) {}

void Lcd::init() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}