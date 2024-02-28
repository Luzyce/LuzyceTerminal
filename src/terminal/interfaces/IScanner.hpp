#pragma once

class IScanner {
 public:
  virtual int scan() = 0;
  virtual ~IScanner() {}
};