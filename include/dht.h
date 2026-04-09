#ifndef __DHT_H__
#define __DHT_H__

#include <DHT20.h>
#include <wire.h>

extern float humidity;
extern float temperature;

struct dht_values {
    float humidity;
    float temperature;
};

void dht_setup(void);

dht_values dht_read(void);

#endif