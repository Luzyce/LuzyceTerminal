#pragma once

#include <ArduinoJson.h>

#include "interfaces/IConsole.hpp"
#include "interfaces/IKeypad.hpp"
#include "interfaces/ILcd.hpp"
#include "interfaces/IMcp.hpp"
#include "interfaces/INetworking.hpp"
#include "interfaces/INfc.hpp"
#include "interfaces/IQr.hpp"
#include "interfaces/IScanner.hpp"

#if defined(esp32)
#include <Wire.h>
#elif defined(utest)
#include "interfaces/FakeArduino.hpp"
#include "interfaces/FakeWire.hpp"
FakeWire Wire;
#endif

class Terminal {
  INetworking& net;
  IScanner& scan;
  IKeypad& key;
  IMcp& mcp;
  ILcd& lcd;
  INfc& nfc;
  IQr& qr;
  IConsole& cons;
  JsonDocument doc;

  void printDocumentInfo();

 public:
  Terminal(INetworking& net, IScanner& scan, IKeypad& key, IMcp& mcp, ILcd& lcd,
           INfc& nfc, IQr& qr, IConsole& cons);
  void init();
  void process();
};
