#include <reg52.h>
#include "LCD.h"

//LCD
sbit LCD_RS = P2^7;
sbit LCD_RW = P2^6;
sbit LCD_EN = P2^5;
#define LCD_DATA P0

//ham delay rieng 
void LCD_Delay(unsigned int t) {
    unsigned int x, y;
    for(x = 0; x < t; x++) for(y = 0; y < 120; y++);
}
//khoi tao LCD
void LCD_Init() {
    LCD_Delay(20);
    LCD_Cmd(0x38);
		LCD_Cmd(0x0C); 
		LCD_Cmd(0x06); 
		LCD_Cmd(0x01);
		LCD_Delay(20);
}
//gui mot lenh
void LCD_Cmd(unsigned char cmd) {
    LCD_DATA = cmd; LCD_RS = 0; LCD_RW = 0; LCD_EN = 1; 
    LCD_Delay(2); LCD_EN = 0;
}
//gui mot ki tu ra man hinh
void LCD_Char(unsigned char dat) {
    LCD_DATA = dat; LCD_RS = 1; LCD_RW = 0; LCD_EN = 1; 
    LCD_Delay(1); LCD_EN = 0;
}
//gui mot chuoi ki tu ra man hinh
void LCD_String(char *str) {
    while(*str) LCD_Char(*str++);
}