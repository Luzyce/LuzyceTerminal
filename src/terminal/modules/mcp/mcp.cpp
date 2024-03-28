#include "mcp.hpp"

void Mcp::init() {
  if (!mcp.begin_I2C(0x24)) ESP.restart();

  for (uint8_t i = 0; i < 7; i++) mcp.pinMode(i, INPUT_PULLUP);
  mcp.pinMode(LEDR, OUTPUT);
  mcp.pinMode(LEDG, OUTPUT);
  mcp.pinMode(LEDB, OUTPUT);
}

void Mcp::statusLed(int led) {
  mcp.digitalWrite(LEDR, (led == LEDR) ? HIGH : LOW);
  mcp.digitalWrite(LEDG, (led == LEDG) ? HIGH : LOW);
  mcp.digitalWrite(LEDB, (led == LEDB) ? HIGH : LOW);
}

readBtnAnswer Mcp::readBtn() {
  int64_t timeForTimeout = (esp_timer_get_time() / 1000ULL) + 30000;
  while (true) {
    int64_t currentMillis = (esp_timer_get_time() / 1000ULL);
    if (currentMillis >= timeForTimeout) {
      Serial2.write("RDC000");

      answer.status = 1;
      answer.pole = "";
      answer.type = '/';

      return answer;
    }
    for (uint8_t btn = 0; btn < 7; btn++) {
      if (!mcp.digitalRead(btn)) {
        while (!mcp.digitalRead(btn))
          delay(30);
        switch (btn) {
          case 0:
            answer.type = '-';
            answer.pole = "Zlych";
            break;
          case 1:
            answer.type = '+';
            answer.pole = "Zlych";
            break;
          case 2:
            answer.type = '-';
            answer.pole = "DoPoprawy";
            break;
          case 3:
            answer.type = '+';
            answer.pole = "DoPoprawy";
            break;
          case 4:
            answer.type = '-';
            answer.pole = "Dobrych";
            break;
          case 5:
            answer.type = '+';
            answer.pole = "Dobrych";
            break;
          case 6:
            answer.pole = "KoniecKwita";
            break;
          deafult:
            break;
        }
        return answer;
      }
    }
  }
}

void Mcp::resetBtn() {
  while (1) {
    delay(500);
    if (!mcp.digitalRead(6) && !mcp.digitalRead(7)) return;
  }
}