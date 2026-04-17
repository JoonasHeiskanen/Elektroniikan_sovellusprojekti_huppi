#include <Arduino.h>

#include "secrets.h"
#include "timeutils.h"
#include "spotprice.h"
#include "lcd.h"
#include "network.h"
#include "weather.h"
#include "ui.h"
#include "dht.h"
#include "scd.h"

static int lastMinute = -1;
static int lastHour = -1;
static String lastDate = "";
static String lastApiDate = "";
static unsigned long lastWeatherUpdate = 0;
const unsigned long weatherInterval = 300000;

static unsigned long lastSensorUpdate = 0;
const unsigned long sensorInterval = 1000;

void setup() {
    Serial.begin(115200);
    delay(1000);

    lcd_setup();
    dht_setup();
    scd_setup();

    networkBegin();
    timeInit();

    fetchPrices();

    weatherBegin();
    weatherUpdate();

    uiBegin();
    uiUpdateDate();
    uiUpdateTime();
    uiUpdatePrices();
    uiUpdateWeather();
    uiUpdateWifi();
    uiUpdateDHT();
    uiUpdateSCD();
}

void loop() {
    networkUpdate();
    dht_update();
    scd_update();

    time_t now = time(nullptr);
    struct tm *t = localtime(&now);

    if (t->tm_min != lastMinute) {
        lastMinute = t->tm_min;
        uiUpdateTime();
    }

    if (t->tm_hour != lastHour) {
        lastHour = t->tm_hour;
        uiUpdatePrices();
    }

    String date = getDisplayDate();
    if (date != lastDate) {
        lastDate = date;
        uiUpdateDate();
    }

    String today = getApiDate();
    if (today != lastApiDate) {
        lastApiDate = today;
        fetchPrices();
        uiUpdatePrices();
    }

    uiUpdateWifi();

    if (millis() - lastWeatherUpdate >= weatherInterval) {
        lastWeatherUpdate = millis();

        if (isWifiConnected() && weatherUpdate()) {
            uiUpdateWeather();
        }
    }

    if (millis() - lastSensorUpdate >= sensorInterval) {
        lastSensorUpdate = millis();
        uiUpdateDHT();
        uiUpdateSCD();
    }

    delay(100);
}
