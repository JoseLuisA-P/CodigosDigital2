/* 
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
#include "DHT11.h"
#include "LCDD2.h"
#include "I2C.h"
#define _XTAL_FREQ 8000000 //utilizado para los delays

//variables utilizadas
uint8_t Temp1;
uint8_t dummyT1;
uint8_t Hum1;
uint8_t dummyHum1;
uint8_t CHECKSUM;
uint8_t TempENT, TempDEC;
int temp;
float digTemp;
char temperatura[] = "TEMP = 00.0 C ";
char humedadR[] = "RH   = 00.0 % ";
unsigned char TEMPdig[6];

//prototipos
void floToChar(const float valor, unsigned char *salida);
void divisiondecimal(uint8_t conteo,uint8_t* un,uint8_t* dec,uint8_t* cent);
void config(void);

//loop principal
void main(void) {
    config();
    TMR1H = 0;
    TMR1L = 0;
    initLCD();
    
    //configurando el LM75BD
    MasterStart_I2C();
    MasterSend_I2C(0X90); //direccion del sensor
    MasterSend_I2C(0X01); //registro de configuracion
    MasterSend_I2C(0x02); //Configuracion de interrupcion
    MasterStop_I2C();   //se detiene la comunicacion
    
    while(1){
        
        //lectura del LM75DB
        MasterStart_I2C();
        MasterSend_I2C(0X90); //DIRECCION DEL SENSORE
        MasterSend_I2C(0X00); //indica que leeremos temperatura
        MasterRepeatS_I2C();
        MasterSend_I2C(0X91); //direccion pero de lectura
        MasterReceive_I2C(&TempENT); //lee los dos bytes de temperatura, entero
        MasterReceive_I2C(&TempDEC); //esta es la parte decimal
        MasterStop_I2C();
        
        temp = (TempENT<<3)|(TempDEC>>5);
        digTemp = (float)temp*0.125;
        PORTB = TempENT;
        floToChar(digTemp,TEMPdig);
        
        DHT11_START();
        
        if(DHT11_ALIVE()){
            DHT11_ReadData(&Hum1);
            DHT11_ReadData(&dummyHum1);
            DHT11_ReadData(&Temp1);
            DHT11_ReadData(&dummyT1);
            DHT11_ReadData(&CHECKSUM);
            
            if(CHECKSUM == ((Hum1 + dummyHum1 + Temp1 + dummyT1) & 0XFF)){
                temperatura[7] = Temp1/10 + 48;
                temperatura[8] = Temp1%10 + 48;
                humedadR[7] = Hum1/10 + 48;
                humedadR[8] = Hum1%10 + 48;
                cursorLCD(1,1);
                LCDstring(temperatura);
                cursorLCD(2,1);
                dispCHAR(TEMPdig[5]+48);
                dispCHAR(TEMPdig[4]+48);
                dispCHAR(TEMPdig[3]+48);
                dispCHAR('.');
                dispCHAR(TEMPdig[1]+48);
                dispCHAR(TEMPdig[2]+48);
                dispCHAR(TEMPdig[0]+48);
                //LCDstring(humedadR);
            }
            else{
                cursorLCD(1,1);
                LCDstring("ERROR");
                cursorLCD(2,1);
                LCDstring("ERROR");
            }
        }
        else{
                cursorLCD(1,1);
                LCDstring("ERROR");
                cursorLCD(2,1);
                LCDstring("ERROR");
            }
        
        T1CONbits.TMR1ON = 0;
        __delay_ms(10);
    }
}

void config(void){
    ANSEL =     0X00;
    ANSELH =    0X00;
    TRISA =     0X00;
    TRISB =     0X00;
    TRISD =     0X00;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    PORTA =     0X00;
    PORTB =     0X00;
    PORTD =     0X00;
    
    //Configuracion del oscilador
    OSCCONbits.IRCF = 0b111; //oscilador a 8Mhz
    OSCCONbits.SCS = 0b1;
    
    //Configuracion del timmer1
    T1CON = 0X10; //periodo de 1MHz, ya que fuente es FOSC/4
    TMR1H = 0; //valores en 0 para el conteo de los periodos de pulso
    TMR1L = 0;  
    MasterInit_I2C(100000);
}

void floToChar(const float valor, unsigned char *salida){
    uint8_t entero;
    uint8_t decimal;
    float temp;
    unsigned char digdecimal[2];
    
    entero = valor;
    temp = valor - (float)entero;
    decimal = (temp*100);
    divisiondecimal(entero,&digdecimal[0],&digdecimal[1],&digdecimal[2]);
    salida[3] = digdecimal[0];
    salida[4] = digdecimal[1];
    salida[5] = digdecimal[2];
    divisiondecimal(decimal,&digdecimal[0],&digdecimal[1],&digdecimal[2]);
    salida[2] = digdecimal[0];
    salida[1] = digdecimal[1];
    salida[0] = digdecimal[2];
}

void divisiondecimal(uint8_t conteo,uint8_t* un,uint8_t* dec,uint8_t* cent){
    uint8_t div = conteo;
    *un =   0;
    *dec =  0;
    *cent = 0;
    //modifica los valores de las variables asignadas de forma inmediata
    
    while(div >= 100){
    *cent = div/100;
    div = div - (*cent)*(100);
    }
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10);
    }
    
    *un = div;
}