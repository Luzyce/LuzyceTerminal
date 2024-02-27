#pragma once

#include <Arduino.h>
#include <Wire.h>

class Scanner {
  byte error, address;
  int nDevices;

 public:
  int scan();
};