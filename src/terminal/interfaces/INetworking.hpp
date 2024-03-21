#pragma once

#include <string>

struct RequestAnswer {
  int statusCode;
  std::string data;
};

class INetworking {
 public:
  virtual void init() = 0;
  virtual RequestAnswer request(std::string subpage, std::string data) = 0;
  virtual ~INetworking() {}
};