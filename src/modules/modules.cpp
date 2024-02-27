#include "modules.hpp"

void Modules::init() {
  net.init();

  Wire.begin();

  if (scan.scan() != 3) {
    Serial.println("[ERR] NIEPOPRAWNA ILOSC PODŁĄCZONYCH URZĄDZEŃ");
    ESP.restart();
  }

  key.init();
  mcp.init();
  lcd.init();
  nfc.init();
  qr.init();
}