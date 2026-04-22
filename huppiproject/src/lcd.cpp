#include <Arduino.h>
#include <TFT_eSPI.h>

#include "lcd.h"
#include "timeutils.h"

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

void lcdDrawText(int x, int y, String text, int width = 240, int height = 20) {
    tft.fillRect(x, y, width, height, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(text, x, y, 2);
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
