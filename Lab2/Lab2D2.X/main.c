/* 
 * File:   main.c
 * Author: Jose Alvarez (19392)
 *
 * Created on 21 de julio de 2021, 08:25 PM
 * Edicion 23 de julio de 2021
 * 
 * Hardware:
 * Puerto A: 2 potenciometros en RA0 y RA1
 * Puerto C: pines RS y E de la LCD, pines para comunicacion serial
 * Puerto D: todos los pines de la LCD D0-D7
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
#include "LCDD2.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************
uint8_t pot1,pot2,UARTdat, UARTval; //valores de ADRESH de pots
float val1,val2; //valores decimales de los pots
unsigned char disp1[3];
unsigned char disp2[3];
unsigned char disp3[3];
void configuracion(void);
//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    
    if(PIR1bits.RCIF){
        UARTdat = RCREG; 
        if(UARTdat == '+')UARTval++; //si es + aumenta
        if(UARTdat == '-')UARTval--; //si es - disminuye
        PIR1bits.RCIF = 0;
    }
    
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
}

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    configuracion();
    initLCD();
    while(1){ 
        /*Se envian los valores via comunicacion serial*/
        sendString("POT1: \r");
        CONVdec(&pot1,&val1); //convierte la lectura en flotante
        sendfloat(val1); //envia un flotante
        sendString("POT2: \r");
        CONVdec(&pot2,&val2);
        sendfloat(val2);
        sendString("UART: \r");
        sendhex(&UARTval); //envia un hex, usando 3 caracteres
        sendString("\r\r\r\r\r"); 
        /*se convierten los valores a caracteres para la LCD*/
        floTochar(val1,&disp1);
        floTochar(val2,&disp2);
        hexTochar(UARTval,&disp3);
        /*se imprimen los valores e identificadores en la LCD*/
        cursorLCD(1,1);
        LCDstring("POT1  POT2  UART");
        cursorLCD(2,1);
        dispCHAR(disp1[0]+48);
        dispCHAR('.');
        dispCHAR(disp1[1]+48);
        dispCHAR(disp1[2]+48);
        dispCHAR('V');
        cursorLCD(2,7);
        dispCHAR(disp2[0]+48);
        dispCHAR('.');
        dispCHAR(disp2[1]+48);
        dispCHAR(disp2[2]+48);
        dispCHAR('V');
        cursorLCD(2,13);
        dispCHAR(disp3[2]+48);
        dispCHAR(disp3[1]+48);
        dispCHAR(disp3[0]+48);
        __delay_ms(10); //tiempo para poder visualizar
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
    PORTE =         0X00;
    
    //Configuracion del UART
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
