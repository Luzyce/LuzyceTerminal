#pragma once

#include <string>

class ILcd {
 public:
  virtual void init() = 0;
  virtual void print(uint8_t x, uint8_t y, std::string data) = 0;
  virtual ~ILcd() {}
};