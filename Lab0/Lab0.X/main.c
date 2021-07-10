/*Archivo: main.c
 *Autor: Jose Luis Alvarez Pineda (19392)
 *
 * Creado: 9 de Julio de 2021
 * Modificado: 9 de Julio de 2021
 * 
 * General:
 * 
 * Hardware:
 * 
 * Software:
 * 
 */

/*
 *               Palabras de configuracion e inclusion de librerias
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*                      
 *                                Variables
 */

union Carrera{ //manejar las condiciones de la carrera
    struct{
        unsigned comienzo: 1; //indicar si comienza la carrera
    };
}EstadoCarrera;

uint8_t conteo; //para contar 1 segundo con timmer 1
uint8_t semaforo; //conteo del semaforo

/*
 *                          Funciones y prototipos
 */
void configuracion(void);

void __interrupt() intVec(void){//Funcion de interrupcion
    
    if(PIR1bits.TMR1IF){
        conteo ++;
        TMR1H = 0B00111100;     //para overflow cada 0.1seg
        TMR1L = 0B10101111;
        PIR1bits.TMR1IF = 0;
    }
    
    if(INTCONbits.RBIF && PORTBbits.RB0){
        EstadoCarrera.comienzo = 0; //evita jugar
        if(!T1CONbits.TMR1ON) semaforo++;
        T1CONbits.TMR1ON = 1;   //activar el timmer 1
    }
    
    if(INTCONbits.RBIF && PORTBbits.RB1){
        if(EstadoCarrera.comienzo)PORTC<<1;
    }
    
    if(INTCONbits.RBIF && PORTBbits.RB2){
        if(EstadoCarrera.comienzo)PORTD<<1;
    }
    
    INTCONbits.RBIF = 0;
    
}

void main(void){//Configuraciones y loop principal
    configuracion();
    
    while(1){//loop principal-continuo
        if(conteo >= 10){
            semaforo++;
            conteo = 0;
        }
        
        switch(semaforo){
            case 1:
                PORTA = 0b01001111;
                PORTE = 0b001;
                break;
            case 2:
                PORTA = 0b01011011;
                PORTE = 0b010;
                break;
            case 3:
                PORTA = 0b00000110;
                PORTE = 0b010;
                break;
            case 4:
                PORTA = 0b00111111;
                PORTE = 0b100;
                EstadoCarrera.comienzo = 1; //permite jugar
                T1CONbits.TMR1ON = 0; //apaga el timmer
                TMR1H = 0B00111100;     //para overflow cada 0.1seg
                TMR1L = 0B10101111;
                semaforo = 0;
                PORTC = 1; //Comienza el valor en la carrera
                PORTD = 1;
                break;
            default:
                break;
        }
        
    }
}

void configuracion(void){//Configuraciones del uC
    //Configuracion general de pines
    ANSEL =         0X00;
    ANSELH =        0X00;
    TRISA =         0X00;
    TRISB =         0X07; //primeros 3 pines como entradas
    TRISC =         0X00;
    TRISD =         0X00;
    TRISE =         0X00;
    PORTA =         0X00;
    PORTB =         0X00;
    PORTC =         0X00;
    PORTD =         0X00;
    PORTE =         0X00;
    
    //iniciando valores
    conteo = 0;
    
    //Configuracion del reloj
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //Configuracion del TIMER 1
    T1CONbits.T1CKPS = 0B10;    //preescalador de 4
    TMR1H = 0B00111100;     //para overflow cada 0.1seg
    TMR1L = 0B10101111;
    T1CONbits.TMR1ON = 0; //mantenerlo apagado
    
    //Configuracion del IOCB
    IOCBbits.IOCB0 = 1;     //detecta cambios en RB0
    IOCBbits.IOCB1 = 1;     //detecta cambios en RB1
    IOCBbits.IOCB2 = 1;     //detecta cambios en RB2
    
    //Configuracion de interrupciones
    PIE1bits.TMR1IE = 1;    //habilita interrupcion timmer 1
    PIR1bits.TMR1IF = 0;    //apaga bandera timmer 1
    INTCONbits.PEIE = 1;    //habilita las interrpciones perifericas
    INTCONbits.RBIF = 0;    //apaga bandera IOCB
    INTCONbits.RBIE = 1;    //habilita IOCB
    INTCONbits.GIE = 1;     //habilitadas interrupcioens globales
       
}