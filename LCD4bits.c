//
//
//    C library file for type HD44780 LCD driver for Microchip Pic family
//    
//    
//
//
//     Author: Florian CASSOL
//     Institut Universitaire de Technologie
//     Département Génie Electrique & Informatique Industrielle
//     Brest  -  France
//		florian.cassol@univ-brest.fr
//		2011/04/01

#include <xc.h>
#include <string.h>
#include "LCD4bits_setup.h"
#include "LCD4bits.h"




void  EN()
{
LCD_E = 0;
__delay_ms(1);
LCD_E = 1;
__delay_ms(1);
LCD_E = 0;
}




void  Sdata(char Data)
{
LCD_d4 = Data;
LCD_d5 = Data >> 1 ;
LCD_d6 = Data >> 2 ;
LCD_d7 = Data >> 3 ;
EN();
__delay_ms(1);
}





void  lcd_setup(char N)
{
volatile char data;
if (N>=1) {N=1;}

__delay_ms(45);   // not really necessary except if LCD power is driven by the PIC
LCD_RS = 0;
__delay_ms(5);
Sdata(3);
__delay_ms(5);
Sdata(3);
__delay_ms(1);
Sdata(3);
Sdata(2);
Sdata(2);				// function set
data= N*8;		// place 8 in data if display is more than 1 line format
Sdata(data);
Sdata(0);				// display off
Sdata(8);
Sdata(0);				// display on
Sdata(12);
Sdata(0);				// 
Sdata(1);
}





void  lcd_printChar(char Letter)
{
char DH;
char DL;
LCD_RS = 1;
__delay_ms(1);
DL = Letter & 0b00001111;
DH = Letter >> 4;
Sdata(DH);
Sdata(DL);
}




void lcd_printDigit(char nber)
{
lcd_printChar(nber+48);
}



void lcd_printNum(signed short num)
{
volatile unsigned short r;
volatile char n;
volatile char loop;
volatile char m;
m=0;
if(num<0){num=-num;lcd_printChar(45);}  //
n = num / 10000;
if (n) {lcd_printDigit(n);m=1;}  //displays digit only if different than 0
r = n * 10000;
num = num - r;

n = num / 1000;
if (n | m) {lcd_printDigit(n);m=1;}  //displays digit only if different than a first 0
r = n * 1000;
num = num - r;

n = num / 100;
if (n | m) {lcd_printDigit(n);m=1;}  //displays digit only if different than a first 0
r = n * 100;
num = num - r;

n = num / 10;
if (n | m) {lcd_printDigit(n);m=1;}  //displays digit only if different than a first 0
r = n * 10;
num = num - r;

lcd_printDigit(num);     //Displays the last digit, even it's a 0

}




void  lcd_print(const char* text)
{
volatile char Long,loop,c;
Long = strlen(text);

   for(loop=0;loop<Long;loop++) 
	{
	c = text[loop];
	lcd_printChar(c);
	} 
}




void lcd_GotoXY(char X, char Y)
{
char DH;
char DL;
char RamAdress;
RamAdress = 0x80 + DisplayRAM[Y] + X;
LCD_RS = 0;
__delay_ms(1);
DL = RamAdress & 0b00001111;
DH = RamAdress >> 4;
Sdata(DH);
Sdata(DL);
}





void lcd_Clr(void)
{

LCD_RS = 0;
__delay_ms(1);
Sdata(0);
__delay_ms(1);
Sdata(1);
__delay_ms(1);
Sdata(0);
__delay_ms(1);
Sdata(2);
__delay_ms(1);   
}


void lcd_CursorON(void)
{
LCD_RS = 0;
Sdata(0);				// display on with cursor
Sdata(14);
}

void lcd_CursorOFF(void)
{
LCD_RS = 0;
Sdata(0);				// display on without cursor
Sdata(12);
}
