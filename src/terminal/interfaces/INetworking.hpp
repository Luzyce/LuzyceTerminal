#pragma once

#include <string>
#include "ILcd.hpp"
#include "IMcp.hpp"

struct requestAnswer {
  int statusCode;
  std::string data;
};

class INetworking {
 public:
  virtual std::string getIp() = 0;
  virtual void init(ILcd* lcd, IMcp* mcp) = 0;
  virtual void initOTA() = 0;
  virtual requestAnswer request(std::string subpage, std::string data) = 0;
  virtual ~INetworking() {}
};