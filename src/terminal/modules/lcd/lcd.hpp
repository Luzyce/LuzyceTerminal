#pragma once

#include <LiquidCrystal_I2C.h>

#include "../../interfaces/ILcd.hpp"

class Lcd : public ILcd {
  LiquidCrystal_I2C lcd;

 public:
  Lcd();
  void init() override;
};