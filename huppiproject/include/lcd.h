#ifndef LCD_H
#define LCD_H

void lcdSetup(void);
void lcdClear();
void lcdDrawText(int x, int y, String text, int width, int height);
void lcdDrawSpotGraph(float* prices);

#endif
