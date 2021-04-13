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
//   Author:            Mathys LE ROUX, ...
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
#include <htc.h>
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

//définitions des variables

char dixième,sec,min,heure;

//Déclaration des prototypes des fonctions

//--------------------------------------------------------------
// Programme d'initialisation
//--------------------------------------------------------------
void init(void)
{
OSCCON=0b01110111; //Oscillateur interne réglé sur 8Mhz
TRISA= 0b00001001; // Port A mis en entrée AN0 est en entrée et le reste en sortie
TRISB= 0b00011100; // Port B en sortie
TRISC= 0b00000000; // Port C en sortie,

ANSEL= 0b00000001;	// AN0 est en entrée digitale
ANSELH=0b00000000;

WPUB=  0b00011100;		// config individuelle des R pull Up
nRBPU=0;			//Résistances de pull up ENABLE

CCP1CON=0;	//désactive comparateur et module pwm
CCP2CON=0;	//désactive comparateur et module pwm

SSPEN=0;

ADCON0=0b10000001;
ADCON1=0b10010000;

INTCON=0b01000000;      // Active les interruption, sur les périphérique
PIE1=  0b00000001;        // Active interruption sur overflow du timer 1
PIE2=  0b00000000;        // Désactive toute les autres interruptions
T1CON= 0b00001011;       // Timer1 activer, prescaler reglé sur 1:1, EXTERNAL, sur front montant, oscillateur on

}

void interrupt prog(){
if(TMR1IF==1)
{
    sec++;
    TMR1H= 0b01111111; // 8 bits de poids forts de la valeur de préchargement
    TMR1L= 0b11111111; // 8 bits de poids faibles de la valeur de préchargement
    TMR1IF=0;
    




}

int main()
{
  while(1)
      {

          if(sec>=60){
              min++;
              sec=0;
              }
          if(min>=60){
              heure++;
              min=0;
              }
          if(heure>=24){
              heure=0;
              sec=0;
              min=0;
              }
          Afficher_temp(5,0);
          Afficher_heure(4,1);
          lcd_GotoXY(15,1);
          lcd_print(">");

           if(droite==appuye){
              page_reg=1;
              lcd_Clr();
           }



      }
}
