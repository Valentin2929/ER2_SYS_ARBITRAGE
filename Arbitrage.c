//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                                                     
//                     Software License Agreement                      
//                                                                     
//
//                                                                     
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//   Filename:           Arbitrage.c
//   Dependencies:       
//                       
//   target:          	PIC16F886
//   Compiler:			XC8 V1.35 
//   Date:              13/04/2021
//   File Version:      0.1
//
//   Author:            Valentin BIHAN, ...
//   Company:           IUT Brest, département GEII
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//   A QUOI CA SERT ?
//      Permettre l'arbitrage d'une compétition de robot suiveur de lignes
//    
//   		
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <xc.h>
#include <stdio.h>

#include "LCD4bits.h"
#include "LCD4bits_setup.h"

//--------------------------------------------------------------
// 		instruction à destination du compilateur pour la mise en place 
//    automatique des bits de configuration dans l'environnement MPLAB (menu configure, configuration bits)
//
//--------------------------------------------------------------

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)





// indique au compilateur la fréquence d'horloge à laquelle fonctionne le µC (dépand du quartz) 
#define _XTAL_FREQ 8000000  // 8Mhz configuration de la carte

// pré programmation de l'eeprom


// définition des constantes

#define gauche RB2	//bouton poussoir S1
#define valid RB3	//bouton poussoir S2
#define droite RB4	//bouton poussoir S3

#define	appuye 0	// état logique pour un appui sur un bouton
#define relache 1	// état logique pour un bouton relâché
#define Commande RB5	//sortie de commande 
#define buzz RC3 // sortie buzzer
#define flash RC4 //


//définitions des variables

char centieme,sec,min;
int x,y;

char old_state_sec,old_state_min;

//Déclaration des prototypes des fonctions


void init(void);

void interrupt prog();

int afficher_temps(int x,int y);
    
//************************************************************************
//      Main
//************************************************************************


/*
 lancer le timer au tout et sans l'interuption et précharger au start du chrono pour "chauffer" le timer
 
 
 */
int main()
{ 
    unsigned char start,sure;
    x=5;
    y=0;
    init();
    lcd_setup(1);
    lcd_Clr();
    lcd_print("IUT: Module ER2");
    lcd_GotoXY(3,1);
    lcd_print("Arbitrage");
    __delay_ms(800);
    lcd_Clr();
    buzz=0;
    flash=0;
    
    
    
        
	while(1)
    {
    T1CON= 0b00000000;          // desactive le TMR1
    GIE=0;		
    lcd_Clr();
    afficher_temps(5,0);
    lcd_GotoXY(8,1);
    lcd_print(">");
    lcd_GotoXY(0,1);
    lcd_print("R");
    
    __delay_ms(100);
    while(valid == relache){
        
        if(gauche==appuye)
        {
            sure=0;
            lcd_Clr();
            while(sure==0)
            {
                
                lcd_GotoXY(0,0);
                lcd_print("Confirmer la RAZ");
                lcd_GotoXY(7,1);
                lcd_print("Oui");
                lcd_GotoXY(13,1);
                lcd_print("Non");
                if(valid==appuye)
                {
                    lcd_Clr();
                    min=0;
                    sec=0;
                    TMR1=32700; // 8 bits de poids faibles de la valeur de préchargement 
                    sure=1;
                    
                }
                if(droite==appuye)
                {
                    
                    lcd_Clr();
                    sure=1;
                    
                }
            }
            afficher_temps(5,0); 
            lcd_GotoXY(8,1);
            lcd_print(">");
            lcd_GotoXY(0,1);
            lcd_print("R");
            __delay_ms(200);
        }
    }
    
    if(min==0 && sec==0 && centieme==0){
    
    __delay_ms(1000);
        lcd_Clr();
        lcd_GotoXY(8,0);
        lcd_print("3");
        
        __delay_ms(1000);
        lcd_Clr();
        lcd_GotoXY(8,0);
        lcd_print("2");
        
        __delay_ms(1000);
        lcd_Clr();
        lcd_GotoXY(8,0);
        lcd_print("1");
        
        __delay_ms(1000);
        
        
        buzz=1;
        flash=0;
        __delay_ms(500);
        buzz=0;
        flash=1;
    }
    
    
    T1CON= 0b00001011;    // active le TMR1
    GIE=1;
    TMR1H= 0b01111111; // 8 bits de poids forts de la valeur de préchargement
    TMR1L= 0b11111111; // 8 bits de poids faibles de la valeur de préchargement
    lcd_Clr();
    start=1;
    afficher_temps(5,0);
    while(start==1)
    {   
	
    
    
    lcd_GotoXY(14,1);
	lcd_print("||");
    old_state_min=min;
    old_state_sec=sec;
    
    if(droite==appuye)
    {
        start=0;
    }
    if(sec>=60)
    {
        sec=0;
        min++;
    }
    afficher_temps(5,0);
    
    
 
    }
    }
}
//****************************************************************
//      Définition des fonctions
//****************************************************************

void init(void)
{
OSCCON=0b01110111; //Oscillateur interne réglé sur 8Mhz
TRISA= 0b00001001; // Port A mis en entrée AN0 est en entrée et le reste en sortie
TRISB= 0b00011100; // Port B en sortie
TRISC= 0b00000000; // Port C en sortie,

ANSEL= 0b00000001;	// AN0 est en entrée digitale
ANSELH=0b00000000;

WPUB= 0b00011100;		// config individuelle des R pull Up
nRBPU=0;			//Résistances de pull up ENABLE

CCP1CON=0;	//désactive comparateur et module pwm
CCP2CON=0;	//désactive comparateur et module pwm

SSPEN=0;

ADCON0=0b10000001;
ADCON1=0b10010000;

INTCON= 0b01000000;      // Active les interruption, sur les périphérique
PIE1=   0b00000001;        // Active interruption sur overflow du timer 1
PIE2=   0b00000000;        // Désactive toute les autres interruptions
//T1CON= 0b00001011;       // Timer1 activer, prescaler reglé sur 1:1, EXTERNAL, sur front montant, oscillateur on

//GIE=1;
}

void interrupt prog()
{
if(TMR1IF==1)
{
    sec++;
    TMR1H= 0b01111111; // 8 bits de poids forts de la valeur de préchargement
    TMR1L= 0b11111111; // 8 bits de poids faibles de la valeur de préchargement
    TMR1IF=0;
    
}

}

int afficher_temps(int x,int y)
{
    
    lcd_GotoXY(x+2,y);
    lcd_print(":");
    lcd_GotoXY(x+5,y);
    lcd_print(":");
    lcd_GotoXY(x+8,y);                                                                                                                  
    lcd_print(" ");
    
    lcd_GotoXY(x,y);
    
    if(min<10)
    {
        lcd_print("0");
    }
    lcd_printNum(min);
    lcd_GotoXY(x+3,y);
    
    if(sec<10)
    {
        lcd_print("0");
    }
    
    lcd_printNum(sec);
     
    lcd_GotoXY(x+6,y);
   
        lcd_printNum((TMR1)%100);
}
