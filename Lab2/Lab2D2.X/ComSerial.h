
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */
 
#ifndef XC_HEADER_TEMPLATE_D
#define	XC_HEADER_TEMPLATE_D

#include <xc.h> // include processor files - each processor file is guarded.  

void configUART(void);
void send1dato(char dato);
void sendString(unsigned char *mensaje);
void sendfloat(const float valor);
void division(uint8_t conteo,uint8_t* un,uint8_t* dec);
void divisiondecimal(uint8_t conteo,uint8_t* un,uint8_t* dec,uint8_t* cent);
void sendhex(uint8_t *valor);
void floTochar(const float valor,unsigned char *conv);
void hexTochar(uint8_t valor,unsigned char *conv);

#endif	/* XC_HEADER_TEMPLATE_H */

