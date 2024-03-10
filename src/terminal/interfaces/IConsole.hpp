#pragma once
#include <string>

class IConsole {
 public:
  virtual void print(std::string data) = 0;
  virtual ~IConsole() {}
};