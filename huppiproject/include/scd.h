#pragma once

#include <Arduino.h>

struct SCDData {
    uint16_t co2;
    float temperature;
    float humidity;
    bool valid;
};

void scdSetup();
void scdUpdate();
SCDData scdGet();
