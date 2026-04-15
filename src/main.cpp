#include <Arduino.h>
#include <Wire.h>
#include <SensirionI2cScd4x.h>

SensirionI2cScd4x scd4x;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("SCD4x test start");

  Wire.begin(13, 12);

  uint16_t error;
  char errorMessage[256];

  scd4x.begin(Wire, 0x62);

  // Stop any previous measurement mode first
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
    Serial.print("stopPeriodicMeasurement() failed: ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
  }

  delay(500);

  // Start normal periodic measurement
  error = scd4x.startPeriodicMeasurement();
  if (error) {
    Serial.print("startPeriodicMeasurement() failed: ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    return;
  }

  Serial.println("Waiting for first measurement...");
}

void loop() {
  uint16_t error;
  char errorMessage[256];

  bool dataReady = false;
  uint16_t co2 = 0;
  float temperature = 0.0f;
  float humidity = 0.0f;

  error = scd4x.getDataReadyStatus(dataReady);
  if (error) {
    Serial.print("getDataReadyStatus() failed: ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    delay(1000);
    return;
  }

  if (!dataReady) {
    Serial.println("No new data yet...");
    delay(1000);
    return;
  }

  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    Serial.print("readMeasurement() failed: ");
    errorToString(error, errorMessage, sizeof(errorMessage));
    Serial.println(errorMessage);
    delay(1000);
    return;
  }

  // Sometimes a 0 ppm reading means invalid data
  if (co2 == 0) {
    Serial.println("Invalid sample detected");
  } else {
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print(" ppm | Temp: ");
    Serial.print(temperature, 2);
    Serial.print(" C | RH: ");
    Serial.print(humidity, 2);
    Serial.println(" %");
  }

  delay(2000);
}