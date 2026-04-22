#ifndef LCD_H
#define LCD_H

void lcdSetup(void);

void lcdClear();

void lcdDrawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye);

void lcdDrawText(int x, int y, String text, int width, int height);

void lcdDrawDate(String d);

void lcdDrawTime(String t);

void lcdDrawWeather(String temp, String temp2, String desc);

void lcdDrawSpotGraph(float* prices);

#endif
