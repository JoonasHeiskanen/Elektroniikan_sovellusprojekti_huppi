#pragma once

#include <Arduino.h>

struct WeatherData {
    float outTemperature;
    float feelsLike;
    int humidity;
    float wind;

    char desc[32];

    bool updated;
};

void weatherBegin();
bool weatherUpdate();

WeatherData getWeather();
