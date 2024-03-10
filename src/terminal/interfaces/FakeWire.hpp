#pragma once

class FakeWire {
 public:
  bool begin() {
    return true;
  };
  ~FakeWire() {}
};

// extern FakeWire Wire;