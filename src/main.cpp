
#include <Arduino.h>
#include "LCD.h"

void setup() {
  Serial.begin(115200);
  lcd_setup();
}


void loop() {
  testWeatherIcons();
  delay(5000);
  testSmoothFonts();
  delay(5000);
}
