#ifndef __DHT_H__
#define __DHT_H__

#include <DHT20.h>
#include <Wire.h>

struct DHTData {
    float humidity;
    float temperature;
    bool valid;
};

void dhtSetup();
void dhtUpdate();
DHTData dhtGet();

#endif
