#pragma once

class IError {
 public:
  virtual void error(int errorType) = 0;
  virtual ~IError() {}
};