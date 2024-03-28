#pragma once

#include <string>

struct ScanAnswer {
  int status;
  std::string scan;
};

class IQr {
 public:
  virtual void init() = 0;
  virtual ScanAnswer scan() = 0;
  virtual ~IQr() {}
};