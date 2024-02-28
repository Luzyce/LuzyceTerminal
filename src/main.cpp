#include <Arduino.h>

#include "modules/networking/networking.hpp"
#include "modules/scanner/scanner.hpp"

#include "modules/modules.hpp"

Networking net;
Scanner scan;
Modules modules(net, scan);

void setup() {
  Serial.begin(115200);
  modules.init();
}

void loop() {}
