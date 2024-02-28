#include "nfc.hpp"

void Nfc::init() { rdm6300.begin(26); }