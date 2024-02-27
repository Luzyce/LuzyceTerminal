#pragma once

#include <LiquidCrystal_I2C.h>

class Lcd {
  LiquidCrystal_I2C lcd;

 public:
  Lcd();
  void init();
};