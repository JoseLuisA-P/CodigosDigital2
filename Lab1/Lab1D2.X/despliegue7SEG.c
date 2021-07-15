#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "despliegue7SEG.h"

//******************************************************************************
//  configuracion del ADC
//******************************************************************************
void ADCconfig(uint8_t canal, uint8_t just){
    switch(OSCCONbits.IRCF){
        case 0b100:
            ADCON0bits.ADCS =   0b000; // FOSC/0 para no violar tiempos
            break;
        case 0b110:
            ADCON0bits.ADCS =   0b001; // FOSC/8 para no violar tiempos
            break;
        case 0b111:
            ADCON0bits.ADCS =   0b010; // FOSC/32 para no violar tiempos
            break;
        default:
            ADCON0bits.ADCS =   0b11; // FRC para no violar tiempos
            break;
    }
    ADCON0bits.CHS =    canal; //selecciona el canal acorde al valor
    ADCON0bits.GO =     0b0;
    ADCON0bits.ADON =   0b1;
    ADCON1bits.ADFM =   just; // 0--IZQUIERDA; 1--DERECHA
    ADCON1bits.VCFG1 =  0b0; //referencias a alimentacion del PIC
    ADCON1bits.VCFG0 =  0b0;
}
//******************************************************************************
//  Conversion de datos
//******************************************************************************
void CONVhexa(uint8_t valor, uint8_t *upper, uint8_t *lower){
    //utilizada para convertir valor en 2 variables hexadecimales
    //obtiene un valor que se desea transformar y devuelve los 2
    //valores hexadecimales equivalentes, superior e inferior
    
    uint8_t temp; //variable temporal para la separacion de datos
    temp = valor;
    *lower = (valor & 0x0F);
    temp = temp>>4;
    *upper = (temp & 0x0F);
}

//******************************************************************************
//  Despliegue de datos
//******************************************************************************
uint8_t Seg7EQ(uint8_t dato){
    //utilizado para regresar el equivalente para desplegar el dato en
    //un display de 7 segmentos.
    switch(dato){    //depende el valor, regresa el equivalente para el 7SEG
        case 0:
        return 0b00111111;	break;//0
        case 1:
        return 0b00000110; break;//1
        case 2:
        return 0b01011011;	break;//2
        case 3:
        return 0b01001111;	break;//3
        case 4:
        return 0b01100110;	break;//4
        case 5:
        return 0b01101101;	break;//5
        case 6:
        return 0b01111101;	break;//6
        case 7:
        return 0b00000111;	break;//7
        case 8:
        return 0b01111111;	break;//8
        case 9:
        return 0b01100111;	break;//9   
        case 10:
        return 0b01110111;	break;//A   
        case 11:
        return 0b01111100;	break;//B   
        case 12:
        return 0b00111001;	break;//C   
        case 13:
        return 0b01011110;	break;//D   
        case 14:
        return 0b01111001;	break;//E   
        case 15:
        return 0b01110001;	break;//F   
        default:
        return 0b00111111;	break;//0
    }
}