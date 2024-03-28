#pragma once

#include <Arduino.h>

#include "../../interfaces/IQr.hpp"

class Qr : public IQr {
  std::string fullScan;
  ScanAnswer answer;

 public:
  void init() override;
  ScanAnswer scan() override;
};