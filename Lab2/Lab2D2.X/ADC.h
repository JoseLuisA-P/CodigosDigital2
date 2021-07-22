/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
 
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
void ADCconfig(uint8_t canal, uint8_t just);
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower);
void CONVdec(uint8_t *lectura,uint8_t *ref,float *equiv);
#endif	/* XC_HEADER_TEMPLATE_H */

