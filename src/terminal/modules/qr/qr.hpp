#pragma once

#include <Arduino.h>

#include "../../interfaces/IQr.hpp"

class Qr : public IQr {
 public:
  void init() override;
};