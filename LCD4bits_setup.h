//
//
//    User's header file for type HD44780 LCD driver for Microchip Pic family
//    
//    
//
//
//     Author: Florian CASSOL
//     Institut Universitaire de Technologie
//     Département Génie Electrique & Informatique Industrielle
//     Brest  -  France
//		florian.cassol@univ-brest.fr
//

// Must be configured by user according schematic, no rules !
// configuration of signals connections between the PIC and the display

#define LCD_d4 RA4
#define LCD_d5 RA5
#define LCD_d6 RA7
#define LCD_d7 RA6
#define LCD_RS RB1
#define LCD_E RB0



// define hex DD Ram adress of the HD44780 drived type display for the
// beginning of each line (L1 is 0)

char DisplayRAM[] = {0x00,0x40,0x10,0x50};

// indique au compilateur la fréquence d'horloge à laquelle fonctionne le µC (dépand du quartz) 
#define _XTAL_FREQ 8000000  // 8Mhz configuration de la carte

