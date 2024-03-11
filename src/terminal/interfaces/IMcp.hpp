#pragma once

#define LEDR 10
#define LEDG 11
#define LEDB 12
  
class IMcp {
 public:

  virtual void init() = 0;
  virtual void statusLed(int led) = 0;
  virtual ~IMcp() {}
};