#include <WiFi.h>
#include "spotprice.h"

const char* ssid = "";
const char* password = "";

void connectWiFi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void printAllPrices() {
  Serial.print("[");

  for (int i = 0; i < 24; i++) {
    Serial.print(prices[i], 2);

    if (i < 23) Serial.print(", ");
  }

  Serial.println("]");
}

void printCurrentAndNext() {
  int h = getCurrentHour();

  char buf[64];

  snprintf(buf, sizeof(buf), "%d–%d: %.2f c/kWh", h, h + 1, prices[h]);

  Serial.println(buf);

  if (h < 23) {
    snprintf(buf, sizeof(buf), "%d–%d: %.2f c/kWh", h + 1, h + 2, prices[h + 1]);

    Serial.println(buf);
  }
}

void setup() {
  Serial.begin(115200);

  connectWiFi();

  spotInit();
  fetchPrices();

  printAllPrices();
}

void loop() {
  fetchPrices();

  printCurrentAndNext();

  delay(60000);
}
