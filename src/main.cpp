#include <Arduino.h>
#include "modules/networking/networking.hpp"

Networking net;

void setup() {
  Serial.begin(115200);
  net.init();
}

void loop() {
}