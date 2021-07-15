/*Archivo: main.c
 *Autor: Jose Luis Alvarez Pineda (19392)
 *
 * Creado: 14 de Julio de 2021
 * Modificado: 15 de Julio de 2021
 * 
 * General: 
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "despliegue7SEG.h"

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************
uint8_t referencia; //referencia contrastada en el ADC
uint8_t uphex,lowhex,multi; //valores hex a desplegar
uint8_t tempo; //para realizar la conversion solo con el timmer0
void configuracion(void);
//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){
    
    if(INTCONbits.RBIF && PORTBbits.RB0){
        referencia++; //aumenta el valor de referencia
        INTCONbits.RBIF = 0;
    }
    
    if(INTCONbits.RBIF && PORTBbits.RB1){
        referencia--; //disminuye el valor de referencia
        INTCONbits.RBIF = 0;
    }
        INTCONbits.RBIF = 0;
        
    if(INTCONbits.T0IF){
        CONVhexa(ADRESH,&uphex,&lowhex); //obtener los valores a desplegar del 
        //ADC en formato hexadecimal
        if(!ADCON0bits.GO)ADCON0bits.GO = 1; //hacer la conversion acorde al T0
        tempo = 1;
        INTCONbits.T0IF = 0;
    }
         
}
//******************************************************************************
//  loop principal
//******************************************************************************
void main(void){
    configuracion();
    while(1){
        
        PORTD = referencia; //puertoD toma el valor de referencia
        
        if(ADRESH>referencia)PORTAbits.RA1 = 1; //comparador para ver
        else PORTAbits.RA1 = 0; // que no se supera el valor
        
        if(tempo){
            multi++; //para multiplexar los 2 valores
            if(multi>=2)multi = 0;
        switch(multi){
            case 0:
                PORTE = 0; //evitar el parpadeo en cambio de valor
                PORTC = Seg7EQ(uphex); //regresa el valor superior del hex
                PORTE = 0x01;
                break;
            case 1:
                PORTE = 0;
                PORTC = Seg7EQ(lowhex); //regresa el valor ingerior del hex
                PORTE = 0x02;
                break;
            default:
                break;
        }
        tempo = 0;
        }
            
    }
}
//******************************************************************************
//  funciones secundarias
//******************************************************************************
void configuracion(void){
    //Configuracion de los puertos y tipos de entradas
    ANSEL =         0X01; //1 entrada analogica
    ANSELH =        0X00;
    TRISA =         0X01;//1 entrada analogica RA0
    TRISB =         0X03;//2 entradas IOCB
    TRISC =         0X00;// todos como salidas y valor 0
    TRISD =         0X00;
    TRISE =         0X00;
    PORTA =         0X00;
    PORTB =         0X00;
    PORTC =         0X00;
    PORTD =         0X00;
    PORTE =         0X01;
    
    //Configuracion del oscilador
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //Configuracion del IOCB
    IOCBbits.IOCB0 = 1;     //detecta cambios en RB0
    IOCBbits.IOCB1 = 1;     //detecta cambios en RB1
    
    //Configuracion de interrupciones
    INTCONbits.RBIF = 0;    //apaga bandera IOCB
    INTCONbits.RBIE = 1;    //habilita IOCB
    INTCONbits.T0IF = 0;    //Interrupciones del timmer 0
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;     //habilitadas interrupcioens globales
    
    //configuracion timmer0
    OPTION_REGbits.T0CS = 0;    //Timmer 0 a FOSC y Prescalador asignado
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS2  = 1;    //valor del prescalador
    OPTION_REGbits.PS1  = 1;
    OPTION_REGbits.PS0  = 0;
    
    //Configuracion del ADC
    ADCconfig(0,0); //seleccion de canal 0 y justificado a la izquierda
}
