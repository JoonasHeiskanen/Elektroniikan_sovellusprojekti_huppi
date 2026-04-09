#include <dht.h>

//Declare DHT libary classs
DHT20 DHT(&Wire);

//Variables for sensor values
float humidity;
float temperature;

void dht_setup(){
    //Serial.begin(115200); Put this in main.ccp
    Serial.print("DHT20 LIBRARY VERSION: ");
    Wire.begin(13,12); //I2C pins SDA 13 SCL 12
    DHT.begin();    //Start the DHT I2C
}

//This function reads DHT values and returns them in dht_values struct
dht_values dht_read()
{
  dht_values result = {0.0f, 0.0f};

  if (millis() - DHT.lastRead() >= 1000)
  {
    DHT.read(); // Read values

    humidity = DHT.getHumidity();
    temperature = DHT.getTemperature();

    result.humidity = humidity;
    result.temperature = temperature;

    Serial.print("Humidity: ");
    Serial.print(result.humidity, 2);
    Serial.print(" %, Temperature: ");
    Serial.print(result.temperature, 2);
    Serial.println(" °C");
  }

  return result;
}