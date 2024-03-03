#include "error.hpp"

void Error::error(int errorType) {
    Serial.println(String(errorType));
}