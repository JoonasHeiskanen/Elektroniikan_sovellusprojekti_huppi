#include "ui.h"
#include "lcd.h"
#include "timeutils.h"
#include "spotprice.h"
#include "weather.h"
#include "network.h"
#include "dht.h"
#include "scd.h"

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

    //lcd_drawText(0, 20, String(buf));
    lcd_updateTime(buf);
}

void uiUpdateDate() {
    String date = getDisplayDate();

    //lcd_drawText(0, 0, date);
    lcd_updateDate(date);
}

void uiUpdatePrices() {
    int h = getCurrentHour();
    int nextH = (h + 1) % 24;

    char buf1[32];
    snprintf(buf1, sizeof(buf1), "Now: %.2f c/kWh", prices[h]);
    //lcd_drawText(0, 40, String(buf1));

    char buf2[32];
    snprintf(buf2, sizeof(buf2), "Next: %.2f c/kWh", prices[nextH]);
    //lcd_drawText(0, 60, String(buf2));
}

void uiUpdateWeather() {
    WeatherData w = getWeather();

    char buf1[32];
    snprintf(buf1, sizeof(buf1), "%.1f", w.out_temperature);
    char buf2[32];
    snprintf(buf2, sizeof(buf2), "Feels like %.1f", w.feelsLike);

    //lcd_drawText(0, 100, String(buf));
    //lcd_drawText(0, 120, String(w.desc));
    lcd_updateWeather(buf1, buf2, w.desc);
}

void uiUpdateWifi() {
    bool wifi = isWifiConnected();

    if (wifi == lastWifi) return;
    lastWifi = wifi;

    //lcd_drawText(0, 80, wifi ? "WiFi OK" : "WiFi LOST");
}

void uiUpdateDHT() {
    DHTData d = dht_get();

    /*if (!d.valid) {
        lcd_drawText(0, 140, "T: --.- C");
        lcd_drawText(0, 160, "H: --.- %");
        return;
    }

    lcd_drawText(0, 140, "T: " + String(d.temperature, 1) + " C");
    lcd_drawText(0, 160, "H: " + String(d.humidity, 1) + " %");*/
}

void uiUpdateSCD() {
    SCDData s = scd_get();

    /*if (!s.valid) {
        lcd_drawText(0, 180, "CO2: --- ppm");
        lcd_drawText(0, 200, "T2: --.- C");
        lcd_drawText(0, 220, "H2: --.- %");
        return;
    }

    lcd_drawText(0, 180, "CO2: " + String(s.co2) + " ppm");
    lcd_drawText(0, 200, "T2: " + String(s.temperature, 1) + " C");
    lcd_drawText(0, 220, "H2: " + String(s.humidity, 1) + " %");*/
}

void ui_menu() {
  lcd_drawLine(0, 100, 240, 100);
  lcd_drawLine(0, 210, 240, 210);
  lcd_drawLine(0, 101, 240, 101);
  lcd_drawLine(0, 211, 240, 211);

  uiUpdateDate();
  uiUpdateTime();
  uiUpdateWeather();
}

void ui_24h_spotPrice() {
  lcd_drawLine(0, 100, 240, 100);
  lcd_drawLine(0, 210, 240, 210);
  lcd_drawText(60, 160, "24H SPOT PRICE HERE!");
}
