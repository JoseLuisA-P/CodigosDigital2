/*Codigo Laboratorio 9
 * Digital 2
 * Jose Luis Alvarez Pineda
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

//Variables de control
bool deb1;

//PROTOTIPOS
void delayMs(uint32_t milis);
void combinacionColores(void);

int main(void)
{   //configuracion de los perifericos y reloj de la placa
    SysCtlClockSet(SYSCTL_SYSDIV_8 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Reloj a 25MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //reloj para el puerto F habilitado
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //pines del puerto F como salidas ROJO/AZUL/VERDE
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4); //Push 1 como entrada
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //pin4 indicado como un pull-up

    while(1){ //LOOP para ejecutar el programa
        if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)){
        combinacionColores();

        }
    }

}

//Funcion para hacer delays en mili-segundos
void delayMs(uint32_t milis){

    SysCtlDelay(milis * (SysCtlClockGet()/3000)); //3 ciclos de reloj por SysCtlDelay en assembler, la frecuencia dividida por 3000 da el valor en milis
                                                  //multiplicando por los milis da un valor entero aproximado
}

//rutina para mostrar cada color de forma individual PRELAB
void combinacionColores(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X02); //enciende el rojo
    delayMs(1000);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X0A); //enciende el amarillo
    delayMs(1000);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X08); //enciende el verde
    delayMs(1000);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X00);
}
