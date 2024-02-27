#include <Arduino.h>

#include "modules/networking/networking.hpp"
#include "modules/scanner/scanner.hpp"
#include "modules/keypad/keypad.hpp"
#include "modules/mcp/mcp.hpp"
#include "modules/lcd/lcd.hpp"
#include "modules/nfc/nfc.hpp"
#include "modules/qr/qr.hpp"

#include "modules/modules.hpp"

Networking net;
Scanner scan;
MatrixKeypad key;
Mcp mcp;
Lcd lcd;
Nfc nfc;
Qr qr;

Modules modules(net, scan, key, mcp, lcd, nfc, qr);

void setup() {
  Serial.begin(115200);
  modules.init();
}

void loop() {}
