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
  auto* mcp = static_cast<IMcp*>(static_cast<void**>(pvParameters)[1]);
  auto* cons =
      static_cast<IConsole*>(static_cast<void**>(pvParameters)[2]);
  auto* lcd = static_cast<ILcd*>(static_cast<void**>(pvParameters)[3]);
  auto* esp =
      static_cast<EspClass*>(static_cast<void**>(pvParameters)[4]);

  mcp->resetBtn();
  lcd->clear();
  lcd->print(0, 0, "Resetuje terminal");
  mcp->statusLed(LEDR);
  Terminal::buzzer(false);
  cons->print("Button Reset Pressed");
  delay(300);
  esp->restart();
}

#if defined(esp32)
[[noreturn]] void OTA(void *pvParameters) {
    while (true) {
        ArduinoOTA.handle();
    }
}
#endif

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

std::string Terminal::refactorPolishToEnglish(const std::string& input) {
  std::string output = input;

  const std::map<std::string, std::string> polishToEnglish = {
    {"ą", "a"}, {"ć", "c"}, {"ę", "e"}, {"ł", "l"}, {"ń", "n"},
    {"ó", "o"}, {"ś", "s"}, {"ź", "z"}, {"ż", "z"},
    {"Ą", "A"}, {"Ć", "C"}, {"Ę", "E"}, {"Ł", "L"}, {"Ń", "N"},
    {"Ó", "O"}, {"Ś", "S"}, {"Ź", "Z"}, {"Ż", "Z"}
  };

  for (const auto & it : polishToEnglish) {
    const std::string& polishChar = it.first;
    const std::string& englishChar = it.second;

    size_t pos = 0;
    while ((pos = output.find(polishChar, pos)) != std::string::npos) {
      output.replace(pos, polishChar.length(), englishChar);
      pos += englishChar.length();
    }
  }

  return output;
}

void Terminal::init() {
  Wire.begin();
  ledcSetup(0, 5000, 8);
  ledcAttachPin(BUZZER, 0);
  cons.print("CONNECTED DEVICES: " + std::to_string(scan.scan()));

  mcp.init();
  lcd.init();
  lcd.print(0, 0, "Inicjalizacja");
  net.init(&lcd, &mcp);
  net.initOTA();
  void* params[5] = {this, &mcp, &cons, &lcd, &ESP};
  xTaskCreatePinnedToCore(Reset, "Reset", 10000, params, 1, nullptr, 1);
  xTaskCreatePinnedToCore(OTA, "OTA", 4096, nullptr, 1, nullptr, 1);

  key.init();
  nfc.init();
  qr.init();

  buzzer(true);
  cons.print("DEVICES INITIALIZED");
}

void Terminal::process() {
  std::string ipAddr = net.getIp();

  lcd.clearLine(0);
  lcd.print(0, 0, "Zaloguj sie");
  lcd.print(0, 1, "Uzyj swojej karty");
  lcd.print(0, 3, "IP: " + ipAddr);

  // NFC
  mcp.statusLed(LEDG);
  std::string nfcTag = nfc.scan();
  cons.print("NFC TAG: " + nfcTag);
  mcp.statusLed(LEDB);

  doc["hash"] = nfcTag;
  doc["ipAddress"] = ipAddr;
  std::string serializedNfc;
  serializeJson(doc, serializedNfc);
  doc.clear();

  requestAnswer requestAnswer = net.request("login", serializedNfc);
  cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
             " DATA: " + requestAnswer.data);

  if (requestAnswer.statusCode == 200) {
    deserializeJson(doc, requestAnswer.data);
    std::string displayName = refactorPolishToEnglish(
      std::string(static_cast<const char*>(doc["result"]["name"])) + " " +
      std::string(static_cast<const char*>(doc["result"]["lastName"])));
    doc.clear();

    lcd.clear();
    lcd.print(0, 0, "Zalogowano jako");
    lcd.print(0, 1, displayName);

    buzzer(true);
    mcp.statusLed(LEDG);
  } else {
    if (requestAnswer.statusCode == 401)
      lcd.print(0, 2, "Niepoprawna karta");

    lcd.clearLine(3);
    lcd.print(0, 3, "Sprobuj ponownie");

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

  requestAnswer = net.request("document/terminal/getKwit", serializedDoc);
  cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
             " DATA: " + requestAnswer.data);

  if (requestAnswer.statusCode == 200) {
    deserializeJson(doc, requestAnswer.data);
    lcd.clear();
    lcd.print(0, 0, "Dok: " + std::string(static_cast<const char*>(doc["number"])));
    lcd.print(0, 1,
              "Dobrych:   " + std::to_string(doc["documentPositions"][0]["quantityNetto"].as<int>()));
    lcd.print(0, 2,
              "Do Sprawdzenia: " + std::to_string(doc["documentPositions"][0]["quantityToImprove"].as<int>()));
    documentId = std::to_string(doc["id"].as<int>());
    lcd.print(0, 3,
              "Braki:     " + std::to_string(doc["documentPositions"][0]["quantityLoss"].as<int>()));
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
               " STAN: " + button.pole);
    mcp.statusLed(LEDB);

    if (button.pole == "KoniecKwita") {
      lcd.clear();
      lcd.print(0, 3, "Koniec kwitu");
      auto response = net.request("document/terminal/closeKwit/" + documentId, "");
      cons.print("STATUS CODE: " + std::to_string(response.statusCode) +
                 " DATA: " + response.data);
      buzzer(true);
      mcp.statusLed(LEDG);
      return;
    }

    std::string fullQuantity = "1";
    bool isDefaultQuantity = true;

    if (button.type == '+') {
      lcd.clear();
      lcd.print(0, 0, "Podaj ilosc");
      lcd.print(0, 1, "Ilosc: 1");
      lcd.print(0, 3, "Ilosc jest domyslna");
      char character = '/';

      while (true) {
        mcp.statusLed(LEDG);
        while (character == '/') {
          character = key.singleRead();
          auto mcpResp = mcp.singleReadBtn();
          if (mcpResp.type == '+' && mcpResp.pole == button.pole)
          {
            character = '#';
          }
        }
        cons.print(std::string(1, character));
        mcp.statusLed(LEDB);

        if (fullQuantity == "1" && isdigit(character) && character != '0' && isDefaultQuantity) {
          fullQuantity = "";
          isDefaultQuantity = false;
          lcd.clearLine(3);
        }

        if (isdigit(character)) {
          fullQuantity += character;
        }

        if (character == '*') {
          if (fullQuantity.size() == 1) {
            fullQuantity = "1";
            isDefaultQuantity = true;
            lcd.print(0, 3, "Ilosc jest domyslna");
          } else if (fullQuantity.size() > 1) {
            fullQuantity.pop_back();
            lcd.print(7 + fullQuantity.length(), 1, " ");
          }
        }

        if (character == '#' && !fullQuantity.empty()) {
          break;
        }

        character = '/';
        lcd.print(7, 1, fullQuantity);
      }
    }

    if (button.type == '+' && button.pole == "Zlych") {
      lcd.clear();
      lcd.print(0, 0, "Podaj kod bledu");
      lcd.print(0, 1, "Kod Bledu: ");
      char codeCharacter;
      std::string fullCode;
      bool isCodeExist = false;
      while (true) {
        buzzer(true);
        mcp.statusLed(LEDG);
        codeCharacter = key.read();
        cons.print(std::string(1, codeCharacter));
        mcp.statusLed(LEDB);

        if (std::isdigit(codeCharacter) && fullCode.size() < 2) {
          fullCode += codeCharacter;
          lcd.print(10 + fullCode.length(), 1, std::string(1, codeCharacter));

          if (fullCode.size() == 2)
          {
            doc["errorCode"] = fullCode;
            serializeJson(doc, serializedDoc);
            doc.clear();
            auto resp = net.request("document/terminal/getError", serializedDoc);
            if (resp.statusCode == 200)
            {
              isCodeExist = true;
              lcd.clearLine(2);

              std::string textToDisplay = refactorPolishToEnglish(resp.data);

              if (textToDisplay.length() > 20) {
                lcd.print(0, 2, textToDisplay.substr(0, 20));
                delay(300);

                lcd.clearLine(2);
                lcd.clearLine(3);
                lcd.print(0, 2, textToDisplay.substr(0, 20));
              } else {
                lcd.print(0, 2, textToDisplay);
              }

              doc.clear();
            }
            else
            {
              isCodeExist = false;
              lcd.print(0, 2, "KOD NIEZNANY");
            }
          }
        } else if (codeCharacter == '*' && fullCode.size() == 1)
        {
          fullCode.pop_back();
          lcd.print(11, 1, " ");
        } else if (codeCharacter == '*' && fullCode.size() == 2)
        {
          lcd.clearLine(2);
          fullCode.pop_back();
          lcd.print(12, 1, " ");
        } else if (codeCharacter == '#' && fullCode.size() == 2 && isCodeExist) {
          doc["errorCode"] = fullCode;
          break;
        }
      }
    }

    doc["type"] = std::string(1, button.type);
    doc["field"] = button.pole;
    doc["quantity"] = std::stoi(fullQuantity);
    std::string serializedBtn;
    serializeJson(doc, serializedBtn);
    doc.clear();

    requestAnswer = net.request("document/terminal/updateKwit/" + documentId, serializedBtn);
    cons.print("STATUS CODE: " + std::to_string(requestAnswer.statusCode) +
               " DATA: " + requestAnswer.data);
    if (requestAnswer.statusCode == 200) {
      deserializeJson(doc, requestAnswer.data);
      lcd.clear();
      lcd.print(0, 1,
                "Dobrych:   " + std::to_string(doc["quantityNetto"].as<int>()));
      lcd.print(0, 2,
                "Do Sprawdzenia: " + std::to_string(doc["quantityToImprove"].as<int>()));
      lcd.print(0, 3,
                "Braki:     " + std::to_string(doc["quantityLoss"].as<int>()));

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