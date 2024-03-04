#pragma once

#include "interfaces/IError.hpp"
#include "interfaces/IKeypad.hpp"
#include "interfaces/ILcd.hpp"
#include "interfaces/IMcp.hpp"
#include "interfaces/INetworking.hpp"
#include "interfaces/INfc.hpp"
#include "interfaces/IQr.hpp"
#include "interfaces/IScanner.hpp"

class Terminal {
  INetworking& net;
  IScanner& scan;
  IKeypad& key;
  IMcp& mcp;
  ILcd& lcd;
  INfc& nfc;
  IQr& qr;
  IError& err;

 public:
  Terminal(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd,
          INfc& nfc, IQr& qr, IError& err);
  void init();
};
