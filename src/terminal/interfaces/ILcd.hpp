#pragma once

class ILcd {
 public:
  virtual void init() = 0;
  virtual ~ILcd() {}
};