#include "qr.hpp"

void Qr::init() {
  Serial2.begin(9600, SERIAL_8N1, 33, 32);
  delay(250);
  Serial2.write("RAB000");
  delay(250);
  Serial2.write("RDM020;"); 
  delay(250);
  Serial2.write("RDC010"); 
  delay(1000);
  Serial2.write("RDC000"); 
  delay(250);
}

std::string Qr::scan() {
  fullScan = "";
  Serial2.write("RDC010");
  while (true) {
    if (Serial2.available())
    {
      while (Serial2.available())
      {
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