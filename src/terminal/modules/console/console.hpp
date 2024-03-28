#pragma once

#include <Arduino.h>

#include "../../interfaces/IConsole.hpp"

class Console : public IConsole {
 public:
  void print(std::string data) override;
};