#include "nfc.hpp"

void Nfc::init() { rdm6300.begin(26); }

std::string Nfc::scan() {
  while (!rdm6300.get_new_tag_id()) {}

  String nfcTag = String(rdm6300.get_tag_id(), HEX);
  nfcTag.toUpperCase();
  return nfcTag.c_str();
}