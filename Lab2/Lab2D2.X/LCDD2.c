#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "LCDD2.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  definicion datos usables
//******************************************************************************
#define LCDport PORTD
#define RS RC0
#define EN RC1

//******************************************************************************
//  funciones de manejo
//******************************************************************************

void initLCD(void){
    RS =    0;
    LCDport = 0X00;
    __delay_ms(20);
    //LCDport = 0X30;
    comandoLCD(0X30);
    __delay_ms(5);
    //LCDport = 0X30;
    comandoLCD(0X30);
    __delay_ms(11);
    //LCDport = 0X30;
    comandoLCD(0X30);
    //LCDport = 0X38; // 2 Lineas y 5x8
    comandoLCD(0X38);
    //LCDport = 0X08; //display off
    comandoLCD(0X08);
    //LCDport = 0X01; // display clear
    comandoLCD(0X01);
    //LCDport = 0X06; //corrimiento a la derecha y no mueve el display
    comandoLCD(0X06);
    
}

//******************************************************************************
//  Despliegue de datos
//******************************************************************************

void dispCHAR(unsigned char b){
    //enviar un dato en la LCD
    RS = 1;             //se envia dato
    LCDport = b;        //se coloca el dato en el puerto
    EN = 1;
    __delay_ms(4);     //cambio en EN para la recepcion del dato
    EN = 0;

}

void cursorLCD(uint8_t pos){
    RS = 0;
    LCDport = pos;
    EN = 1;
    __delay_ms(4);     //cambio en EN para la recepcion del dato
    EN = 0;
}

void comandoLCD(uint8_t cmd){
    RS = 0;
    LCDport = cmd;
    EN = 1;
    __delay_ms(4);     //cambio en EN para la recepcion del dato
    EN = 0;
}