#include "spotprice.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <time.h>

float prices[24];
static String lastFetchedDate = "";

int getCurrentHour() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);
  return t->tm_hour;
}

String getCurrentDate() {
  time_t now = time(nullptr);
  struct tm *t = localtime(&now);

  char buf[20];
  sprintf(buf, "%04d/%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

  return String(buf);
}

void spotInit() {
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1);
  tzset();

  while (time(nullptr) < 100000) {
    delay(500);
  }
}

void fetchPrices() {
  String date = getCurrentDate();

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
