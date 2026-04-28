#include "dht.h"

DHT20 DHT(&Wire);

static DHTData state = {0.0f, 0.0f, false};
static unsigned long lastRead = 0;

void dhtSetup() {
    Wire.begin(13, 12);
    DHT.begin();
}

void dhtUpdate() {
    if (millis() - lastRead < 10000) return;
    lastRead = millis();

    DHT.read();

    float h = DHT.getHumidity();
    float t = DHT.getTemperature();

    if (!isnan(h) && !isnan(t)) {
        state.humidity = h;
        state.temperature = t;
        state.valid = true;
    } else {
        state.valid = false;
    }
}

DHTData dhtGet() {
    return state;
}
