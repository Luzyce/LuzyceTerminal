#pragma once

#include <WiFiManager.h>

class Networking {
  WiFiManager wm;
  bool res;

public:
  void init();
};