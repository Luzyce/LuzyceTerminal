#pragma once

#include "networking/networking.hpp"
#include "scanner/scanner.hpp"
#include "keypad/keypad.hpp"

class Modules {
  Networking net;
  Scanner scan;
  MatrixKeypad key;

 public:
  void init();
};