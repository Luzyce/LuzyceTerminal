#include "networking.hpp"

void Networking::init() {
  res = wm.autoConnect();

  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
  } else {
    Serial.println("connected...yeey :)");
  }
}

requestAnswer Networking::request(std::string subpage, std::string data) {
  client->setInsecure();
  if (https.begin(*client, "https://phsapp.getin.ovh/api/" +
                               String(subpage.c_str()))) {
    https.addHeader("Content-Type", "application/json");
    if (subpage == "login") {
      cookie = "";
    } else {
      https.addHeader("Cookie", String(cookie.c_str()));
    }
    Serial.println(data.c_str());
    int httpCode = https.POST(String(data.c_str()));
    if (httpCode > 0) {
      String payload = https.getString();

      if (subpage == "login") {
        JsonDocument doc;
        deserializeJson(doc, payload);
        std::string cookie = doc["token"];
      }

      requestAnswer answer;

      answer.statusCode = httpCode;
      answer.data = payload.c_str();

      return answer;
    }
  }
  return {};
}