#pragma once

#include <Arduino.h>

struct WeatherData {
    float out_temperature;
    float feelsLike;
    float wind;
    char desc[32];
    bool updated;
};

void weatherBegin();
bool weatherUpdate();

WeatherData getWeather();
