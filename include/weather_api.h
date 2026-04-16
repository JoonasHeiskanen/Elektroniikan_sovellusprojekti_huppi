#ifndef WEATHER_API_H
#define WEATHER_API_H

struct weatherData {
    float out_temperature;
    float feelsLike;
    float wind;
    char desc[64];
    bool updated; // Flag when we know when data has arrived
};

extern weatherData weatherNow;

void getWeather(void);
void weather_setup(void);
void printWeather(void);
void checkWifiConnection(void);

#endif