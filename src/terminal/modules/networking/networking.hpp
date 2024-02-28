#pragma once

#include <WiFiManager.h>

#include "../../interfaces/INetworking.hpp"

class Networking : public INetworking {
  WiFiManager wm;
  bool res;

 public:
  void init() override;
};