#include "terminal.hpp"

Terminal::Terminal(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp,
                   ILcd& lcd, INfc& nfc, IQr& qr, IConsole& cons)
    : net(net),
      scan(scan),
      key(key),
      mcp(mcp),
      lcd(lcd),
      nfc(nfc),
      qr(qr),
      cons(cons) {}

void Reset(void* pvParameters) {
  IMcp* mcp = static_cast<IMcp*>(reinterpret_cast<void**>(pvParameters)[0]);
  IConsole* cons = static_cast<IConsole*>(reinterpret_cast<void**>(pvParameters)[1]);
  EspClass* ESP = static_cast<EspClass*>(reinterpret_cast<void**>(pvParameters)[2]);

  mcp->resetBtn();
  mcp->statusLed(LEDR);
  cons->print("Button Reset Pressed");
  ESP->restart();
}

void Terminal::init() {
  net.init();
  Wire.begin();
  cons.print("CONNECTED DEVICES: " + std::to_string(scan.scan()));
  key.init();
  mcp.init();
  lcd.init();
  nfc.init();
  qr.init();

  void* params[3] = {&mcp, &cons, &ESP};
  xTaskCreatePinnedToCore(Reset, "Reset", 10000, params, 1, NULL, 1);

  cons.print("DEVICES INITIALIZED");
}

void Terminal::printDocumentInfo() {
  lcd.print(0, 1,
            "Dobrych:   " + std::to_string(doc["Data"]["Dobrych"].as<int>()));
  lcd.print(0, 2,
            "Zlych:     " + std::to_string(doc["Data"]["Zlych"].as<int>()));
  lcd.print(0, 3,
            "DoPoprawy: " + std::to_string(doc["Data"]["DoPoprawy"].as<int>()));
}

void Terminal::process() {
  lcd.print(0, 0, "Zaloguj sie");
  lcd.print(0, 1, "Uzyj swojej karty");

  // NFC
  mcp.statusLed(LEDG);
  std::string nfcTag = nfc.scan();
  cons.print("NFC TAG: " + nfcTag);
  mcp.statusLed(LEDB);

  doc["login"] = nfcTag;
  std::string serializedNfc;
  serializeJson(doc, serializedNfc);
  doc.clear();

  requestAnswer requestAnswer = net.request("login", serializedNfc);
  cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
             " DATA: " + requestAnswer.data);

  if (requestAnswer.statusCode == 200) {
    deserializeJson(doc, requestAnswer.data);
    std::string dispName = doc["Data"]["displayName"];
    doc.clear();

    lcd.clear();
    lcd.print(0, 0, "Zalogowano jako");
    lcd.print(0, 1, dispName);
    mcp.statusLed(LEDG);
  } else {
    lcd.print(0, 2, "Sprobuj ponownie");
    mcp.statusLed(LEDR);
    delay(3000);
    mcp.statusLed(LEDG);
    return;
  }

  // SKANER QR
  lcd.print(0, 2, "Zeskanuj dokument ");
  ScanAnswer scanAnswer = qr.scan();

  if (scanAnswer.status > 0) {
    mcp.statusLed(LEDR);
    lcd.clear();
    lcd.print(0, 0, "Przekroczono limit");
    lcd.print(0, 1, "czasu");
    lcd.print(0, 2, "Sprobuj ponownie");
    delay(3000);
    return;
  }

  std::string document = scanAnswer.scan;
  cons.print("DOC NUM: " + document);
  mcp.statusLed(LEDB);

  doc["dokument"] = document;
  std::string serializedDoc;
  serializeJson(doc, serializedDoc);
  doc.clear();

  requestAnswer = net.request("otworzDokument", serializedDoc);
  cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
             " DATA: " + requestAnswer.data);

  if (requestAnswer.statusCode == 200) {
    deserializeJson(doc, requestAnswer.data);
    lcd.clear();
    lcd.print(0, 0, "Dok: " + document);
    printDocumentInfo();

    doc.clear();
    mcp.statusLed(LEDG);
  } else {
    lcd.clearLine(2);
    lcd.print(0, 2, "Sprobuj ponownie");
    mcp.statusLed(LEDR);
    delay(3000);
    mcp.statusLed(LEDG);
    lcd.clearLine(0);
    lcd.clearLine(1);
    return;
  }

  // BUTTONY
  while (true) {
    readBtnAnswer button = mcp.readBtn();

    if (button.status > 0) {
      mcp.statusLed(LEDR);
      lcd.clear();
      lcd.print(0, 0, "Przekroczono limit");
      lcd.print(0, 1, "czasu");
      lcd.print(0, 2, "Sprobuj ponownie");
      delay(3000);
      return;
    }

    cons.print("TYP: " + std::string(1, button.type) +
               " STAN: " + button.pole.c_str());
    mcp.statusLed(LEDB);

    if (button.pole == "KoniecKwita") {
      lcd.clear();
      lcd.print(0, 3, "Koniec kwita");
      mcp.statusLed(LEDG);
      return;
    } else if (button.type == '+' && button.pole == "Zlych") {
      lcd.clearLine(0);
      lcd.print(0, 0, "Kod Bledu: ");
      char codeCharacter;
      std::string fullCode;
      while (true) {
        mcp.statusLed(LEDG);
        codeCharacter = key.read();
        cons.print(std::string(1, codeCharacter));
        mcp.statusLed(LEDB);

        if (std::isdigit(codeCharacter) && fullCode.size() < 2) {
          fullCode += codeCharacter;
          lcd.print(10 + fullCode.length(), 0, std::string(1, codeCharacter));
        } else if (codeCharacter == '*' && fullCode.size() > 0) {
          fullCode.pop_back();
          lcd.print(11 + fullCode.length(), 0, " ");
        } else if (codeCharacter == '#' && fullCode.size() == 2) {
          doc["kodbledu"] = fullCode;
          break;
        }
      }
    }

    doc["typ"] = std::string(1, button.type);
    doc["pole"] = button.pole;
    doc["terminal"] = "1";
    std::string serializedBtn;
    serializeJson(doc, serializedBtn);
    doc.clear();

    requestAnswer = net.request("operacja", serializedBtn);
    cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
               " DATA: " + requestAnswer.data);
    if (requestAnswer.statusCode == 200) {
      deserializeJson(doc, requestAnswer.data);
      lcd.clear();
      printDocumentInfo();

      doc.clear();
      mcp.statusLed(LEDG);
    }
  }
}