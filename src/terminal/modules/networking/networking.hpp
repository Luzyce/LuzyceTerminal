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
  HTTPClient http;
  WiFiManager wm;
  bool res = false;
  std::string cookie;
  std::string ipAddr;
  void configModeCallback(WiFiManager *wm);

  ILcd* lcd = nullptr;
  IMcp* mcp = nullptr;

 public:
  explicit Networking(std::string ipAddr);
  std::string getIp() override;
  void init(ILcd* lcd, IMcp* mcp) override;
  void initOTA() override;
  requestAnswer request(std::string subpage, std::string data) override;
};