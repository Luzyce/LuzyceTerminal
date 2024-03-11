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
    mcp.statusLed(LEDG);
    std::string nfcTag = nfc.scan();
    cons.print("NFC TAG: " + nfcTag);
    mcp.statusLed(LEDB);

    doc["login"] = nfcTag;
    std::string SerializedNfc;
    serializeJson(doc, SerializedNfc);

    requestAnswer answer = net.request("login", SerializedNfc);
    cons.print("STATUS CODE: " + std::to_string(answer.statusCode) + " DATA: " + answer.data);

    if (answer.statusCode == 200) {
        deserializeJson(doc, answer.data);
        std::string dispName = doc["Data"]["displayName"];

        lcd.clear();
        lcd.print(0,0, "Zalogowano jako");
        lcd.print(0,1, dispName);
        mcp.statusLed(LEDG);
    } else {
        lcd.print(0,2, "Sprobuj ponownie");
        mcp.statusLed(LEDR);
        delay(3000);
        mcp.statusLed(LEDG);
        return;
    }

    lcd.print(0,2, "Zeskanuj dokument ");
    std::string doc = qr.scan();
    cons.print("DOC NUM: " + doc);
    mcp.statusLed(LEDB);
}