#include <LCD.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void lcd_setup() {
  Serial.begin(115200);
  delay(1000);
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(0x0129);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("hello world!", 0, 0, 2);
  tft.drawString("lcd_v0.1", 0, 15, 2);
}