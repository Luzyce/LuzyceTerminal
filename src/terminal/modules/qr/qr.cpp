#include "qr.hpp"

void Qr::init() {
  Serial2.begin(9600, SERIAL_8N1, 33, 32);
  delay(250);
  Serial2.write("RAB160");
  delay(250);
  Serial2.write("RDM020");
  delay(250);
  Serial2.write("RDC010");
  delay(1000);
  Serial2.write("RDC000");
  delay(250);
}

std::string Qr::scan() {
  fullScan = "";
  Serial2.write("RDC010");
  int64_t previousMillis = 0;
  while (true) {
    int64_t currentMillis = (esp_timer_get_time() / 1000ULL);
    if (currentMillis - previousMillis >= 10000) {
      Serial2.write("RDC000");
      delay(500);
      Serial2.write("RDC010");
      previousMillis = currentMillis;
    }

    if (Serial2.available()) {
      while (Serial2.available()) {
        char input = Serial2.read();
        if (input != 13 && (isDigit(input) || input == '/' || isAlpha(input))) {
          fullScan += input;
        } else if (input == 13) {
          Serial2.write("RDC000");
          return fullScan;
        }
      }
    }
  }
}