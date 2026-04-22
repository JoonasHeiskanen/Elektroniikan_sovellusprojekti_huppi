#include "spotprice.h"
#include "timeutils.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

float prices[24];
float nextPrices[24];

static String lastFetchedDate = "";

void fetchPrices() {
  String date = getApiDate();

  if (date == lastFetchedDate) return;
  lastFetchedDate = date;

  String url = "https://www.sahkonhintatanaan.fi/api/v1/prices/" + date + ".json";

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.begin(client, url);

  if (http.GET() <= 0) {
    http.end();
    return;
  }

  String payload = http.getString();

  DynamicJsonDocument doc(4096);
  deserializeJson(doc, payload);

  JsonArray arr = doc.as<JsonArray>();

  for (int i = 0; i < 24; i++) {
    float eur = arr[i]["EUR_per_kWh"];
    prices[i] = eur * 125.5;
  }

  http.end();
}

float getPrice(int hour) {
  return prices[hour];
}

float getCurrentPrice() {
  return prices[getCurrentHour()];
}

float getNextPrice() {
  int h = getCurrentHour();
  if (h >= 23) return 0;
  return prices[h + 1];
}
