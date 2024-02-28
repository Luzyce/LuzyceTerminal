#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "networking/INetworking.hpp"
#include "scanner/IScanner.hpp"

class Modules {
  INetworking& net;
  IScanner& scan;

 public:
  Modules(INetworking& net, IScanner& scan);
  void init();
};
