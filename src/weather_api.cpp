#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weather_api.h"
#include "secrets.h"

String city = "Oulu";
String couyntryCode = "FI";

weatherData weatherNow = {0.0, 0.0, 0.0, "Ladataan...", false};

unsigned long lastUpdate = 0;
const unsigned long vali = 30000; // 3min

void printWeather() {
    unsigned long timeNow = millis();
    
    //Prints weather every 3min
    if (lastUpdate == 0 || timeNow - lastUpdate >= vali) {
        Serial.println("Updating weatherdata...");
        lastUpdate = timeNow;
        getWeather();
    }
}

void getWeather() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + couyntryCode + "&appid=" + String(WEATHER_API_KEY) + "&units=metric&lang=en";

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode == 200) { // Success get
            String payload = http.getString();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                // --- Saving data to struct ---
                weatherNow.out_temperature = doc["main"]["temp"];
                weatherNow.feelsLike = doc["main"]["feels_like"];
                weatherNow.wind = doc["wind"]["speed"];
                
                const char* desc = doc["weather"][0]["description"];
                strncpy(weatherNow.desc, desc, sizeof(weatherNow.desc) - 1);
                weatherNow.desc[sizeof(weatherNow.desc) - 1] = '\0'; // Varmistus
                
                // Nostetaan lippu ylös vasta kun kaikki data on tallennettu
                weatherNow.updated = true; 

                // Debug-print (using struct values)
                Serial.println("--- Weather updated to struct ---");
                Serial.print("Description: "); Serial.println(weatherNow.desc);
                Serial.print("Temperature: "); Serial.print(weatherNow.out_temperature); Serial.println(" °C");
                Serial.print("Feels like: "); Serial.print(weatherNow.feelsLike); Serial.println(" °C");
                Serial.print("Wind: "); Serial.print(weatherNow.wind); Serial.println(" m/s");
                // -------------------------------------------
            } else {
                Serial.print("JSON parse failed: ");
                Serial.println(error.f_str());
            }
        } else {
            Serial.print("Error HTTP-Get: ");
            Serial.println(httpCode);
        }
        http.end();
    } else {
        Serial.println("No WiFi-Connection: ");
    }
}

void weather_setup() {
    //Wifi setup
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
}