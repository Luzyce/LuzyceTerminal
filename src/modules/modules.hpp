#pragma once

#include "networking/networking.hpp"
#include "scanner/scanner.hpp"
#include "keypad/keypad.hpp"
#include "mcp/mcp.hpp"
#include "lcd/lcd.hpp"
#include "nfc/nfc.hpp"

class Modules {
  Networking net;
  Scanner scan;
  MatrixKeypad key;
  Mcp mcp;
  Lcd lcd;
  Nfc nfc;

 public:
  void init();
};