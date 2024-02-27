#pragma once

#include "keypad/keypad.hpp"
#include "lcd/lcd.hpp"
#include "mcp/mcp.hpp"
#include "networking/networking.hpp"
#include "nfc/nfc.hpp"
#include "qr/qr.hpp"
#include "scanner/scanner.hpp"

class Modules {
  Networking net;
  Scanner scan;
  MatrixKeypad key;
  Mcp mcp;
  Lcd lcd;
  Nfc nfc;
  Qr qr;

 public:
  void init();
};