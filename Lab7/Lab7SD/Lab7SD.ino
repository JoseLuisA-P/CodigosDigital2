/*
MOSI - PA_5
MISO - PA_4
SCK -  PA_2
CS -   PA_3
*/

//incluir las librerias de la SD y SPI
#include <SPI.h>
#include <SD.h>

File root;
String archives[10];

const int chipSelect = PA_3; //PIN del chip select

void setup() {
  Serial.begin(9600);

  SPI.setModule(0);
  Serial.print("\nInitializing SD card...");
  pinMode(PA_3,OUTPUT);

  if(!SD.begin(chipSelect)){ //inicia la SD y determina si se logra o no
    Serial.println("Inicio Fallido");
    return;
    }
  Serial.println("Inicio exitoso");
  
  root = SD.open("/"); //permite el acceso a los archivos de la SD
  ListaNombres(root); //crea el listado de nombres de los archivos
  Serial.println("Estos son los archivos");
  for(int i=1; i < 5;i++){ //imprime el nombre de los archivos en la SD
    Serial.println(archives[i]);
    }

  Serial.println("terminado"); //indicador de terminado, evitar errores
}

void loop() {
   
  
}

void ListaNombres(File archivo){
  int n = 0; //para cada uno de los archivos que pueden existir
  while(true){
    File dummy = archivo.openNextFile(); //abre el siguiente archivo
    if(!dummy){ break;} //deja de hacerlo, no hay mas archivos
    archives[n] = String(dummy.name()); //agrega los nombres de los archivos
    n++; //lo coloca en el siguiente string
    }
  
  }
