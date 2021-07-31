/* 
 * ESTE ES EL CODIGO DEL MASTER
 * File:   main.c
 * Author: Jose Alvarez (19392)
 * 
 * Creado 28 de Julio de 2021
 * 
 */
//******************************************************************************
//  Palabras de configurafcion e inclusion de librerias
//******************************************************************************
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SPI.h"
#include "ComSerial.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************
uint8_t val1,val2,UARTdat,UARTsend,UARTsend2;

void config(void);
//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    if(PIR1bits.RCIF){
        PORTA = RCREG;
        PIR1bits.RCIF = 0;
    }
}

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    config();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    while(1){
        PORTCbits.RC2 = 0;
        sendSPI('1');
        //PORTB = readSPI(); UARTsend = PORTB;
        UARTsend = readSPI();
        __delay_ms(10);
        
        sendSPI('2');
        //PORTD = readSPI(); UARTsend2 = PORTD;
        UARTsend2 = readSPI();
        __delay_ms(10);
        PORTCbits.RC2 = 1;
        //sendString("POT1: \n");
        sendhex(&UARTsend);
        sendString("\n");
        //sendString("\rPOT2: ");
        sendhex(&UARTsend2);
        sendString("\n");
        __delay_ms(250);
    }
}

//******************************************************************************
//  funciones secundarias
//******************************************************************************
void config(void){
//Configuracion de los puertos y tipos de entradas
    ANSEL =         0X00; 
    ANSELH =        0X00;
    TRISA =         0X00;
    TRISB =         0X00;
    //TRISC =         0X00;
    TRISCbits.TRISC2 = 0; //entrada del SS
    TRISCbits.TRISC6 = 0; //salida TX
    TRISCbits.TRISC7 = 1; //entrada RX
    TRISD =         0X00;
    TRISE =         0X00;
    PORTA =         0X00;
    PORTB =         0X00;
    PORTC =         0X00;
    PORTD =         0X00;
    PORTE =         0X00;
    
    //Configuracion del oscilador
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //configurar el UART
    configUART();
    
    //configuracion de interrupciones
    INTCONbits.GIE =    1; //permite interrupciones
    INTCONbits.PEIE =   1; //Permite INT perifericas
    PIE1bits.RCIE   = 1; //permite interrupciones de recepcion de datos
}
