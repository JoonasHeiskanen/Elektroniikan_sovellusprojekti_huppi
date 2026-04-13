#include <Arduino.h>
#include "lcd.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void lcd_setup() {
  Serial.println("lcd_setup start");

  #ifdef USER_SETUP_INFO
    Serial.println(USER_SETUP_INFO);
  #else
    Serial.println("USER_SETUP_INFO not found");
  #endif

  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString("hello world!", 0, 0, 2);

  Serial.println("lcd setup done");
}

void lcd_printToScreen(float Data){
  tft.fillScreen(TFT_CYAN);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawFloat(Data,2,0,0);
}