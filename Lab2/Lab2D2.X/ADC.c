#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ADC.h"

//******************************************************************************
//  configuracion del ADC
//******************************************************************************
void ADCconfig(uint8_t canal, uint8_t just){
    //utilizado para configurar el ADC en el PIC
    //se ajusta para no violar los tiempos y usar un estandar de 
    //justificado a la derecha y usar el CH0 como via de lectura
    //no comienza a leer hasta que se le indica en codigo
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
    ADCON1bits.VCFG0 =  0b0; //si se usa otra alimentacion, cambiarlo en codigo
}

//******************************************************************************
//  Conversion de datos
//******************************************************************************
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower){
    //utilizada para convertir valor en 2 variables hexadecimales
    //obtiene un valor que se desea transformar y devuelve los 2
    //valores hexadecimales equivalentes, superior e inferior
    
    uint8_t temp; //variable temporal para la separacion de datos
    temp = *valor;
    *lower = (*valor & 0x0F);//copia el nibble bajo
    temp = temp>>4;
    *upper = (temp & 0x0F);//copia el nibble alto
}

void CONVdec(uint8_t *lectura,float *equiv){
    //convierte un valor leido por el ADC en un float basado en 5V
    *equiv = (float)(0.01961)*(*lectura);
}