#pragma once

#include <LiquidCrystal_I2C.h>

#include "../../interfaces/ILcd.hpp"

class Lcd : public ILcd {
  LiquidCrystal_I2C lcd;

 public:
  Lcd();
  void init() override;
  void print(uint8_t x, uint8_t y, std::string data) override;
  void clearLine(uint8_t y) override;
  void clear() override;
};