#include "qr.hpp"

void Qr::init() {
  Serial2.begin(9600, SERIAL_8N1, 33, 32);
  delay(100);
  Serial2.write("RAB000");
  delay(100);
  Serial2.write("RDM020");
  delay(100);
  Serial2.write("RDC010");
  delay(100);
  Serial2.write("RDC000");
}
