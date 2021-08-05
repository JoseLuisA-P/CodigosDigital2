/* 
 * ESTE ES EL CODIGO DEL MASTER
 * File:   main.c
 * Author: Jose Alvarez (19392)
 * 
 * Creado 4 de Agosto de 2021
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
#include "I2C.h"
#include "LCDD2.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  Variables y prototipos de funciones
//******************************************************************************
uint8_t DatS1;
float lect1;
unsigned char disp1[3];

void config(void);
void floTochar(const float valor,unsigned char *conv);
void division(uint8_t conteo,uint8_t* un,uint8_t* dec);
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
    initLCD();
    while(1){  
        MasterStart_I2C();//Inicia la comunicacion
        MasterSend_I2C(0X21);//direccion del que se va a leer
        MasterReceive_I2C(&DatS1);
        MasterStop_I2C();//termina la comunicacion
        __delay_ms(100);
        //PORTB = DatS1; //observar que si funciona
        lect1 = (float)(0.01961)*(DatS1);//pasar el valor leido a flotante
        __delay_ms(1);
        //despliegue de valores luego de la lectura
        floTochar(lect1,&disp1);
        cursorLCD(1,1);
        LCDstring("S1:   S2:   S3:");
        cursorLCD(2,1);
        dispCHAR(disp1[0]+48);
        dispCHAR('.');
        dispCHAR(disp1[1]+48);
        dispCHAR(disp1[2]+48);
        dispCHAR('V');
    }
}

//******************************************************************************
//  funciones secundarias
//******************************************************************************
void config(void){
    ANSEL =     0X00;
    ANSELH =    0X00;
    TRISA =     0X00;
    TRISB =     0X00;
    TRISD =     0X00;
    TRISE =     0X00;
    PORTA =     0X00;
    PORTB =     0X00;
    PORTD =     0X00;
    PORTE =     0X00;
    
    MasterInit_I2C(100000); //inicializado a 100KHz, frec estandar
}

void floTochar(const float valor,unsigned char *conv){
    uint8_t entero;
    uint8_t decimal;
    float temp;
    unsigned char digdecimal[2];
    /*Toma un valor tipo float y pasa su entero a una variable, el valor 
     * restante lo opera para pasar las decimas a enteros y luego separarlos
     * en 2 datos, para poder representarlos luego en el puerto serial enviando
     * un caracter a la vez.
     */
    entero = valor;
    digdecimal[2] = entero;
    temp = valor-(float)entero;
    decimal = (temp*100);
    division(decimal,&digdecimal[0],&digdecimal[1]);
    conv[0] = entero;
    conv[1] = digdecimal[1];
    conv[2] = digdecimal[0];
}

//Divisiones utilizadas para separar los valores en decimas, centesimas y 
//unidades
void division(uint8_t conteo,uint8_t* un,uint8_t* dec){
    uint8_t div = conteo;
    *un =   0;
    *dec =  0;
    //modifica los valores de las variables asignadas de forma inmediata
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10);
    }
    
    *un = div;
}