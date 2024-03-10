#include "terminal.hpp"

Terminal::Terminal(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd, INfc& nfc, IQr& qr, IConsole& cons)
    : net(net), scan(scan), key(key), mcp(mcp), lcd(lcd), nfc(nfc), qr(qr), cons(cons) {}


void Terminal::init() {
    net.init();
    Wire.begin();
    cons.print("CONNECTED DEVICES: " + std::to_string(scan.scan()));
    key.init();
    mcp.init();
    lcd.init();
    nfc.init();
    qr.init();
    cons.print("DEVICES INITIALIZED");
}

void Terminal::process() {
    lcd.print(0,0, "Zaloguj sie");
    lcd.print(0,1, "Uzyj swojej karty");

    //NFC
    std::string nfcTag = nfc.scan();
    cons.print("NFC TAG: " + nfcTag);

    JsonDocument doc;
    doc["login"] = nfcTag;
    std::string SerializedNfc;
    serializeJson(doc, SerializedNfc);

    requestAnswer answer = net.request("login", SerializedNfc);
    cons.print("STATUS CODE: " + std::to_string(answer.statusCode));
    cons.print("DATA: " + answer.data);
    lcd.clear();
}