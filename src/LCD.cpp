#include "LCD.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>
#include <spinning_cat2.h>

TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;

#define GIF_IMAGE cat_green

static bool gifOpened = false;

// Draw callback for AnimatedGIF
void GIFDraw(GIFDRAW *pDraw)
{
    uint8_t *s;
    uint16_t *usPalette;
    int x, y, iWidth;

    iWidth = pDraw->iWidth;
    if (iWidth + pDraw->iX > tft.width())
        iWidth = tft.width() - pDraw->iX;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line
    s = pDraw->pPixels;

    tft.startWrite();

    // Handle transparency
    if (pDraw->ucHasTransparency)
    {
        uint8_t transparent = pDraw->ucTransparent;
        int xOffset = 0;

        while (xOffset < iWidth)
        {
            // Skip transparent pixels
            while (xOffset < iWidth && s[xOffset] == transparent)
                xOffset++;

            int start = xOffset;

            // Find solid run
            while (xOffset < iWidth && s[xOffset] != transparent)
                xOffset++;

            int count = xOffset - start;

            if (count > 0)
            {
                static uint16_t lineBuffer[320]; // enough for ILI9341 width
                for (int i = 0; i < count; i++)
                {
                    lineBuffer[i] = usPalette[s[start + i]];
                }

                tft.setAddrWindow(pDraw->iX + start, y, count, 1);
                tft.pushColors(lineBuffer, count, true);
            }
        }
    }
    else
    {
        static uint16_t lineBuffer[320];
        for (x = 0; x < iWidth; x++)
        {
            lineBuffer[x] = usPalette[*s++];
        }

        tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
        tft.pushColors(lineBuffer, iWidth, true);
    }

    tft.endWrite();
}

void lcd_setup(void)
{
    Serial.begin(115200);
    delay(1000);

    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("hello world!", 0, 0, 2);
    tft.drawString("lcd_v0.1", 0, 15, 2);

    gif.begin(LITTLE_ENDIAN_PIXELS);

    gifOpened = gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw);

    if (gifOpened)
        Serial.println("GIF opened successfully");
    else
        Serial.println("GIF failed to open");
}

void lcd_loop(void)
{
    if (!gifOpened)
        return;

    if (!gif.playFrame(true, NULL))
    {
        gif.close();
        gifOpened = gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFDraw);
    }
}