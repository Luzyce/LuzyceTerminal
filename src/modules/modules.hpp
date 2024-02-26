#pragma once

#include "networking/networking.hpp"
#include "scanner/scanner.hpp"
#include "keypad/keypad.hpp"
#include "mcp/mcp.hpp"

class Modules {
  Networking net;
  Scanner scan;
  MatrixKeypad key;
  Mcp mcp;

 public:
  void init();
};