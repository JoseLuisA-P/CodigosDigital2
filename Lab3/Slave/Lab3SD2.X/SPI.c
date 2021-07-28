#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SPI.h"

//******************************************************************************
//  configuracion del SPI
//******************************************************************************
void configSPI(Spi_Type mode,ClockType flanco,TipoFlanco cambio,Muestreo punto){
    if(mode & 0X04){ //modo esclavo
        
        SSPSTAT = cambio;
        
        TRISAbits.TRISA0 = 1; //entrada del SS
        TRISCbits.TRISC4 = 1;
        TRISCbits.TRISC5 = 0;
        TRISCbits.TRISC3 = 1;
    }
    else{       //modo maestro
        
        SSPSTAT = cambio | punto;
        
        TRISAbits.TRISA5 = 0; //salida del SS
        TRISCbits.TRISC4 = 1;
        TRISCbits.TRISC5 = 0;
        TRISCbits.TRISC3 = 0;
    }
    
    SSPCON = mode | flanco; //suma los bits de config
}


//******************************************************************************
//  funciones del SPI
//******************************************************************************
void SPIsend (char dat){  //enviar un dato por el SPI
    SSPBUF = dat;
}

char SPIread(void){
    while(!SSPSTATbits.BF);
    return (SSPBUF);  
}