#ifndef USER_SETUP_H
#define USER_SETUP_H

#define USER_SETUP_INFO "MY_CUSTOM_SETUP"
#define ILI9341_DRIVER

#define TFT_WIDTH  320
#define TFT_HEIGHT 240

#define TFT_CS   4
#define TFT_RST  5
#define TFT_DC   6
#define TFT_MOSI 7
#define TFT_SCLK 15
#define TFT_MISO 17

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF

#define TFT_INVERSION_ON

#define SPI_FREQUENCY       10000000
#define SPI_READ_FREQUENCY  5000000

#endif