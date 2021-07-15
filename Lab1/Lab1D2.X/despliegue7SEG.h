/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
void CONVhexa(uint8_t valor, uint8_t *upper, uint8_t *lower);
void ADCconfig(uint8_t canal, uint8_t just);
uint8_t Seg7EQ(uint8_t dato);
#endif	/* XC_HEADER_TEMPLATE_H */

