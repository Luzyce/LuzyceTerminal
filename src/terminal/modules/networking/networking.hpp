#pragma once

#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

#include "../../interfaces/INetworking.hpp"

class Networking : public INetworking {
  WiFiClientSecure *client = new WiFiClientSecure;
  HTTPClient https;
  WiFiManager wm;
  bool res;
  std::string cookie;

 public:
  void init() override;
  requestAnswer request(std::string subpage, std::string data) override;
};