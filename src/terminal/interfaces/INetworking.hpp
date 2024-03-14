#pragma once

#include <string>

struct requestAnswer {
  int statusCode;
  std::string data;
};

class INetworking {
 public:
  virtual void init() = 0;
  virtual requestAnswer request(std::string subpage, std::string data) = 0;
  virtual ~INetworking() {}
};