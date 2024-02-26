#pragma once

#include "networking/networking.hpp"
#include "keypad/keypad.hpp"

class Modules {
  Networking net;
  MatrixKeypad key;

 public:
  void init();
};