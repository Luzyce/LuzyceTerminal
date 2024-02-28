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
