#ifndef WEATHER_API_H
#define WEATHER_API_H

struct weatherData {
    float out_temperature;
    float feelsLike;
    float wind;
    char desc[64];
    bool updated; // Lippu, jotta tiedetään milloin uusi data on saapunut
};

extern weatherData weatherNow;

void getWeather(void);
void weather_setup(void);
void printWeather(void);

#endif