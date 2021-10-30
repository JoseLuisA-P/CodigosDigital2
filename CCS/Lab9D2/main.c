/*Codigo Laboratorio 9
 * Digital 2
 * Jose Luis Alvarez Pineda
 *
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

//Variables de control
uint8_t deb1; //para el antirebote
uint8_t i; //para el ciclo for
uint32_t valTimer; //valor a cargar en el timer

//PROTOTIPOS
void delayMs(uint32_t milis);
void combinacionColores(void);

int main(void)
{   //configuracion de los perifericos y reloj de la placa
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Reloj a 40MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //reloj para el puerto F habilitado
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //pines del puerto F como salidas ROJO/AZUL/VERDE
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4); //Push 1 como entrada
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); //pin4 indicado como un pull-up

    //configuracion del timmer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //habilitar el reloj del timmer 0
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC); //configurar el Timer0 como periodico y down, de 32 bits
    TimerLoadSet(TIMER0_BASE,TIMER_BOTH,valTimer);
    IntEnable(INT_TIMER0B); // Habilita la interrupcion del timer0a
    TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT); //establece interrupcion por timeout en el timer 0
    IntMasterEnable(); //habilita las interrupciones globales
    TimerEnable(TIMER0_BASE,TIMER_BOTH); //para usar ambos timers0

    while(1){ //LOOP para ejecutar el programa

        //antirebote para el semaforo
        if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)){
            deb1 = 1;
        }

        if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) && deb1 == 1 ){
            deb1 = 0;
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
    //rutina para mostrar el movimiento del semaforo
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X08); //enciende el verde
    delayMs(2000);
    for(i=0; i< 9; i++){
        if(i%2 == 0){
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X08); //enciende el verde
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X00); //apaga las luces
        }
       delayMs(400);
    }
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X0A); //enciende el amarillo
    delayMs(2000);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3,0X02); //enciende el rojo
}
