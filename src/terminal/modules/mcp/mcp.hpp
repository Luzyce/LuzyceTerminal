#pragma once

#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <Wire.h>

#include "../../interfaces/IMcp.hpp"

#define LEDR 10
#define LEDG 11
#define LEDB 12

class Mcp : public IMcp {
  Adafruit_MCP23X17 mcp;

 public:
  void init() override;
  void statusLed(int led) override;
  readBtnAnswer readBtn() override;
  void resetBtn() override;
};