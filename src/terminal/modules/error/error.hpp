#pragma once

#include <Arduino.h>

#include "../../interfaces/IError.hpp"

class Error : public IError {

 public:
  void error(int errorType) override;
};