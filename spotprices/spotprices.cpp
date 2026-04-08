#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "-";
const char* password = "-";

const char* apiUrl = "https://www.sahkonhintatanaan.fi/api/v1/prices/2026/04-08.json";

float hourlyPrices[24];

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nwifi connected");

  fetchAndStorePrices();

  for (int i = 0; i < 24; i++) {
    Serial.print("Price hour ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(hourlyPrices[i], 2);
  }
}

void loop() {}

void fetchAndStorePrices() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(apiUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      DynamicJsonDocument doc(8192);
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("json parse failed: ");
        Serial.println(error.c_str());
        return;
      }

      JsonObject obj = doc.as<JsonObject>();

      int index = 0;
      for (JsonPair kv : obj) {
        if (index >= 24) break;
        float eurPerKwh = kv.value()["EUR_per_kWh"].as<float>();

        float price_cents = eurPerKwh * 100.0 * 1.255;
        hourlyPrices[index] = round(price_cents * 100.0) / 100.0;
        index++;
      }

    } else {
      Serial.print("http get failed: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("wifi not connected");
  }
}
