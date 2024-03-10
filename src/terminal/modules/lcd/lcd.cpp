#include "lcd.hpp"

Lcd::Lcd() : lcd(0x27, 20, 4) {}

void Lcd::init() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void Lcd::print(uint8_t x, uint8_t y, std::string data) {
  lcd.setCursor(x, y);
  lcd.print(String(data.c_str()));
}

void Lcd::clear() {
  lcd.clear();
}