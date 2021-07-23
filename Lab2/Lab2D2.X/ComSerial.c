#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ComSerial.h"

//******************************************************************************
//  configuracion del UART
//******************************************************************************
void configUART(void){
//Configuracion del EUSART
    TRISCbits.TRISC6 = 0;   //TX como salida
    TRISCbits.TRISC7 = 1;   //RX como entrada
    SPBRG =                 12;      //12 para un baud rate de 9615
    TXSTAbits.BRGH =        0;      //baja velocidad por el reloj
    TXSTAbits.TXEN =        1;      //habilitar transmision
    RCSTAbits.CREN =        1;      //habilita la recepcion
    TXSTAbits.SYNC =        0;      //modo asincrono
    RCSTAbits.SPEN =        1;      //configura los pines como seriales
}
//******************************************************************************
//  manejo de datos
//******************************************************************************

void send1dato(char dato){ 
    TXREG = dato;   //carga el dato que se va a enviar
    while(!TXSTAbits.TRMT); //espera hasta que se envie el caracter
}

void sendString(unsigned char* mensaje){ // \r se utiliza para el salto de linea
    while(*mensaje != 0x00){    //mira si el valor es distinto a 0
        send1dato(*mensaje);    //si lo es, envia el dato
        mensaje ++;             //apunta a la siguiente letra en el mensaje
    }//como apunta a la direccion de inicio de todo el string, con cada
     //iteracion cambia al siguiente caracter del string
}

void sendfloat(const float valor){
    uint8_t entero;
    uint8_t decimal;
    float temp;
    unsigned char digdecimal[1];
    /*Toma un valor tipo float y pasa su entero a una variable, el valor 
     * restante lo opera para pasar las decimas a enteros y luego separarlos
     * en 2 datos, para poder representarlos luego en el puerto serial enviando
     * un caracter a la vez.
     */
    entero = valor;
    temp = valor-(float)entero;
    decimal = (temp*100);
    division(decimal,&digdecimal[0],&digdecimal[1]);
    send1dato(entero+48);
    sendString(".");
    send1dato(digdecimal[1]+48);
    send1dato(digdecimal[0]+48);
    sendString("\r");
    
}

void sendhex(uint8_t valor){
    uint8_t centena;
    uint8_t decena;
    uint8_t unidad;
    
    divisiondecimal(valor,&unidad,&decena,&centena); //separa los valores
    send1dato(centena +48);//envia cada valor con el arreglo para el 
    send1dato(decena +48);//ASCII
    send1dato(unidad +48);
}

//Divisiones utilizadas para separar los valores en decimas, centesimas y 
//unidades
void division(uint8_t conteo,uint8_t* un,uint8_t* dec){
    uint8_t div = conteo;
    *un =   0;
    *dec =  0;
    //modifica los valores de las variables asignadas de forma inmediata
    
    while (div >= 10){
    *dec = div/10;
    div = div - (*dec)*(10);
    }
    
    *un = div;
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