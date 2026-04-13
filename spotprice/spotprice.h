#pragma once

#include <Arduino.h>

extern float prices[24];

void spotInit();
void fetchPrices();

float getPrice(int hour);
float getCurrentPrice();
float getNextPrice();
int getCurrentHour();
String getCurrentDate();
