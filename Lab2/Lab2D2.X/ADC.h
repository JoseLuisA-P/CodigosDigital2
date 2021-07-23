/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
 
#ifndef XC_HEADER_TEMPLATE_F
#define	XC_HEADER_TEMPLATE_F

#include <xc.h> // include processor files - each processor file is guarded. 

void ADCconfig(uint8_t canal, uint8_t just);
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower);
void CONVdec(uint8_t *lectura, float *equiv);

#endif	/* XC_HEADER_TEMPLATE_H */

