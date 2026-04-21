#pragma once

#include <Arduino.h>

void uiBegin();

void uiUpdateTime();
void uiUpdateDate();
void uiUpdatePrices();
void uiUpdateWeather();
void uiUpdateWifi();
void uiUpdateDHT();
void uiUpdateSCD();
void ui_menu(void);
void ui_24h_spotPrice(void);