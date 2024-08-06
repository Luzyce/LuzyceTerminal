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

std::string Networking::getIp() {
  return WiFi.localIP().toString().c_str();
}

requestAnswer Networking::request(std::string subpage, std::string data) {
  client->setInsecure();
  auto url = "https://phsapp.getin.ovh/api/" + String(subpage.c_str());

  Serial.println("Request: " + url);

  if (https.begin(*client, url)) {
    https.addHeader("Content-Type", "application/json");
    if (subpage == "login") {
      cookie = "";
      Serial.println("No cookie");
    } else {
      https.addHeader("Authorization", "Bearer " + String(cookie.c_str()));
      Serial.println("Authorization:" + String(cookie.c_str()));
    }

    Serial.println("Sending data:" + String(data.c_str()));

    int httpCode;
    if (data == "") {
      httpCode = https.GET();
      Serial.println("GET");
    } else if (subpage.find("updateDocumentPositionOnKwit") != std::string::npos) { 
      httpCode = https.PUT(data.c_str());
      Serial.println("PUT");
    } else {
      httpCode = https.POST(String(data.c_str()));
      Serial.println("POST");
    }
    
    if (httpCode > 0) {
      Serial.println("HTTP Status Code: " + String(httpCode));
      String payload = https.getString();

      if (subpage == "login" && httpCode == 200) {
        JsonDocument doc;
        deserializeJson(doc, payload);
        cookie = (const char*)doc["token"];
      }

      requestAnswer answer;

      answer.statusCode = httpCode;
      answer.data = payload.c_str();

      https.end();
      return answer;
    } else {
      https.end();
      Serial.println("Error on HTTP request: " + String(httpCode));
    }
  }
  return {};
}