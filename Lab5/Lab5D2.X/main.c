/* 
 * LAB5: Pic y AdafruitIO
 * File:   main.c
 * Author: jalva
 *
 * Created on 11 de agosto de 2021, 09:43 AM
 */

//******************************************************************************
//  Palabras de configurafcion e inclusion de librerias
//******************************************************************************
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

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "UART.h"
#include "DHT11.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//******************************************************************************
//  variables y prototipos
//******************************************************************************
uint8_t contador;
uint8_t Temp1;
uint8_t dummyT1;
uint8_t Hum1;
uint8_t dummyHum1;
uint8_t CHECKSUM;

void config(void);
//******************************************************************************
//  interrupciones del programa
//******************************************************************************
void __interrupt() interrupcion(void){
    
    if(INTCONbits.RBIF && PORTBbits.RB0){
        contador ++;
        INTCONbits.RBIF = 0;
    }
    
    if(INTCONbits.RBIF && PORTBbits.RB1){
        contador --;
        INTCONbits.RBIF = 0;
    }
    
    INTCONbits.RBIF = 0;
    
    if(PIR1bits.RCIF){
        PORTD = RCREG; 
        PIR1bits.RCIF = 0;
    }
}

//******************************************************************************
//  funciones y loop principal
//******************************************************************************
void main(void){
    config();
    
    while(1){
        
        DHT11_START();
        if(DHT11_ALIVE()){
            DHT11_ReadData(&Hum1);
            DHT11_ReadData(&dummyHum1);
            DHT11_ReadData(&Temp1);
            DHT11_ReadData(&dummyT1);
            DHT11_ReadData(&CHECKSUM);
            
            if(CHECKSUM == ((Hum1 + dummyHum1 + Temp1 + dummyT1) & 0XFF)){
                sendhex(&Hum1);sendString("\n");
                sendhex(&Temp1);sendString("\n");
            }
            else{
                sendString("ERROR");sendString("\n");
                sendString("ERROR");sendString("\n");
            }
        }
        sendhex(&contador);
        sendString("\n");
        __delay_ms(500);
        
    }
}

void config(void){
    ANSEL =     0X00;
    ANSELH =    0X00;
    TRISA =     0X00;
    TRISB =     0X03; //2 entradas para los botones
    TRISD =     0X00;
    PORTA =     0X00;
    PORTB =     0X00;
    PORTD =     0X00;
    
    //Configuracion del UART
    configUART();
    
    //Configuracion del oscilador
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //Configuracion de las interrupcioens
    INTCONbits.GIE = 1;
    INTCONbits.RBIF = 0; //apaga la bandera
    INTCONbits.RBIE = 1; //Interrupciones del IOCB
    INTCONbits.PEIE =   1; //Permite INT perifericas
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE   = 1; //permite interrupciones de recepcion de datos
    
    //configuracion del IOCB
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    //Configuracion del timmer1
    T1CON = 0X10; //periodo de 1MHz, ya que fuente es FOSC/4
    TMR1H = 0; //valores en 0 para el conteo de los periodos de pulso
    TMR1L = 0;
}