/*Codigo Proyecto 3
 * Digital 2
 * Jose Luis Alvarez Pineda
 *
 */

/*
 * ARCHIVOS UTILIZADOS PARA EL PROYECTO
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

/*
 * PROTOTIPOS DE LAS FUNCIONES UTILIZADAS
 */

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Reloj a 40MHz
}


