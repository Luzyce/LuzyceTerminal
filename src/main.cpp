#include <Arduino.h>

#include "terminal/modules/networking/networking.hpp"
#include "terminal/modules/scanner/scanner.hpp"
#include "terminal/modules/keypad/keypad.hpp"
#include "terminal/modules/error/error.hpp"
#include "terminal/modules/mcp/mcp.hpp"
#include "terminal/modules/lcd/lcd.hpp"
#include "terminal/modules/nfc/nfc.hpp"
#include "terminal/modules/qr/qr.hpp"

#include "terminal/terminal.hpp"

Networking net;
Scanner scan;
MatrixKeypad key;
Error error;
Mcp mcp;
Lcd lcd;
Nfc nfc;
Qr qr;

Terminal terminal(net, scan, key, mcp, lcd, nfc, qr, error);

void setup() {
  Serial.begin(115200);
  terminal.init();
}

void loop() {}
