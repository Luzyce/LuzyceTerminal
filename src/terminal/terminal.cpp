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
  auto* mcp = static_cast<IMcp*>(reinterpret_cast<void**>(pvParameters)[1]);
  auto* cons =
      static_cast<IConsole*>(reinterpret_cast<void**>(pvParameters)[2]);
  auto* lcd = static_cast<ILcd*>(reinterpret_cast<void**>(pvParameters)[3]);
  auto* esp =
      static_cast<EspClass*>(reinterpret_cast<void**>(pvParameters)[4]);

  mcp->resetBtn();
  lcd->clear();
  lcd->print(0, 0, "Resetuje terminal");
  mcp->statusLed(LEDR);
  Terminal::buzzer(false);
  cons->print("Button Reset Pressed");
  delay(300);
  esp->restart();
}

void Terminal::buzzer(bool state) {
  if (state) {
    tone(BUZZER, 100, 100);
    tone(BUZZER, 125, 100);
    tone(BUZZER, 150, 100);
  } else {
    tone(BUZZER, 150, 100);
    tone(BUZZER, 125, 100);
    tone(BUZZER, 100, 100);
  }
}

void Terminal::init() {
  net.init();
  Wire.begin();
  ledcSetup(0, 5000, 8);
  ledcAttachPin(BUZZER, 0);
  cons.print("CONNECTED DEVICES: " + std::to_string(scan.scan()));
  key.init();
  mcp.init();
  lcd.init();
  nfc.init();
  qr.init();

  void* params[5] = {this, &mcp, &cons, &lcd, &ESP};
  xTaskCreatePinnedToCore(Reset, "Reset", 10000, params, 1, nullptr, 1);

  buzzer(true);
  cons.print("DEVICES INITIALIZED");
}

void Terminal::process() {
  lcd.print(0, 0, "Zaloguj sie");
  lcd.print(0, 1, "Uzyj swojej karty");

  // NFC
  mcp.statusLed(LEDG);
  std::string nfcTag = nfc.scan();
  cons.print("NFC TAG: " + nfcTag);
  mcp.statusLed(LEDB);

  doc["hash"] = nfcTag;
  doc["ipAddress"] = net.getIp();
  std::string serializedNfc;
  serializeJson(doc, serializedNfc);
  doc.clear();

  requestAnswer requestAnswer = net.request("login", serializedNfc);
  cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
             " DATA: " + requestAnswer.data);

  if (requestAnswer.statusCode == 200) {
    deserializeJson(doc, requestAnswer.data);
    std::string displayName = std::string((const char*)doc["result"]["name"]) + " " + std::string((const char*)doc["result"]["lastName"]);
    doc.clear();

    lcd.clear();
    lcd.print(0, 0, "Zalogowano jako");
    lcd.print(0, 1, displayName);

    buzzer(true);
    mcp.statusLed(LEDG);
  } else {
    lcd.print(0, 2, "Sprobuj ponownie");

    buzzer(false);
    mcp.statusLed(LEDR);
    delay(3000);
    mcp.statusLed(LEDG);
    return;
  }

  // SKANER QR
  lcd.print(0, 2, "Zeskanuj dokument ");
  ScanAnswer scanAnswer = qr.scan();

  if (scanAnswer.status > 0) {
    buzzer(false);
    mcp.statusLed(LEDR);
    lcd.clear();
    lcd.print(0, 2, "Przekroczono czas");
    return;
  }

  std::string document = scanAnswer.scan;
  cons.print("DOC NUM: " + document);
  mcp.statusLed(LEDB);

  doc["number"] = document;
  std::string serializedDoc;
  serializeJson(doc, serializedDoc);
  doc.clear();

  requestAnswer = net.request("document/getByNumber", serializedDoc);
  cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
             " DATA: " + requestAnswer.data);

  if (requestAnswer.statusCode == 200) {
    deserializeJson(doc, requestAnswer.data);
    lcd.clear();
    lcd.print(0, 0, "Dok: " + document);
    lcd.print(0, 1,
              "Dobrych:   " + std::to_string(doc["documentPositions"][0]["quantityNetto"].as<int>()));
    lcd.print(0, 2,
              "Zlych:     " + std::to_string(doc["documentPositions"][0]["quantityLoss"].as<int>()));
    lcd.print(0, 3,
              "DoPoprawy: " + std::to_string(doc["documentPositions"][0]["quantityToImprove"].as<int>()));
    documentId = std::to_string(doc["id"].as<int>());
    doc.clear();

    buzzer(true);
    mcp.statusLed(LEDG);
  } else {
    lcd.clearLine(2);
    lcd.print(0, 2, "Sprobuj ponownie");
    buzzer(false);
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
      buzzer(false);
      mcp.statusLed(LEDR);
      lcd.clear();
      lcd.print(0, 2, "Przekroczono czas");
      auto response = net.request("document/closeDocument/" + documentId, "");
      cons.print("STATUS CODE: " + std::to_string(response.statusCode) +
                 " DATA: " + response.data);
      return;
    }

    cons.print("TYP: " + std::string(1, button.type) +
               " STAN: " + button.pole);
    mcp.statusLed(LEDB);

    if (button.pole == "KoniecKwita") {
      lcd.clear();
      lcd.print(0, 3, "Koniec kwita");
      auto response = net.request("document/closeDocument/" + documentId, "");
      cons.print("STATUS CODE: " + std::to_string(response.statusCode) +
                 " DATA: " + response.data);
      buzzer(true);
      mcp.statusLed(LEDG);
      return;
    } else if (button.type == '+' && button.pole == "Zlych") {
      lcd.clearLine(0);
      lcd.print(0, 0, "Kod Bledu: ");
      char codeCharacter;
      std::string fullCode;
      while (true) {
        buzzer(true);
        mcp.statusLed(LEDG);
        codeCharacter = key.read();
        cons.print(std::string(1, codeCharacter));
        mcp.statusLed(LEDB);

        if (std::isdigit(codeCharacter) && fullCode.size() < 2) {
          fullCode += codeCharacter;
          lcd.print(10 + fullCode.length(), 0, std::string(1, codeCharacter));
        } else if (codeCharacter == '*' && !fullCode.empty()) {
          fullCode.pop_back();
          lcd.print(11 + fullCode.length(), 0, " ");
        } else if (codeCharacter == '#' && fullCode.size() == 2) {
          doc["errorCode"] = fullCode;
          break;
        }
      }
    }

    doc["type"] = std::string(1, button.type);
    doc["field"] = button.pole;
    std::string serializedBtn;
    serializeJson(doc, serializedBtn);
    doc.clear();

    requestAnswer = net.request("document/updateDocumentPositionOnKwit/" + documentId, serializedBtn);
    cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
               " DATA: " + requestAnswer.data);
    if (requestAnswer.statusCode == 200) {
      deserializeJson(doc, requestAnswer.data);
      lcd.clear();
      lcd.print(0, 1,
                "Dobrych:   " + std::to_string(doc["quantityNetto"].as<int>()));
      lcd.print(0, 2,
                "Zlych:     " + std::to_string(doc["quantityLoss"].as<int>()));
      lcd.print(0, 3,
                "DoPoprawy: " + std::to_string(doc["quantityToImprove"].as<int>()));

      doc.clear();
      buzzer(true);
      mcp.statusLed(LEDG);
    } else {
      lcd.clearLine(0);
      lcd.print(0, 0, "Sprobuj ponownie");
      buzzer(false);
      mcp.statusLed(LEDR);
      delay(3000);
      mcp.statusLed(LEDG);
      lcd.clearLine(0);
    }
  
  }
}