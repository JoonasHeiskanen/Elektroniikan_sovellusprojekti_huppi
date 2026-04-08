#ifndef __LCD_H__
#define __LCD_H__

void lcd_setup(void);
void lcd_loop(void);
//#pragma once

#define ILI9341_2_DRIVER

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

#define SPI_FREQUENCY       20000000
#define SPI_READ_FREQUENCY  5000000

#endif