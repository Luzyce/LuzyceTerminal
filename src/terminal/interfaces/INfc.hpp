#pragma once

class INfc {
 public:
  virtual void init() = 0;
  virtual std::string scan() = 0;
  virtual ~INfc() {}
};