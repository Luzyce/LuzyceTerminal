#pragma once

#include <string>

class IQr {
 public:
  virtual void init() = 0;
  virtual std::string scan() = 0;
  virtual ~IQr() {}
  
  bool timeout;
};