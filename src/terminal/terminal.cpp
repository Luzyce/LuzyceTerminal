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
    std::string serializedNfc;
    serializeJson(doc, serializedNfc);
    doc.clear();

    requestAnswer answer = net.request("login", serializedNfc);
    cons.print("STATUS CODE: " + std::to_string(answer.statusCode) + " DATA: " + answer.data);

    if (answer.statusCode == 200) {
        deserializeJson(doc, answer.data);
        std::string dispName = doc["Data"]["displayName"];
        doc.clear();

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
    std::string document = qr.scan();
    cons.print("DOC NUM: " + document);
    mcp.statusLed(LEDB);

    doc["dokument"] = document;
    std::string serializedDoc;
    serializeJson(doc, serializedDoc);
    doc.clear();

    answer = net.request("otworzDokument", serializedDoc);
    cons.print("STATUS CODE: " + std::to_string(answer.statusCode) + " DATA: " + answer.data);

    if (answer.statusCode == 200) {
        deserializeJson(doc, answer.data);
        lcd.clear();
        lcd.print(0,0, "Dok: " + document);
        lcd.print(0,1, "Dobrych:   " + std::to_string(doc["Data"]["Dobrych"].as<int>()));
        lcd.print(0,2, "Zlych:     " + std::to_string(doc["Data"]["Zlych"].as<int>()));
        lcd.print(0,3, "DoPoprawy: " + std::to_string(doc["Data"]["DoPoprawy"].as<int>()));

        doc.clear();
        mcp.statusLed(LEDG);
    } else {
        lcd.print(0,2,"                    ");
        lcd.print(0,2, "Sprobuj ponownie");
        mcp.statusLed(LEDR);
        delay(3000);
        mcp.statusLed(LEDG);
        lcd.print(0,0,"                    ");
        lcd.print(0,1,"                    ");
        return;
    }

    while(1) {
        yield();
    }
}