#ifndef _I2C_EEPROM_H_
#define _I2C_EEPROM_H_

extern unsigned char rfid_data[12]; // ma dang quet

void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(unsigned char dat);
unsigned char I2C_Read(void);
void EEPROM_Write_Byte(unsigned char addr, unsigned char dat);
unsigned char EEPROM_Read_Byte(unsigned char addr);
void EEPROM_Save_Card(void);
void EEPROM_Delete_All(void);
unsigned char Check_Card_In_EEPROM(void);

#endif