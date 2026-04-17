#pragma once

#include <Arduino.h>

void timeInit();

int getCurrentHour();
int getCurrentMinute();
int getCurrentDay();

String getCurrentTime();
String getDisplayDate();
String getApiDate();
