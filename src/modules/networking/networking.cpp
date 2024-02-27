#include "networking.hpp"

void Networking::init() {
  res = wm.autoConnect();

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    Serial.println("connected...yeey :)");
  }
}