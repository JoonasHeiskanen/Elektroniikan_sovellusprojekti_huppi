#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "weather.h"
#include "secrets.h"

static WeatherData weatherNow = {0.0, 0.0, 0, 0.0, "No data", false};

void weatherBegin() {
    weatherNow.updated = false;
}

WeatherData getWeather() {
    return weatherNow;
}

bool weatherUpdate() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }

    HTTPClient http;

    String url = "http://api.openweathermap.org/data/2.5/weather?q=Oulu,FI&appid=" + String(WEATHER_API_KEY) + "&units=metric&lang=en";

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        return false;
    }

    weatherNow.outTemperature = doc["main"]["temp"];
    weatherNow.feelsLike = doc["main"]["feels_like"];
    weatherNow.humidity = doc["main"]["humidity"];
    weatherNow.wind = doc["wind"]["speed"];

    const char* desc = doc["weather"][0]["description"];
    strncpy(weatherNow.desc, desc, sizeof(weatherNow.desc) - 1);
    weatherNow.desc[sizeof(weatherNow.desc) - 1] = '\0';

    weatherNow.updated = true;

    return true;
}
