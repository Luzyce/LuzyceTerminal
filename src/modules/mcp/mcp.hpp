#pragma once

#include <Adafruit_MCP23X17.h>
#include <Arduino.h>
#include <Wire.h>

#define LEDR 10
#define LEDG 11
#define LEDB 12

class Mcp {
  Adafruit_MCP23X17 mcp;

 public:
  void init();
};