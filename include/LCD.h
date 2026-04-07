#ifndef __LCD_H__
#define __LCD_H__

void lcd_setup(void);

#pragma once

#define ILI9341_2_DRIVER

#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF

#define TFT_INVERSION_ON

#define SPI_FREQUENCY       10000000
#define SPI_READ_FREQUENCY  5000000

#endif