#pragma once

#define LEDR 10
#define LEDG 11
#define LEDB 12

struct readBtnAnswer {
  char type;
  std::string pole;
};

class IMcp {
 public:
  virtual void init() = 0;
  virtual void statusLed(int led) = 0;
  virtual readBtnAnswer readBtn() = 0;
  virtual ~IMcp() {}
};