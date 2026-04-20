#pragma once

#include <Arduino.h>
#include "buttons.h"

void uiBegin();

void uiUpdateTime();
void uiUpdateDate();
void uiUpdatePrices();
void uiUpdateWeather();
void uiUpdateWifi(bool);
void uiUpdateDHT();
void uiUpdateSCD();
void uiRender(DisplayState state);
