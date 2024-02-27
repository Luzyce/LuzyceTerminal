#include "modules.hpp"

Modules::Modules(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd, INfc& nfc, IQr& qr)
    : net(net), scan(scan), key(key), mcp(mcp), lcd(lcd), nfc(nfc), qr(qr) {}

void Modules::init() {
    net.init();

    Wire.begin();

    if (scan.scan() != 3) {
        Serial.println("[ERR] NIEPOPRAWNA ILOSC PODŁĄCZONYCH URZĄDZEŃ");
        ESP.restart(); // Make sure ESP.h is included if using ESP.restart() or adjust for your platform
    }

    key.init();
    mcp.init();
    lcd.init();
    nfc.init();
    qr.init();
}