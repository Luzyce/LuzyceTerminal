#pragma once

#include <Arduino.h>

#include "../../interfaces/IQr.hpp"

class Qr : public IQr {
  std::string fullScan;

 public:
  void init() override;
  std::string scan() override;
};