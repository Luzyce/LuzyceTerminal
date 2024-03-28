#include "console.hpp"

void Console::print(std::string data) { Serial.println(data.c_str()); }