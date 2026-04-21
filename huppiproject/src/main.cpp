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
    checkButton();

    static DisplayState previousState = (DisplayState)-1; // Alustetaan "olemattomaan" tilaan

    // Suoritetaan piirtäminen VAIN jos tila on vaihtunut
    if (currentState != previousState) {
        
        // Tyhjennetään näyttö aina kun tila vaihtuu, jotta vanhat piirrokset poistuvat
        lcd_clear(); 

        if (currentState == STATE1) {
            ui_menu();
        }
        else if (currentState == STATE2) {
            ui_24h_spotPrice();
        }
        else if (currentState == STATE3) {
            lcd_drawText(10, 10, "STATE3");
        }

        // Päivitetään muisti, jotta ensi kierroksella ei piirretä uudestaan
        previousState = currentState;
    }


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
