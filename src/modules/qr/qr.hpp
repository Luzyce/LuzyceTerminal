#pragma once

#include <Arduino.h>

#include "iQr.hpp"

class Qr : public IQr {
 public:
  void init() override;
};