#pragma once

#include <Arduino.h>
#include <WiFi.h>

void networkBegin();

void networkUpdate();

bool isWifiConnected();

void networkReconnect();
