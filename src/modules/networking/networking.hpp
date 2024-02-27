#pragma once

#include <WiFiManager.h>

#include "iNetworking.hpp"

class Networking : public INetworking {
  WiFiManager wm;
  bool res;

 public:
  void init() override;
};