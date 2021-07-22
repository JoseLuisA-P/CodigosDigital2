/* 
 * File:   main.c
 * Author: Jose Alvarez (19392)
 *
 * Created on 21 de julio de 2021, 08:25 PM
 * Edicion 21 de julio de 2021
 * 
 * Hardware:
 * 
 * Software:
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
#include "ComSerial.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************
uint8_t pot1,pot2,UARTdat, UARTval; //valores de ADRESH de pots
float val1,val2; //valores decimales de los pots
unsigned char disp1[3];
void configuracion(void);
//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    
    if(PIR1bits.RCIF){
        UARTdat = RCREG; 
        if(UARTdat == 0X2B)UARTval++; //si es + aumenta
        if(UARTdat == 0X2D)UARTval--; //si es - disminuye
        PIR1bits.RCIF = 0;
    }
    
    if(PIR1bits.ADIF){
        ADCON0bits.CHS0 = ~ADCON0bits.CHS0;
        PIR1bits.ADIF = 0;
        if(ADCON0bits.CHS0)pot1 = ADRESH;
        else pot2 = ADRESH;
    }
    
    if(INTCONbits.T0IF){
        if(!ADCON0bits.GO)ADCON0bits.GO = 1;
        INTCONbits.T0IF = 0;
    }
}

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    configuracion();
    
    while(1){
        sendString("POT1: \r");
        CONVdec(&pot1,&val1);
        sendfloat(val1);
        sendString("POT2: \r");
        CONVdec(&pot2,&val2);
        sendfloat(val2);
        sendString("UART: \r");
        sendhex(UARTval);
        sendString("\r");
        sendString("\r");
        sendString("\r");
        sendString("\r");
        __delay_ms(1000);
    }
}

//******************************************************************************
//  funciones secundarias
//******************************************************************************
void configuracion(void){
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
    PORTE =         0X01;
    
    configUART();
    
    //Configuracion del oscilador
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //configuracion de interrupciones
    INTCONbits.GIE =    1; //permite interrupciones
    INTCONbits.PEIE =   1; //Permite INT perifericas
    INTCONbits.T0IF =   0; //Apaga bandera timer0
    INTCONbits.T0IE =   1; //permite interrupciones timmer0
    PIE1bits.RCIE   = 1; //permite interrupciones de recepcion de datos
    
    //configuracion timmer0
    OPTION_REGbits.T0CS = 0;    //Timmer 0 a FOSC y Prescalador asignado
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS2  = 1;    //valor del prescalador
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 0;
    
    //configuracion timmer1
    
    //configuracion del ADC
    ADCconfig(0,0); //configurado comienza en el canal 0 y just Izquierda
    
    PIR1bits.ADIF = 0; //interrupciones del ADC
    PIE1bits.ADIE = 1;
    
}
