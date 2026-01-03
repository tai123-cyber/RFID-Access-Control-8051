#include <reg52.h>
#include <string.h>
#include "LCD.h"
#include "I2C_EEPROM.h"
//
sbit RELAY = P2^0;  // LED 
sbit BUTTON = P1^0; //nut nhan exit

unsigned char code MASTER_CARD[] = "18008DC02E7B"; //the master

unsigned char rfid_data[12]; // ma dang quet

//Khai báo hàm
void Delay_ms(unsigned int t);
void UART_Init(void);
void UART_Receive_Tag(void);
void Open_Door(void);
unsigned char Check_Is_Master();

//main
void main() {
		unsigned char Mode = 0; // 0:che do thuong, 1:che do them the

    //trang thai ban dau
    RELAY = 0;      // Khóa cua
    BUTTON = 1;     // nut bam
    
    //cac module
    UART_Init();    // UART 9600 baud
    LCD_Init();     // LCD 16x2
    
    //man hinh khoi dong
    LCD_Cmd(0x80); LCD_String("RFID ACCESS");
    LCD_Cmd(0xC0); LCD_String("System Ready..");
    Delay_ms(1000);
    
    //màn hinh chinh
    LCD_Cmd(0x01); Delay_ms(10);
    LCD_String("RFID ACCESS");
    LCD_Cmd(0xC0);
    LCD_String("Swipe Tag..");

    while(1) {
        // cho tin hieu
        while (RI == 0 && BUTTON == 1);
				//TH1: nhan nut exit 
        if (BUTTON == 0) {
            Delay_ms(20); //Debounce
            if (BUTTON == 0) {
                Open_Door(); //mo cua
                while(BUTTON == 0); //cho nha nut
            }
        }
        //TH2: quet the
        else if (RI == 1) {
            UART_Receive_Tag(); //doc du lieu the vao rfid_data
            
            //the master
            if (Check_Is_Master() == 1) {
                if (Mode == 0) {
                    //chuyen sang che do them the
                    Mode = 1; 
                    LCD_Cmd(0x01); Delay_ms(10); LCD_String("MASTER CARD"); 
                    LCD_Cmd(0xC0); LCD_String("Mode: ADD USER");
             
                } 
                else {
                    //quet the master lan nua -> xoa sach
                    EEPROM_Delete_All();
                    Mode = 0; //quay lai ban dau
                }
            }
            
            //the nguoi dung
            else {
                //dang cho de them (mode=1)
                if (Mode == 1) {
                    EEPROM_Save_Card(); //luu the vao EEPROM
                    Mode = 0; // quay lai che do thuong
                }
                //neu che do thuong (mode=0);
                else {
                    if (Check_Card_In_EEPROM() == 1) {
                        //the dung -> mo cua
                        LCD_Cmd(0x01); 
												Delay_ms(10);
												LCD_Cmd(0x80);
												LCD_String("Access Granted");
                        RELAY = 1;      
                        Delay_ms(3000); 
                        RELAY = 0; Delay_ms(10);    
                    } else {
                        //the sai
                        LCD_Cmd(0x01); 
												Delay_ms(10);
												LCD_Cmd(0x80);
												LCD_String("Access Denied");
                        Delay_ms(2000);
                    }
                }
            }
            
            RI = 0; //
            
            //hienthi man hinh cho
            LCD_Cmd(0x01);
            if(Mode == 1) {
                Delay_ms(10);LCD_String("ADD MODE..."); 
                LCD_Cmd(0xC0); LCD_String("Swipe New Tag"); 
            } else {
								Delay_ms(10);
                LCD_String("RFID ACCESS"); 
                LCD_Cmd(0xC0); LCD_String("Swipe Tag.."); 
            }
        }
    }
}

void Delay_ms(unsigned int t) {
    unsigned int x, y;
    for(x = 0; x < t; x++)
        for(y = 0; y < 120; y++);
}

void Open_Door() {
    LCD_Cmd(0x01);
		Delay_ms(10);
    LCD_String("  Door Opened"); 
    RELAY = 1;     
    Delay_ms(3000); 
    RELAY = 0;
}

//khoi tao UART
void UART_Init() {
    TMOD = 0x20;  // timer 1 mode 2 
    TH1 = 0xFD;   // 9600 baud - 11.0592MHz
    SCON = 0x50;  // mode 1, REN = 1
    TR1 = 1;      // bat dau dem timer1
}
//xu ly va nhan du lieu
void UART_Receive_Tag() {
    unsigned char i = 0;
    unsigned char temp;
    while(i < 12) { 
        while(RI == 0); // Cho phim
        temp = SBUF;
        RI = 0; 
        
        // Xu ly Backspace (0x08)
        if (temp == 0x08) { 
            if (i > 0) i--; 
            continue; 
        }

        // Loc ki tu va chuyen sang chu hoa
        if(temp >= 'a' && temp <= 'z') temp -= 32; 
        if ((temp >= '0' && temp <= '9') || (temp >= 'A' && temp <= 'Z')) {
            rfid_data[i] = temp;
            i++; 
        }
    }
}

unsigned char Check_Is_Master() {
    unsigned char i;
    for(i=0; i<12; i++) {
        if(rfid_data[i] != MASTER_CARD[i]) return 0;
    }
    return 1;

}
