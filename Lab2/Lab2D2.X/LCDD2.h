
/* 
 * File: LCDD2.h   
 * Author: Jose Alvarez
 * Comments:
 * Creado: 21 de Julio 2021
 * Revision history: 21 de Julio 2021
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

void initLCD(void);
void dispCHAR(unsigned char b);
void cursorLCD(uint8_t pos);
void comandoLCD(uint8_t cmd);

#endif	/* XC_HEADER_TEMPLATE_H */

