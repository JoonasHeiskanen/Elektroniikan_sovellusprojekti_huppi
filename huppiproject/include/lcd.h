#ifndef LCD_H
#define LCD_H

void lcdSetup(void);

void lcdClear();

void lcdDrawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye);

void lcdDrawText(int x, int y, String text, int width, int height);

void lcdDrawDate(String d);

void lcdDrawTime(String t);

void lcdDrawWeather(String temp, String temp2, String hum, String wind, String desc);

void lcdDrawCurrentPrice(String c);

void lcdDrawSpotGraph(float* prices);

void lcdDrawDHT(float temp, float hum);

void lcdDrawSCD(uint16_t co2);

void lcdDrawIN_OUT();

struct WeatherData;

void lcdDrawWeatherIcon(WeatherData w);

void lcdSetTextSize(uint8_t size);

void lcdDrawSpotHours();

//Function that converts numeric code into a UTF-8 string.
String utf8FromCodepoint(uint16_t cp);

//Codes for weather icons
constexpr uint16_t WI_DAY_SUNNY           = 0xF00D; // clear sky
constexpr uint16_t WI_DAY_SUNNY_OVERCAST  = 0xF002; // few clouds
constexpr uint16_t WI_CLOUD               = 0xF041; // scattered clouds
constexpr uint16_t WI_CLOUDY              = 0xF013; // broken clouds
constexpr uint16_t WI_RAIN                = 0xF019; // shower rain
constexpr uint16_t WI_DAY_RAIN            = 0xF008; // rain
constexpr uint16_t WI_THUNDERSTORM        = 0xF01E; // thunderstorm
constexpr uint16_t WI_SNOW_WIND           = 0xF064; // snow
constexpr uint16_t WI_FOG                 = 0xF014; // mist

constexpr uint16_t WI_STRONG_WIND         = 0xF050; //wind icon
constexpr uint16_t WI_CELSIUS             = 0xF03C; //celsius icon
constexpr uint16_t WI_HUMIDITY            = 0xF07A; // droplet and %
constexpr uint16_t WI_DEGREES             = 0xF042; // only the celsius dot .
constexpr uint16_t WI_CLOUD_DOWN          = 0xF03D; //wifi down
constexpr uint16_t WI_CLOUD_UP            = 0xF040; //wifi up

#endif
