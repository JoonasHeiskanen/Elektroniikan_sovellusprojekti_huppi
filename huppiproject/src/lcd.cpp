#include <Arduino.h>
#include "lcd.h"
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>

TFT_eSPI tft = TFT_eSPI();

void lcd_setup() {
  Serial.println("lcd_setup start");

  //Check for User_Setup.h
  #ifdef USER_SETUP_INFO
    Serial.println(USER_SETUP_INFO);
  #else
    Serial.println("USER_SETUP_INFO not found");
  #endif

//Check for SPIFFS
//SPIFFS = internal flash memory location where fonts are loaded into
 if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed");
    while (true) delay(1000);
  }

  //Set SCREEN_LED pin high
  pinMode(16, OUTPUT);
  digitalWrite(16,HIGH);

  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawString("hello world!", 0, 0, 2);

  Serial.println("lcd setup done");
}

void lcd_clear() {
    tft.fillScreen(TFT_BLACK);
}

void lcd_drawText(int x, int y, String text) {
    tft.fillRect(x, y, 240, 20, TFT_BLACK);

    //Normal ESPI fonts:
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(text, x, y, 2);

    /*
    //Smooth fonts:
    if (SPIFFS.exists("/calibri16.vlw")) {
      tft.loadFont("calibril16", SPIFFS);
      tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);
      tft.drawString(text, 0, y);
      //Serial.println("Printed with font: calibri16");
      y += tft.fontHeight() + 6;
      tft.unloadFont();
    } else {
      Serial.println("Missing font: /calibri16.vlw");
    }
    */
    
}

// Convert a Unicode/icon code point into a UTF-8 string
// TFT_eSPI smooth fonts need UTF-8 text, not raw hex values
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

void testWeatherIcons() {
  // Mount SPIFFS so the .vlw weather font can be loaded
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed");
    return;
  }

  // Clear screen and set icon color
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK, true);

  // Load the weather icon font from SPIFFS
  tft.loadFont("weathericons-36", SPIFFS);

  // Draw icons directly using the same Unicode values
  // that were added to specificUnicodes in the Processing tool

  tft.drawString(utf8FromCodepoint(0xF00D), 10, 20);   // wi-day-sunny
  tft.drawString(utf8FromCodepoint(0xF002), 50, 20);   // wi-day-sunny-overcast
  tft.drawString(utf8FromCodepoint(0xF041), 90, 20);   // wi-cloud
  tft.drawString(utf8FromCodepoint(0xF013), 130, 20);  // wi-cloudy
  tft.drawString(utf8FromCodepoint(0xF019), 170, 20);  // wi-rain

  tft.drawString(utf8FromCodepoint(0xF008), 10, 80);   // wi-day-rain
  tft.drawString(utf8FromCodepoint(0xF01E), 50, 80);   // wi-thunderstorm
  tft.drawString(utf8FromCodepoint(0xF064), 90, 80);   // wi-snow-wind
  tft.drawString(utf8FromCodepoint(0xF014), 130, 80);  // wi-fog
  tft.drawString(utf8FromCodepoint(0xF050), 170, 80);  // wi-strong-wind

  tft.drawString(utf8FromCodepoint(0xF03C), 10, 140);  // wi-celsius
  tft.drawString(utf8FromCodepoint(0xF07A), 50, 140);  // wi-humidity
  tft.drawString(utf8FromCodepoint(0xF042), 90, 140);  // wi-degrees
  tft.drawString(utf8FromCodepoint(0xF03D), 130, 140); // wi-cloud-down
  tft.drawString(utf8FromCodepoint(0xF040), 170, 140); // wi-cloud-up

  // Free font from memory after use
  tft.unloadFont();
}

void testSmoothFonts() {

  // Mount SPIFFS so the .vlw font files can be loaded
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed");
    return;
  }

  // Clear screen and set default text colors
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);

  // Current Y position for printing lines
  int y = 0;

  // -----------------------------
  // Font 1: calibri16
  // -----------------------------
  if (SPIFFS.exists("/calibri16.vlw")) {
    tft.loadFont("calibri16", SPIFFS);
    tft.drawString("Hello World!123", 0, y);
    Serial.println("Printed with font: calibri16");
    y += tft.fontHeight() + 6;
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibri16.vlw");
  }

  // -----------------------------
  // Font 2: calibri24
  // -----------------------------
  if (SPIFFS.exists("/calibri24.vlw")) {
    tft.loadFont("calibri24", SPIFFS);
    tft.drawString("Hello World!123", 0, y);
    Serial.println("Printed with font: calibri24");
    y += tft.fontHeight() + 6;
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibri24.vlw");
  }

  // -----------------------------
  // Font 3: calibri36
  // -----------------------------
  if (SPIFFS.exists("/calibri36.vlw")) {
    tft.loadFont("calibri36", SPIFFS);
    tft.drawString("Hello World!123", 0, y);
    Serial.println("Printed with font: calibri36");
    y += tft.fontHeight() + 6;
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibri36.vlw");
  }

  // If the next fonts will not fit, pause and clear screen
  if (y > tft.height() - 60) {
    delay(2500);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    y = 0;
  }

  // -----------------------------
  // Font 4: calibril16
  // -----------------------------
  if (SPIFFS.exists("/calibril16.vlw")) {
    tft.loadFont("calibril16", SPIFFS);
    tft.drawString("Hello World!123", 0, y);
    Serial.println("Printed with font: calibril16");
    y += tft.fontHeight() + 6;
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibril16.vlw");
  }

  // -----------------------------
  // Font 5: calibril24
  // -----------------------------
  if (SPIFFS.exists("/calibril24.vlw")) {
    tft.loadFont("calibril24", SPIFFS);
    tft.drawString("Hello World!123", 0, y);
    Serial.println("Printed with font: calibril24");
    y += tft.fontHeight() + 6;
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibril24.vlw");
  }

  // -----------------------------
  // Font 6: calibril36
  // -----------------------------
  if (SPIFFS.exists("/calibril36.vlw")) {
    tft.loadFont("calibril36", SPIFFS);
    tft.drawString("Hello World!123", 0, y);
    Serial.println("Printed with font: calibril36");
    y += tft.fontHeight() + 6;
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibril36.vlw");
  }

  // Show the last big font on a fresh screen so it fits properly
  delay(2500);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  y = 0;

  // -----------------------------
  // Font 7: DSEG7Modern-Bold36
  // -----------------------------
  if (SPIFFS.exists("/DSEG7Modern-Bold36.vlw")) {
    tft.loadFont("DSEG7Modern-Bold36", SPIFFS);
    tft.drawString("0123456789", 0, y);
    Serial.println("Printed with font: DSEG7Modern-Bold36");
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /DSEG7Modern-Bold36.vlw");
  }
}

void lcd_UpdateDate(String d){

  tft.drawRect(0,50,120,40,TFT_GREEN);

  if (SPIFFS.exists("/calibri16.vlw")) {
    tft.loadFont("calibril16", SPIFFS);
    tft.setTextColor(TFT_GREEN, TFT_BLACK, true);
    tft.drawString(d, 0, 300);
    //Serial.println("Printed with font: calibri16");
    tft.unloadFont();
  } else {
    Serial.println("Missing font: /calibri16.vlw");
  }
}