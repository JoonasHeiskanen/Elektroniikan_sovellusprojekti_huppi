#ifndef LCD_H
#define LCD_H

void lcd_setup(void);
void lcd_clear();
void lcd_drawText(int x, int y, String text);

#endif
