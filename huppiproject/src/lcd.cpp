#include <Arduino.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include "weather.h"

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

void lcdDrawText(int x, int y, String text, int width = 240, int height = 20) {
    tft.fillRect(x, y, width, height, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(text, x, y, 1);
}

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
        tft.drawString(t, 175, topAlign_y);
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

    if (code == "01d" || code == "01n") {
        iconCP = WI_DAY_SUNNY;
        iconColor = TFT_ORANGE;
    } 
    else if (code == "02d" || code == "02n") {
        iconCP = WI_DAY_SUNNY_OVERCAST;
        iconColor = TFT_WHITE;
    }
    else if (code == "09d" || code == "09n") {
        iconCP = WI_RAIN; // Rankkasade / Kuurot
        iconColor = TFT_WHITE;
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

    if (SPIFFS.exists("/weathericons-36.vlw")) {
        tft.loadFont("weathericons-36", SPIFFS);
        tft.setTextColor(iconColor, TFT_BLACK, true);
        tft.drawString(utf8FromCodepoint(iconCP), 160, 250); 
        tft.unloadFont();
    }
}

void lcdDrawWeather(String temp, String feel, String hum, String wind, String desc) {
    if (SPIFFS.exists("/"+ tempFont + ".vlw")) {
        tft.loadFont(tempFont, SPIFFS);
        tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);
        tft.drawString(temp, 140, 205);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ tempFont + ".vlw");
    }

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
    /*if (SPIFFS.exists("/calibri24.vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(desc, 200, 250);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /calibri24.vlw");
    }*/
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
    tft.drawString(utf8FromCodepoint(0xF03C), 200, 105);  // wi-celsius
    tft.unloadFont();
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        // Luodaan valmis teksti: "Humidity: 45.2%"
        // String(hum, 1) muuttaa floatin tekstiksi yhdellä desimaalilla
        String humMessage = "Hum: " + String(hum, 1) + "%  ";
        
        // Tulostetaan koko viesti mainFontilla
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
    if (SPIFFS.exists("/"+ mainFont + ".vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("IN", leftAlign_x, 100);
        tft.drawString("OUT", leftAlign_x, 205);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /"+ mainFont + ".vlw");
    }
}

void lcdDrawSpotHours() {
    tft.setTextFont(1);             // Käytetään pienintä vakiofonttia
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    int yPos = 160;                 // Haluttu y-akselin korkeus
    int pixelsPerHour = 10;         // 240 pikseliä / 24 tuntia = 10px per tunti

    for (int hour = 0; hour < 24; hour++) {
        // Lasketaan x-akselin sijainti
        int xPos = hour * pixelsPerHour;

        // Tulostetaan teksti esim. 3 tunnin välein (00, 03, 06...), 
        // jotta numerot eivät mene päällekkäin
        if (hour % 3 == 0) {
            String hourLabel = (hour < 10 ? "0" : "") + String(hour);
            
            // drawCentreString auttaa pitämään numeron keskellä sille varattua 10px tilaa
            tft.drawCentreString(hourLabel, xPos, yPos, 1);
        }

        // Piirretään pieni harmaa merkkiviiva jokaisen tunnin kohdalle 
        // y-akselille 160:n yläpuolelle
        tft.drawFastVLine(xPos, yPos - 5, 3, TFT_DARKGREY);
    }
}

void lcdDrawCurrentPrice(String c) {
    if (SPIFFS.exists("/calibri24.vlw")) {
        tft.loadFont(mainFont, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString(c, leftAlign_x, 60);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /calibri24.vlw");
    }
    if (SPIFFS.exists("/calibri16.vlw")) {
        tft.loadFont(mainFont2, SPIFFS);
        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.drawString("Electricity price", leftAlign_x, 40);
        tft.unloadFont();
    } else {
        Serial.println("Missing font: /calibri24.vlw");
    }
}

void lcdDrawSpotGraph(float* prices) {
    int graphHeight = 100;  // Height of the graph area (60% of screen = 192)
    int graphWidth = 240;   // Width of the screen
    int graphBottom = 320;  // Position of the bottom of the screen (height of the TFT screen)
    int dataSize = 24;      // Size of data array

    // Find maximum price to scale the graph
    float maxPrice = 0;
    for (int i = 0; i < dataSize; i++) {
        if (prices[i] > maxPrice) {
            maxPrice = prices[i];
        }
    }

    // Get the current hour
    int currentHour = getCurrentHour();

    // Draw bars for graph according to data array size
    for (int i = 0; i < dataSize; i++) {
        // Scale bar height by maxPrice
        int scaledHeight = (int)((prices[i] / maxPrice) * graphHeight);

        // X position of the bar
        int graphX = map(i, 0, dataSize, 0, graphWidth);

        // Y position (bottom up)
        int graphY = graphBottom - scaledHeight;

        // Alternate column colors White - LightGrey
        uint16_t barColor = (i % 2 == 0) ? TFT_WHITE : TFT_LIGHTGREY;

        // Highlight current hour in Blue
        if (i == currentHour) {
            barColor = TFT_BLUE;
        }

        // Highlight peak price in Red
        if (prices[i] == maxPrice) {
            barColor = TFT_RED;
        }

        // Draw bar
        tft.fillRect(graphX, graphY, 10, scaledHeight, barColor);  // Draw the bar
    }
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
