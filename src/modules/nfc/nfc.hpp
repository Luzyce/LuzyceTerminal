#pragma once

#include <rdm6300.h>

#include "iNfc.hpp"

class Nfc : public INfc {
  Rdm6300 rdm6300;

 public:
  void init() override;
};