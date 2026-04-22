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
#include "buttons.h"

static DisplayState lastScreen = STATE1;
static DisplayState currentScreen;
static bool forceScreenRefresh = true;

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

    lcdSetup();
    dhtSetup();
    scdSetup();
    buttonSetup();

    networkBegin();
    timeInit();

    fetchPrices();

    weatherBegin();
    weatherUpdate();

    uiBegin();
}

void loop() {
    networkUpdate();
    dhtUpdate();
    scdUpdate();

    checkButton();

    currentScreen = getState();

    if (currentScreen != lastScreen) {
        lastScreen = currentScreen;
        lcdClear();
        forceScreenRefresh = true;
    }

    time_t now = time(nullptr);
    struct tm *t = localtime(&now);

    if (currentScreen == STATE1) {

       if (forceScreenRefresh) {
            forceScreenRefresh = false;
            uiUpdateDate();
            uiUpdateTime();
            uiUpdateDHT();
            uiUpdateSCD();
            uiUpdateWeather();
            uiUpdateWifi(true);
        }

        if (t->tm_min != lastMinute) {
            lastMinute = t->tm_min;
            uiUpdateTime();
        }

        String date = getDisplayDate();
        if (date != lastDate) {
            lastDate = date;
            uiUpdateDate();
        }

        if (millis() - lastSensorUpdate >= sensorInterval) {
            lastSensorUpdate = millis();
            uiUpdateDHT();
            uiUpdateSCD();
        }

        if (millis() - lastWeatherUpdate >= weatherInterval) {
            lastWeatherUpdate = millis();

            if (isWifiConnected() && weatherUpdate()) {
                uiUpdateWeather();
            }
        }
    }

    else if (currentScreen == STATE2) {

        if (forceScreenRefresh) {
            forceScreenRefresh = false;
            uiUpdatePrices();
            uiSpotGraph();
        }

        if (t->tm_hour != lastHour) {
            lastHour = t->tm_hour;
            uiUpdatePrices();
            uiSpotGraph();
        }

        String today = getApiDate();
        if (today != lastApiDate) {
            lastApiDate = today;
            if (isWifiConnected()) {
                fetchPrices();
            }
        }
    }

    delay(100);
}
