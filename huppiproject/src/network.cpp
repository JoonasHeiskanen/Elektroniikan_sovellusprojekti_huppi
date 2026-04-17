#include "network.h"
#include "secrets.h"

static uint8_t WIFI_LED_PIN = 38;

static unsigned long lastReconnectAttempt = 0;
static const unsigned long reconnectInterval = 10000;

static bool wifiWasConnected = false;

void networkBegin() {
    pinMode(WIFI_LED_PIN, OUTPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(WIFI_LED_PIN, HIGH);
        wifiWasConnected = true;
    } else {
        digitalWrite(WIFI_LED_PIN, LOW);
        wifiWasConnected = false;
    }
}

bool isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void networkReconnect() {
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void networkUpdate() {
    bool connected = isWifiConnected();

    digitalWrite(WIFI_LED_PIN, connected ? HIGH : LOW);

    if (!connected && wifiWasConnected) {
        wifiWasConnected = false;
    }

    if (!connected && millis() - lastReconnectAttempt >= reconnectInterval) {
        lastReconnectAttempt = millis();
        networkReconnect();
    }

    if (connected && !wifiWasConnected) {
        wifiWasConnected = true;
    }
}
