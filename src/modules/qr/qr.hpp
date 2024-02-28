#pragma once

#include <Arduino.h>

#include "IQr.hpp"

class Qr : public IQr {
 public:
  void init() override;
};