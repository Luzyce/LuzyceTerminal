#include "networking.hpp"

void Networking::configModeCallback(WiFiManager *wm)
{
  if (mcp == nullptr || lcd == nullptr) {
      Serial.println("One or more pointers are null!");
      return;
  }

  mcp->statusLed(LEDR);
  lcd->print(0, 0, "Konf terminala");
  lcd->print(0, 1, "WiFi: ");
  lcd->print(0, 2, std::string(wm->getConfigPortalSSID().c_str()));
  lcd->print(0, 3, "IP: 192.168.4.1");
}

std::string Networking::getIp() {
    return WiFi.localIP().toString().c_str();
}

void Networking::init(ILcd* lcd, IMcp* mcp) {
  this->lcd = lcd;
  this->mcp = mcp;

  wm.setAPCallback(bind(&Networking::configModeCallback, this, std::placeholders::_1));
  res = wm.autoConnect();

  if (!res) {
    Serial.println("Failed to connect");
    ESP.restart();
  } else {
    Serial.println("connected...yeey :)");
    // lcd: Serial.println(WiFi.localIP());
  }
}

void Networking::initOTA()
{
    Serial.println("OTA starting");
    ArduinoOTA.setPort(8081);
    ArduinoOTA.onStart([]()
                       { Serial.println("Start updating"); });
    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
    ArduinoOTA.onError([](ota_error_t error)
                       {
                           Serial.printf("Error[%u]: ", error);
                           if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                           else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                           else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                           else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                           else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
    ArduinoOTA.begin();
}

requestAnswer Networking::request(std::string subpage, std::string data) {
  client->setInsecure();
  auto url = "https://app-phs.cloudsolutions.ovh/api/" + String(subpage.c_str());

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
    } else if (subpage.find("updateKwit") != std::string::npos) {
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
