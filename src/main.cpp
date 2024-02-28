#include <Arduino.h>

#include "terminal/networking/networking.hpp"
#include "terminal/scanner/scanner.hpp"
#include "terminal/error/error.hpp"

#include "terminal/modules.hpp"

Networking net;
Scanner scan;
Error error;
Modules modules(net, scan, error);

void setup() {
  Serial.begin(115200);
  modules.init();
}

void loop() {}
