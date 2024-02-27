#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "keypad/iKeypad.hpp"
#include "lcd/iLcd.hpp"
#include "mcp/iMcp.hpp"
#include "networking/iNetworking.hpp"
#include "nfc/iNfc.hpp"
#include "qr/iQr.hpp"
#include "scanner/iScanner.hpp"

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
