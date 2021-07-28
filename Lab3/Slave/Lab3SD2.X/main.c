/* 
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

#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************


void config(void);
//******************************************************************************
//  Rutina de interrupcion
//******************************************************************************
void __interrupt() interrupcion(void){

}

//******************************************************************************
//  loop principal
//******************************************************************************
void main(void) {
    config();
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
}