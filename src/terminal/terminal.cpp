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
  Terminal* terminal =
      static_cast<Terminal*>(reinterpret_cast<void**>(pvParameters)[0]);
  IMcp* mcp = static_cast<IMcp*>(reinterpret_cast<void**>(pvParameters)[1]);
  IConsole* cons =
      static_cast<IConsole*>(reinterpret_cast<void**>(pvParameters)[2]);
  ILcd* lcd = static_cast<ILcd*>(reinterpret_cast<void**>(pvParameters)[3]);
  EspClass* ESP =
      static_cast<EspClass*>(reinterpret_cast<void**>(pvParameters)[4]);

  mcp->resetBtn();
  lcd->clear();
  lcd->print(0, 0, "Resetuje terminal");
  mcp->statusLed(LEDR);
  terminal->buzzer(false);
  cons->print("Button Reset Pressed");
  delay(300);
  ESP->restart();
}

void Terminal::printDocumentInfo() {
  lcd.print(0, 1,
            "Dobrych:   " + std::to_string(doc["Data"]["Dobrych"].as<int>()));
  lcd.print(0, 2,
            "Zlych:     " + std::to_string(doc["Data"]["Zlych"].as<int>()));
  lcd.print(0, 3,
            "DoPoprawy: " + std::to_string(doc["Data"]["DoPoprawy"].as<int>()));
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
  xTaskCreatePinnedToCore(Reset, "Reset", 10000, params, 1, NULL, 1);

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
      return;
    }

    cons.print("TYP: " + std::string(1, button.type) +
               " STAN: " + button.pole.c_str());
    mcp.statusLed(LEDB);

    if (button.pole == "KoniecKwita") {
      lcd.clear();
      lcd.print(0, 3, "Koniec kwita");
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
      buzzer(true);
      mcp.statusLed(LEDG);
    }
  }
}