#include <reg52.h>
#include "I2C_EEPROM.h" 
#include "LCD.h"

//dinh nghia chan I2C
sbit SDA = P2^2;
sbit SCL = P2^1;

extern unsigned char rfid_data[12]; // ma dang quet

unsigned char Total_Cards = 0;

void I2C_Delay(){ 
	unsigned char i; 
	for(i=0; i<10; i++); 
}

void Sys_Delay(unsigned int t) {
    unsigned int x, y;
    for(x = 0; x < t; x++) for(y = 0; y < 120; y++);
}

void I2C_Start(){ 
	SDA = 1; SCL = 1; I2C_Delay(); 
	SDA = 0; I2C_Delay(); SCL = 0; 
}

void I2C_Stop() {
	SDA = 0; SCL = 1; I2C_Delay(); 
	SDA = 1; I2C_Delay(); 
}

void I2C_Write(unsigned char dat) {
    unsigned char i;
    for(i=0; i<8; i++) {
        SDA = (dat & 0x80) ? 1 : 0; SCL = 1; I2C_Delay(); SCL = 0; I2C_Delay();
        dat <<= 1;
    }
    SCL = 1; I2C_Delay(); SCL = 0; // ACK
}

unsigned char I2C_Read() {
    unsigned char i, dat=0;
    SDA = 1; 
    for(i=0; i<8; i++) {
        SCL = 1; I2C_Delay();
        dat <<= 1; if(SDA) dat |= 1;
        SCL = 0; I2C_Delay();
    }
    return dat;
}

void EEPROM_Write_Byte(unsigned char addr, unsigned char dat) {
    I2C_Start(); I2C_Write(0xA0); I2C_Write(addr); I2C_Write(dat); I2C_Stop();
    Sys_Delay(10); //cho ghi
}

unsigned char EEPROM_Read_Byte(unsigned char addr) {
    unsigned char dat;
    I2C_Start(); I2C_Write(0xA0); I2C_Write(addr);
    I2C_Start(); I2C_Write(0xA1); dat = I2C_Read(); I2C_Stop();
    return dat;
}

void EEPROM_Save_Card() {
    unsigned char i, start_addr;
    Total_Cards = EEPROM_Read_Byte(0x00);
    if(Total_Cards == 0xFF) Total_Cards = 0; //chip moi
    if(Total_Cards >= 10) return; //hon 10 the
    
    start_addr = 0x10 + (Total_Cards * 12);
    for(i=0; i<12; i++) EEPROM_Write_Byte(start_addr + i, rfid_data[i]);
    
    Total_Cards++;
    EEPROM_Write_Byte(0x00, Total_Cards);
    LCD_Cmd(0x01); Sys_Delay(10); LCD_String("Card Saved!"); Sys_Delay(1000);
}

void EEPROM_Delete_All() {
    EEPROM_Write_Byte(0x00, 0);
    LCD_Cmd(0x01); Sys_Delay(10); LCD_String("All Deleted!"); Sys_Delay(1000);
}

unsigned char Check_Card_In_EEPROM() {
    unsigned char i, j, addr, match, card_byte;
    Total_Cards = EEPROM_Read_Byte(0x00);
    if(Total_Cards == 0xFF || Total_Cards == 0) return 0;

    for(i=0; i < Total_Cards; i++) {
        addr = 0x10 + (i * 12);
        match = 1;
        for(j=0; j<12; j++) {
            card_byte = EEPROM_Read_Byte(addr + j);
            if(card_byte != rfid_data[j]) { match = 0; break; }
        }
        if(match == 1) return 1; 
    }
    return 0; 
}