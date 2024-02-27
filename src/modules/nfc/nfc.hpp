#pragma once

#include <rdm6300.h>

class Nfc {
  Rdm6300 rdm6300;

 public:
  void init();
};