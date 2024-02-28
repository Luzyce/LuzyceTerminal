#pragma once

class INetworking {
 public:
  virtual void init() = 0;
  virtual ~INetworking() {}
};