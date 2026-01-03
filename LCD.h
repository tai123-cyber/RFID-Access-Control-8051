#ifndef _LCD_H_
#define _LCD_H_

void LCD_Init();
void LCD_Cmd(unsigned char cmd);
void LCD_Char(unsigned char dat);
void LCD_String(char *str);

#endif