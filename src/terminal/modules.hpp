#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "networking/INetworking.hpp"
#include "scanner/IScanner.hpp"
#include "error/IError.hpp"

class Modules {
  INetworking& net;
  IScanner& scan;
  IError& err;

 public:
  Modules(INetworking& net, IScanner& scan, IError& err);
  void init();
};
