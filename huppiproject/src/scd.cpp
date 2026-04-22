#include "scd.h"
#include <Wire.h>
#include <SensirionI2cScd4x.h>

SensirionI2cScd4x scd4x;

static SCDData state = {0, 0.0f, 0.0f, false};
static unsigned long lastRead = 0;

void scdSetup() {
    Wire.begin(13, 12);

    scd4x.begin(Wire, 0x62);

    uint16_t error;

    error = scd4x.stopPeriodicMeasurement();
    delay(500);

    error = scd4x.startPeriodicMeasurement();
}

void scdUpdate() {
    if (millis() - lastRead < 2000) return;
    lastRead = millis();

    uint16_t error;
    char errorMessage[256];

    bool dataReady = false;

    error = scd4x.getDataReadyStatus(dataReady);
    if (error || !dataReady) return;

    uint16_t co2;
    float temperature;
    float humidity;

    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) return;

    if (co2 == 0) return;

    state.co2 = co2;
    state.temperature = temperature;
    state.humidity = humidity;
    state.valid = true;
}

SCDData scdGet() {
    return state;
}
