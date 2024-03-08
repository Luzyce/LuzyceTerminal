#pragma once

#include "interfaces/IConsole.hpp"
#include "interfaces/IKeypad.hpp"
#include "interfaces/ILcd.hpp"
#include "interfaces/IMcp.hpp"
#include "interfaces/INetworking.hpp"
#include "interfaces/INfc.hpp"
#include "interfaces/IQr.hpp"
#include "interfaces/IScanner.hpp"
#include <Wire.h>

class Terminal {
  INetworking& net;
  IScanner& scan;
  IKeypad& key;
  IMcp& mcp;
  ILcd& lcd;
  INfc& nfc;
  IQr& qr;
  IConsole& cons;
  TwoWire& wire;

 public:
  Terminal(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd,
          INfc& nfc, IQr& qr, IConsole& cons, TwoWire& wire);
  void init();
};
