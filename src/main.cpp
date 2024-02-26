#include <Arduino.h>

#include "modules/modules.hpp"

Modules modules;

void setup() {
  Serial.begin(115200);
  modules.init();
}

void loop() {}