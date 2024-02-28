#pragma once

#include <Arduino.h>

#include "IError.hpp"

class Error : public IError {

 public:
  void error(int errorType) override;
};