
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
 
#ifndef SPI_H
#define	SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  
typedef enum 
{
    SPI_MASTER_OSC_DIV4  = 0b00100000,
    SPI_MASTER_OSC_DIV16 = 0b00100001,
    SPI_MASTER_OSC_DIV64 = 0b00100010,
    SPI_MASTER_TMR2      = 0b00100011,
    SPI_SLAVE_SS_EN      = 0b00100100,
    SPI_SLAVE_SS_DIS     = 0b00100101
}Spi_Type;

typedef enum {
    
    CLOCK_HIGH = 0X10,
    CLOCK_LOW = 0X00
    
}ClockType;

typedef enum{

    FLANCO_CAMBIO_REPOSO = 0B01000000,
    FLANCO_REPOSO_CAMBIO = 0B01000000
    
}TipoFlanco;

typedef enum{

    MUESTREO_FIN = 0X80,
    MUESTREO_MITAD = 0X00
    
}Muestreo;

void configSPI(Spi_Type mode,ClockType flanco,TipoFlanco cambio,Muestreo punto);
void SPIsend (char dat);
char SPIread(void);

#endif	/* XC_HEADER_TEMPLATE_H */

