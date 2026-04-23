#pragma once

#include <Arduino.h>
#include "buttons.h"

void uiBegin();

void uiLines(int s);

void uiUpdateTime();
void uiUpdateDate();

void uiUpdatePrices();

void uiUpdateCurrentPrice();

void uiUpdateWeather();

void uiUpdateWifi(bool);

void uiUpdateDHT();
void uiUpdateSCD();

void uiSpotGraph();

void uiRender(DisplayState state);
