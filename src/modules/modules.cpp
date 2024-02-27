#include "modules.hpp"

Modules::Modules(Networking& net, Scanner& scan, MatrixKeypad& key, Mcp& mcp, Lcd& lcd, Nfc& nfc, Qr& qr)
    : net(net), scan(scan), key(key), mcp(mcp), lcd(lcd), nfc(nfc), qr(qr) {}

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
