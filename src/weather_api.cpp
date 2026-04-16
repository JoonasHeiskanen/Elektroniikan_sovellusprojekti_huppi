#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weather_api.h"
#include "secrets.h"

String city = "Oulu";
String couyntryCode = "Fi";

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

void checkWifiConnection() {
    // If connection is true --> return
    if (WiFi.status() == WL_CONNECTED) {
        return; 
    }

    // If connection is false
    digitalWrite(38, LOW);
    Serial.println("Connection lost, trying to reconnect...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait 10 seconds to reconnect wifi
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
        delay(500);
        Serial.print(".");
    }

    // Check if connection is true
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi reconnected!");
        digitalWrite(38, HIGH); //Set led high
    } else {
        Serial.println("\nConnection failed.");
        digitalWrite(38, LOW); //Set led low
    }
}

void getWeather() {
    checkWifiConnection();
    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(38, HIGH);
        HTTPClient http;
        String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + couyntryCode + "&appid=" + String(WEATHER_API_KEY) + "&units=metric&lang=en";

        http.begin(url);
        int httpCode = http.GET();

        if (httpCode == 200) { // Success get
            String payload = http.getString();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);
            // Lisää getWeather-funktioon JSON-parsimisen jälkeen:
            long dataTime = doc["dt"];
            Serial.print("Datan mittausaika (Unix): ");
            Serial.println(dataTime);

            if (!error) {
                // --- Saving data to struct ---
                weatherNow.out_temperature = doc["main"]["temp"];
                weatherNow.feelsLike = doc["main"]["feels_like"];
                weatherNow.wind = doc["wind"]["speed"];
                
                const char* desc = doc["weather"][0]["description"];
                strncpy(weatherNow.desc, desc, sizeof(weatherNow.desc) - 1);
                weatherNow.desc[sizeof(weatherNow.desc) - 1] = '\0';
                
                // Flag up when data is saved
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
    //lastUpdate = millis(); //If used when device restart, does not do instant http-GET from api
    pinMode(38, OUTPUT);
    digitalWrite(38, HIGH);
}