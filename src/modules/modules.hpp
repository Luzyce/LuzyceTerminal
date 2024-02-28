#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "keypad/IKeypad.hpp"
#include "lcd/ILcd.hpp"
#include "mcp/IMcp.hpp"
#include "networking/INetworking.hpp"
#include "nfc/INfc.hpp"
#include "qr/IQr.hpp"
#include "scanner/IScanner.hpp"

class Modules {
  INetworking& net;
  IScanner& scan;
  IKeypad& key;
  IMcp& mcp;
  ILcd& lcd;
  INfc& nfc;
  IQr& qr;

 public:
  Modules(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd,
          INfc& nfc, IQr& qr);
  void init();
};
