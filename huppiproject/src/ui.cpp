#include "ui.h"
#include "lcd.h"
#include "timeutils.h"
#include "spotprice.h"
#include "weather.h"
#include "network.h"
#include "dht.h"
#include "scd.h"
#include "buttons.h"

static int lastMinute = -1;
static int lastHour = -1;
static String lastDate = "";
static bool lastWifi = false;
static float lastTemp = -999;
static String lastWeatherDesc = "";

void uiBegin() {}

void uiUpdateTime() {
    int h = getCurrentHour();

    char buf[16];
    snprintf(buf, sizeof(buf), "%s", getCurrentTime().c_str());

    lcdDrawText(0, 20, String(buf));
}

void uiUpdateDate() {
    String date = getDisplayDate();

    lcdDrawText(0, 0, date);
}

void uiUpdatePrices() {
    int h = getCurrentHour();
    int nextH = (h + 1) % 24;

    char buf1[32];
    snprintf(buf1, sizeof(buf1), "Now: %.2f c/kWh", prices[h]);
    lcdDrawText(0, 40, String(buf1));

    char buf2[32];
    snprintf(buf2, sizeof(buf2), "Next: %.2f c/kWh", prices[nextH]);
    lcdDrawText(0, 60, String(buf2));
}

void uiUpdateWeather() {
    WeatherData w = getWeather();

    if (!w.updated) {
        lcdDrawText(0, 0, "Weather: no data");
        return;
    }

    char line1[32];
    snprintf(line1, sizeof(line1), "%.1fC (Feels %.1fC)", w.outTemperature, w.feelsLike);

    char line2[32];
    snprintf(line2, sizeof(line2), "Hum: %d%%  Wind: %.1fm/s", w.humidity, w.wind);

    lcdDrawText(0, 0, String(line1));
    lcdDrawText(0, 20, String(w.desc));
    lcdDrawText(0, 40, String(line2));
}

void uiUpdateWifi(bool force) {
    if (getState() != STATE1) return;

    bool wifi = isWifiConnected();

    if (!force && wifi == lastWifi) return;
    lastWifi = wifi;

    lcdDrawText(0, 300, wifi ? "WiFi OK" : "WiFi LOST");
}

void uiUpdateDHT() {
    DHTData d = dhtGet();

    if (!d.valid) {
        lcdDrawText(0, 0, "T: --.- C");
        lcdDrawText(0, 20, "H: --.- %");
        return;
    }

    lcdDrawText(0, 0, "T: " + String(d.temperature, 1) + " C");
    lcdDrawText(0, 20, "H: " + String(d.humidity, 1) + " %");
}

void uiUpdateSCD() {
    SCDData s = scdGet();

    if (!s.valid) {
        lcdDrawText(0, 40, "CO2: --- ppm");
        lcdDrawText(0, 60, "T2: --.- C");
        lcdDrawText(0, 80, "H2: --.- %");
        return;
    }

    lcdDrawText(0, 40, "CO2: " + String(s.co2) + " ppm");
    lcdDrawText(0, 60, "T2: " + String(s.temperature, 1) + " C");
    lcdDrawText(0, 80, "H2: " + String(s.humidity, 1) + " %");
}

void uiRender(DisplayState state) {
    switch (state) {

        case STATE1: {
            uiUpdateDate();
            uiUpdateTime();
            uiUpdatePrices();
            break;
        }

        case STATE2: {
            uiUpdateWeather();
            break;
        }

        case STATE3: {
            uiUpdateDHT();
            uiUpdateSCD();
            break;
        }
    }
}
