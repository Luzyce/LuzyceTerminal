#pragma once

#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

#include "../../interfaces/INetworking.hpp"
#include "terminal/interfaces/ILcd.hpp"
#include "terminal/interfaces/IMcp.hpp"

class Networking : public INetworking {
  WiFiClientSecure *client = new WiFiClientSecure;
  HTTPClient https;
  WiFiManager wm;
  bool res;
  std::string cookie;
  void configModeCallback(WiFiManager *wm);

  ILcd* lcd = nullptr;
  IMcp* mcp = nullptr;

 public:
  std::string getIp() override;
  void init(ILcd* lcd, IMcp* mcp) override;
  void initOTA() override;
  requestAnswer request(std::string subpage, std::string data) override;
};