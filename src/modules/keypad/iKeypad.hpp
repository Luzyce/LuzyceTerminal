#pragma once

class IKeypad {
 public:
  virtual void init() = 0;
  virtual ~IKeypad() {}
};