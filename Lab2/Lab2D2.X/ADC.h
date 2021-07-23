/* 
 * File: ADC.h  
 * Author: Jose Alvarez
 * Comments:
 * Creado: 21 de Julio 2021
 * Revision history: 23 de julio 2021
 */

#ifndef XC_HEADER_TEMPLATE_F
#define	XC_HEADER_TEMPLATE_F

#include <xc.h> // include processor files - each processor file is guarded. 

void ADCconfig(uint8_t canal, uint8_t just);
/*configura el ADC y lee el valor del reloj para no violar los tiempos
 permite elegir el canal inicial y la justificacion, el cambio de 
 canal se puede hacer manual luego*/
void CONVhexa(uint8_t *valor, uint8_t *upper, uint8_t *lower);
/*convierte valores hexadecimal en sus dos nibbles, el alto y bajo*/
void CONVdec(uint8_t *lectura, float *equiv);
/*utilizado especificamente para convertir una lectura de ADC con la
 referencia utilizada, en este caso es de uso especifico para
 voltajes entre 0V y 5V*/
#endif	/* XC_HEADER_TEMPLATE_H */

