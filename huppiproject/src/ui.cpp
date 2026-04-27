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

//Draw STATE1 lines
void uiLines(int s = 1) {
    if (s == 1) {
        lcdDrawLine(0, 34, 240, 34);
        lcdDrawLine(0, 35, 240, 35);
        lcdDrawLine(0, 90, 240, 90);
        lcdDrawLine(0, 91, 240, 91);
        lcdDrawLine(0, 195, 240, 195);
        lcdDrawLine(0, 196, 240, 196);
    } else {
        lcdDrawLine(0, 120, 240, 120);
    }
}

void uiIN_OUT() {
    lcdDrawIN_OUT();
}

void uiUpdateTime() {
    char buf[16];
    snprintf(buf, sizeof(buf), "%s", getCurrentTime().c_str());

    //lcdDrawText(200, 0, String(buf), 40, 20);
    lcdDrawTime(buf);
}

void uiUpdateDate() {
    String date = getDisplayDate();

    //lcdDrawText(0, 0, date, 100, 20);
    lcdDrawDate(date);
    //lcdDrawText(5, 20, date, 40, 20);
    
    //lcdSetTextSize(1);
    //lcdDrawText(5, 40, date, 40, 20);
    

}

void uiUpdatePrices() {
    int h = getCurrentHour();
    int nextH = (h + 1) % 24;

    char buf1[32];
    snprintf(buf1, sizeof(buf1), "Now: %.2f c/kWh", prices[h]);
    lcdDrawText(0, 0, String(buf1), 240, 20);

    if (nextH == 0) {
        lcdDrawText(0, 20, "Next: -.-- c/kWh", 240, 20);
    } else {
        char buf2[32];
        snprintf(buf2, sizeof(buf2), "Next: %.2f c/kWh", prices[nextH]);
        lcdDrawText(0, 20, String(buf2), 240, 20);
    }
}

void uiUpdateCurrentPrice() {
    int h = getCurrentHour();
    int nextH = (h + 1) % 24;

    char buf[32];
    snprintf(buf, sizeof(buf), "%.2f", prices[h]);
    //lcdDrawText(5, 45, String(buf), 240, 20);
    lcdDrawCurrentPrice(buf);
}

void uiUpdateWeather() {
    WeatherData w = getWeather();

    if (!w.updated) {
        lcdDrawText(0, 120, "Weather: no data", 240, 20);
        return;
    }

    //for icons testing!
    //strncpy(w.icon, "09d", sizeof(w.icon));
    
    lcdDrawWeatherIcon(w);

    char buf1[32];
    snprintf(buf1, sizeof(buf1), "%.1f ", w.outTemperature);

    char buf2[32];
    snprintf(buf2, sizeof(buf2), "Feels: %.1fC  ", w.feelsLike);

    char buf3[32];
    snprintf(buf3, sizeof(buf3), "Hum: %d%%  ", w.humidity);

    char buf4[32];
    snprintf(buf4, sizeof(buf4), "Wind: %.1fm/s ", w.wind);

    //lcdDrawText(0, 120, String(buf1), 240, 20);
    //lcdDrawText(175, 165, String(w.desc), 240, 20);
    //lcdDrawText(0, 160, String(buf2), 240, 20);

    lcdDrawWeather(buf1, buf2, buf3, buf4);
}

void uiUpdateWifi(bool force) {
    if (getState() != STATE1) return;

    bool wifi = isWifiConnected();

    if (!force && wifi == lastWifi) return;
    lastWifi = wifi;

    //lcdDrawText(125, 5, wifi ? "WiFi OK" : "WiFi LOST", 240, 20);
}

void uiUpdateDHT() {
    DHTData d = dhtGet();

    /*if (!d.valid) {
        lcdDrawText(0, 40, "T: --.- C", 240, 20);
        lcdDrawText(0, 60, "H: --.- %", 240, 20);
        return;
    }

    lcdDrawText(0, 40, "T: " + String(d.temperature, 1) + " C", 240, 20);
    lcdDrawText(0, 60, "H: " + String(d.humidity, 1) + " %", 240, 20);*/

    lcdDrawDHT(d.temperature, d.humidity);
}

void uiUpdateSCD() {
    SCDData s = scdGet();

    if (!s.valid) {
        //lcdDrawText(0, 140, "CO2: --- ppm", 240, 20);
        lcdDrawSCD(s.co2);
        return;
    }
    //lcdDrawText(0, 140, "CO2: " + String(s.co2) + " ppm", 240, 20);
    lcdDrawSCD(s.co2);
}

void uiSpotGraph() {
    lcdDrawSpotGraph(prices);
    lcdDrawSpotHours();
}

void uiRender(DisplayState state) {
    switch (state) {

        case STATE1: {
            uiUpdateDate();
            uiUpdateTime();
            uiUpdateCurrentPrice();
            uiUpdateDHT();
            uiUpdateSCD();
            uiUpdateWeather();
            break;
        }

        case STATE2: {
            uiUpdatePrices();
            lcdDrawSpotHours();
            break;
        }
    }
}
