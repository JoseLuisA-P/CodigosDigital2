/* 
 * ESTE ES EL CODIGO DEL SLAVE
 * File:   main.c
 * Author: Jose Alvarez (19392)
 * 
 * Hardware:
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
#include "ADC.h"
#include "SPI.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************
uint8_t pot1,pot2;
unsigned char dato;

void config(void);
//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    if(PIR1bits.ADIF){
        ADCON0bits.CHS0 = ~ADCON0bits.CHS0; //cambio de canal por cada lectura
        PIR1bits.ADIF = 0;
        if(ADCON0bits.CHS0)pot1 = ADRESH; //asignando valores por lectura
        else pot2 = ADRESH;
    }
    
    if(INTCONbits.T0IF){
        if(!ADCON0bits.GO)ADCON0bits.GO = 1; //go de ADC con timer y no
        INTCONbits.T0IF = 0;                //automatico
    }
    
    if(PIR1bits.SSPIF){
        dato = SPIread();
        PORTAbits.RA5 = 1;
        if(dato == '1') SPIsend(pot1);       //envia dato POT1
        else if (dato == '2') SPIsend(pot2); //envia dato POT2
        else SPIsend('b'); //por mala comunicacion
        PORTAbits.RA5 = 0;
    }
    
}

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    config();
    configSPI(SPI_SLAVE_SS_EN,CLOCK_LOW,FLANCO_REPOSO_CAMBIO,MUESTREO_MITAD);
    while(1){

    }
}

//******************************************************************************
//  funciones secundarias
//******************************************************************************
void config(void){
//Configuracion de los puertos y tipos de entradas
    ANSEL =         0X03; //2 entradas analogica
    ANSELH =        0X00;
    TRISA =         0X03;//2 entradas analogica RA0 y RA1
    TRISB =         0X00;
    TRISC =         0X00;
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
    
    //configuracion de interrupciones
    INTCONbits.GIE =    1; //permite interrupciones
    INTCONbits.PEIE =   1; //Permite INT perifericas
    INTCONbits.T0IF =   0; //Apaga bandera timer0
    INTCONbits.T0IE =   1; //permite interrupciones timmer0
    PIR1bits.SSPIF = 0;     //borrar bandera SPI
    PIE1bits.SSPIE = 1;    // habilitar interrupcion SPI
    PIE1bits.RCIE   = 1; //permite interrupciones de recepcion de datos
    
    //configuracion timmer0
    OPTION_REGbits.T0CS = 0;    //Timmer 0 a FOSC y Prescalador asignado
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS2  = 1;    //valor del prescalador
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 0;
    
    //configuracion del ADC
    ADCconfig(0,0); //configurado comienza en el canal 0 y just Izquierda
    
    PIR1bits.ADIF = 0; //interrupciones del ADC
    PIE1bits.ADIE = 1;
    
}