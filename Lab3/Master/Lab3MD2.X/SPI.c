/*
 * File            : spi.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : SPI Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * modificada 28 de Julio de 2021, adecuada para el uso del laboratorio
 */

#include "SPI.h"

void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) //detecta si es modo esclavo
    {
        SSPSTAT = sTransmitEdge; //coloca la forma en que detecta el flanco
        //si de estacionario a su cambio o si del cambio a estacionario
        TRISC3 = 1; //CLK como entrada
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge; //OR de los dos valores para
        //colocar el muestreo de los datos y el flanco de transmision
        TRISC3 = 0; //CLK como salida
    }
    
    SSPCON = sType | sClockIdle;//OR para colocar el rol del uC y el estado
    //estacionario del reloj
}

void sendSPI(char valor){
    SSPBUF = valor; //coloca el valor en el SSPBUF
}

char readSPI(void){
    while(!SSPSTATbits.BF); //espera a que se termine de colocar el dato del SR
    return(SSPBUF);  //regresa el valor que tiene el SSPBUF
}