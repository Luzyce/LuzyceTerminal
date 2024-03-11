#include "mcp.hpp"

void Mcp::init() {
  if (!mcp.begin_I2C(0x24)) ESP.restart();

  for (int i = 0; i < 7; i++) mcp.pinMode(i, INPUT_PULLUP);
  mcp.pinMode(LEDR, OUTPUT);
  mcp.pinMode(LEDG, OUTPUT);
  mcp.pinMode(LEDB, OUTPUT);
}

void Mcp::statusLed(int led) {
  mcp.digitalWrite(LEDR, (led == LEDR) ? HIGH : LOW);
  mcp.digitalWrite(LEDG, (led == LEDG) ? HIGH : LOW);
  mcp.digitalWrite(LEDB, (led == LEDB) ? HIGH : LOW);
}