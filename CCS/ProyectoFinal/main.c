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
#include "driverlib/systick.h"
#include "driverlib/uart.h"

/*
 * PROTOTIPOS DE LAS FUNCIONES UTILIZADAS
 */
void GPIOConfig(void);
void UARTConfig(void);

/*
 * CONFIGURACION GENERAL Y LOOP PRINCIPAL
 */

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Reloj a 40MHz
    UARTConfig();
    GPIOConfig();
    UARTCharPut(UART0_BASE, 'P');
    UARTCharPut(UART0_BASE, 'r');
    UARTCharPut(UART0_BASE, 'u');
    while(1){
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0X02);
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0X00);
        }
    }
}

/*
 * FUNCIONES UTILIZADAS
 */

void UARTConfig(void){
    //Configuracion del UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Configurar reloj del UART y del puerto que lo utiliza
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1); //configurar los pines como UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //Configurar a 8 bits, 1 bit de parada y sin revisar paridad
}

void GPIOConfig(void){
    //Confiuracion de los puertos, dejarlo al final y no incluir los relojes de los puertos activados con otros modulos

    //Configuraciones en el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //habilitar el reloj del puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1); //led rojo como salida
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4); //habilitar el push 1 como entrada
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //pin4 indicado como un pull-up

}
