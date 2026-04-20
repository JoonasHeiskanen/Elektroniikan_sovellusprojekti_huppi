#include <Arduino.h>
#include "lcd.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void lcdSetup() {
    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
}

void lcdClear() {
    tft.fillScreen(TFT_BLACK);
}

void lcdDrawText(int x, int y, String text) {
    tft.fillRect(x, y, 240, 20, TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(text, x, y, 2);
}
