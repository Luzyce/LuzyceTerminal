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
    const char *headerKeys[] = {"date", "set-cookie"};
    client->setInsecure();
    if (https.begin(*client, "https://daniel-phs.wroc.ovh" + String(subpage.c_str())))
    {
        https.addHeader("Content-Type", "application/json");

        int httpCode = https.POST(String(data.c_str()));
        if (httpCode > 0)
        {
            String payload = https.getString();

            requestAnswer answer;

            answer.statusCode = httpCode;
            answer.data = payload.c_str();

            return answer;
        }
    }
    return {};
}