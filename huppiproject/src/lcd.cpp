#include <Arduino.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include "weather.h"
#include "spotprice.h"
#include "lcd.h"
#include "timeutils.h"

String mainFont = "calibri24";
String mainFontLight = "calibril24";
String mainFont2 = "calibri16";
String tempFont = "calibri36";
//String tempFont = "DSEG7Modern-Bold36";
//String tempFont = "DSEG7MR-36";
String spotPriceFont = "calibri10";

//Align edge pixels where drawfunctions start to draw
int leftAlign_x = 0;
int topAlign_y = 5;

TFT_eSPI tft = TFT_eSPI();

void lcdSetup() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        while (true) delay(1000);
    }

    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
}

void lcdClear() {
    tft.fillScreen(TFT_BLACK);
}

void lcdDrawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye) {
    tft.drawLine(xs, ys, xe, ye, TFT_LIGHTGREY);
}

// voi poistaa
void lcdDrawText(int x, int y, String text, int width = 240, int height = 20) {
    tft.fillRect(x, y, width, height, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(text, x, y, 1);
}

// voi poistaa
void lcdSetTextSize(uint8_t size) {
    tft.setTextSize(size);
}

void lcdDrawDate(String d){
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(d + " ", leftAlign_x, 5);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
}

void lcdDrawTime(String t) {
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(t, 180, topAlign_y);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
}

void lcdDrawWeatherIcon(WeatherData w) {
    uint16_t iconCP;
    uint16_t iconColor = TFT_WHITE;
    String code = String(w.icon);
    code.trim();

    if (code == "01d") {
        iconCP = WI_DAY_SUNNY;
        iconColor = TFT_ORANGE;
    }
    else if (code == "01n") {
        iconCP = WI_NIGHT_MOON;
        iconColor = TFT_YELLOW;
    }
    else if (code == "02n") {
        iconCP = WI_NIGHT_ALT_CLOUDY; 
        iconColor = TFT_WHITE;
    }
    else if (code == "02d") {
        iconCP = WI_DAY_SUNNY_OVERCAST;
        iconColor = TFT_WHITE;
    }
    else if (code == "09d" || code == "09n") {
        iconCP = WI_RAIN; // Rankkasade / Kuurot
        iconColor = TFT_BLUE;
    }
    else if (code == "10d") {
        iconCP = WI_DAY_RAIN; // Aurinkoa ja sadetta (Päivä)
        iconColor = TFT_WHITE; // Kirkkaampi sininen
    }
    else if (code == "10n") {
        iconCP = WI_RAIN; // Yöllä käytetään tavallista sadetta
        iconColor = TFT_WHITE;
    }
    else if (code == "11d" || code == "11n") {
        iconCP = WI_THUNDERSTORM;
        iconColor = TFT_PURPLE;
    }
    else if (code == "13d" || code == "13n") {
        iconCP = WI_SNOW_WIND;
        iconColor = TFT_CYAN;
    }
    else if (code == "50d" || code == "50n") {
        iconCP = WI_FOG;
        iconColor = TFT_WHITE;
    }
    else {
        // Koodit 03d/n ja 04d/n (Pilvistä)
        iconCP = WI_CLOUDY;
        iconColor = TFT_WHITE;
    }

    tft.fillRect(160, 250, 100, 60, TFT_BLACK);

    if (SPIFFS.exists("/weathericons-36.vlw")) {
        tft.loadFont("weathericons-36", SPIFFS);
        tft.setTextColor(iconColor, TFT_BLACK, true);
        tft.drawString(utf8FromCodepoint(iconCP), 160, 250); 
        tft.unloadFont();
    }
}

void lcdDrawWeather(String temp, String feel, String hum, String wind) {
    if (SPIFFS.exists("/"+ tempFont + ".vlw")) {
        tft.loadFont(tempFont, SPIFFS);
        WeatherData w = getWeather();
        if (w.outTemperature >= 15.0) {
            tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);
        }
        else if (w.outTemperature >= 0.0) {
            tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        }
        else {
            tft.setTextColor(TFT_CYAN, TFT_BLACK, true);
        }
        tft.drawString(temp, 140, 210);
        tft.unloadFont();

    } else {
        Serial.println("Missing font: /"+ tempFont + ".vlw");
    }
    tft.loadFont("weathericons-36", SPIFFS);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.drawString(utf8FromCodepoint(0xF03C), 212, 200);  // wi-celsius
    tft.unloadFont();

    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(feel, leftAlign_x, 240);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(hum, leftAlign_x, 270);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(wind, leftAlign_x, 300);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
}

void lcdDrawDHT(float temp, float hum) {
    if (SPIFFS.exists("/"+ tempFont + ".vlw")) {
        tft.loadFont(tempFont, SPIFFS);
        tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);
        tft.drawFloat(temp, 1, 140, 105);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ tempFont + ".vlw");
    }
    tft.loadFont("weathericons-36", SPIFFS);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.drawString(utf8FromCodepoint(0xF03C), 212, 95);  // wi-celsius
    tft.unloadFont();
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        // String(hum, 1) muuttaa floatin tekstiksi yhdellä desimaalilla
        String humMessage = "Hum: " + String(hum, 1) + "%  ";
        tft.drawString(humMessage, leftAlign_x, 135);

        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
}

void lcdDrawSCD(uint16_t co2) {
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        String co2Message = "CO2: " + String(co2) + " ppm  ";
        tft.drawString(co2Message, leftAlign_x, 165);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
}

void lcdDrawIN_OUT() {
    if (SPIFFS.exists("/"+ mainFontLight + ".vlw")) {
        tft.loadFont(mainFontLight, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("IN", leftAlign_x, 100);
        tft.drawString("OUT", leftAlign_x, 205);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFontLight + ".vlw");
    }
}

void lcdDrawCurrentPrice(String c) {
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        int currentHour = getCurrentHour();
        if (prices[currentHour] >= 15) {
            tft.setTextColor(TFT_RED, TFT_BLACK, true);
        }
        else if (prices[currentHour] >= 10 && prices[currentHour] <= 14.99) {
            tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);
        }
        else if (prices[currentHour] >= 5 && prices[currentHour] <= 9.99) {
            tft.setTextColor(TFT_YELLOW, TFT_BLACK, true);
        }
        else {
            tft.setTextColor(TFT_GREEN, TFT_BLACK, true);
        }
        tft.drawString(c, leftAlign_x, 60);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont2, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Electricity price", leftAlign_x, 40);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont2 + ".vlw");
    }
}

void lcdDrawPricePanel(int x, int y, int w, int h, String text, uint16_t color) {
    if (!SPIFFS.exists("/calibri24.vlw")) {
        Serial.println("Missing font: /calibri24.vlw");
        return;
    }
    tft.fillRect(x, y, w, h, TFT_BLACK);
    tft.loadFont(mainFont, SPIFFS);
    tft.setTextColor(color, TFT_BLACK, true);
    tft.drawString(text, x + 5, y + 5);
    tft.unloadFont();
}

void lcdDrawSpotGraph(float* prices) {
    int leftMargin = 30;
    int graphHeight = 192;
    int graphBottom = 305;
    int dataSize = 24;

    int graphWidth = 240 - leftMargin;

    float maxPrice = prices[0];
    float minPrice = prices[0];
    for (int i = 0; i < dataSize; i++) {
        if (prices[i] > maxPrice) maxPrice = prices[i];
        if (prices[i] < minPrice) minPrice = prices[i];
    }

    int currentHour = getCurrentHour();

    for (int i = 0; i < dataSize; i++) {
        int graphX = leftMargin + (i * graphWidth) / dataSize;
        int nextX  = leftMargin + ((i + 1) * graphWidth) / dataSize;
        int barWidth = nextX - graphX;

        int scaledHeight = (int)((prices[i] / maxPrice) * graphHeight);
        int graphY = graphBottom - scaledHeight;

        uint16_t barColor = (i % 2 == 0) ? TFT_WHITE : TFT_LIGHTGREY;

        if (i == currentHour) {
            barColor = TFT_BLUE;
        }
        else if (prices[i] == maxPrice) {
            barColor = TFT_RED;
        }
        else if (prices[i] == minPrice) {
            barColor = TFT_GREEN;
        }

        tft.fillRect(graphX, graphY, barWidth, scaledHeight, barColor);
    }
}

void lcdDrawSpotHours() {
    tft.setTextFont(1);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int leftMargin = 30;
    int graphBottom = 305;
    int yPos = graphBottom + 5;

    int dataSize = 24;
    int graphWidth = 240 - leftMargin;

    for (int hour = 0; hour < dataSize; hour++) {
        int graphX = leftMargin + (hour * graphWidth) / dataSize;
        int nextX  = leftMargin + ((hour + 1) * graphWidth) / dataSize;

        int xCenter = (graphX + nextX) / 2;

        if (hour % 3 == 0) {
            String hourLabel = String(hour);
            tft.drawCentreString(hourLabel, xCenter + 1, yPos, 1);
        }

        tft.drawFastVLine(xCenter, yPos - 5, 3, TFT_DARKGREY);
    }
}

void lcdDrawSpotPriceScale(float* prices) {
    int leftMargin = 30;
    int graphBottom = 305;
    int graphHeight = 192;

    int graphTop = graphBottom - graphHeight;

    float maxPrice = 0;
    for (int i = 0; i < 24; i++) {
        if (prices[i] > maxPrice) maxPrice = prices[i];
    }
    if (maxPrice == 0) maxPrice = 1;

    tft.setTextFont(1);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    if (maxPrice <= 0) maxPrice = 1;

    tft.drawRightString(String(maxPrice, 1), leftMargin - 2, graphTop - 5, 1);

    float midPrice = maxPrice / 2.0;
    int midY = graphBottom - (graphHeight / 2);

    tft.drawRightString(String(midPrice, 1), leftMargin - 2, midY - 5, 1);

    tft.drawRightString("0", leftMargin - 2, graphBottom - 5, 1);

    tft.drawFastVLine(leftMargin, graphTop, graphHeight, TFT_DARKGREY);
}

String utf8FromCodepoint(uint16_t cp) {
  String s = "";

  // 1-byte UTF-8 character (basic ASCII)
  if (cp < 0x80) {
    s += char(cp);
  }

  // 2-byte UTF-8 character
  else if (cp < 0x800) {
    s += char(0xC0 | (cp >> 6));
    s += char(0x80 | (cp & 0x3F));
  }

  // 3-byte UTF-8 character
  // Weather Icons code points such as 0xF00D go here
  else {
    s += char(0xE0 | (cp >> 12));
    s += char(0x80 | ((cp >> 6) & 0x3F));
    s += char(0x80 | (cp & 0x3F));
  }

  return s;
}
