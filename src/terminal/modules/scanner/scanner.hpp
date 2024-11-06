#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "../../interfaces/IScanner.hpp"

class Scanner : public IScanner {
  byte error, address;
  int nDevices;

 public:
  int scan() override;
};
