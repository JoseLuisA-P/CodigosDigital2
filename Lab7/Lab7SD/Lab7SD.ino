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
int archselect = 1;

const int chipSelect = PA_3; //PIN del chip select

const int pushB1 = 31; //button 1 placa
const int pushB2 = 17;//button 2 placa

bool S1DEB = false;  //antirebote software B1
bool S2DEB = false;  //antirebote software B2

//////////////////////////////////////////VOID SETUP////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  pinMode(pushB1, INPUT_PULLUP); //Boton con PullUp
  pinMode(pushB2, INPUT_PULLUP);
  
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
  
  Serial.println("Esos son todos los archivos"); //indicador de terminado, evitar errores
}

//////////////////////////////////////////VOID LOOP////////////////////////////////////////////////

void loop() {
  //COMPROBACION DEL BOTON 1
  if(digitalRead(pushB1) == LOW){ 
    S1DEB = true;
    }
    
  if(digitalRead(pushB1)== HIGH && S1DEB == true){
    archselect ++;
    if (archselect >= 4){
      archselect = 1;
      }
    Serial.println("¿Quiere imprimir el archivo " + archives[archselect] + "?\n\n\n\n");
    S1DEB = false;
    }   
    
  //COMPROBACION DEL BOTON 2
  if(digitalRead(pushB2) == LOW){
    S2DEB = true;
    }
    
   if(digitalRead(pushB2)== HIGH && S2DEB == true){
    char impresion[20]; //para crear el array de caracteres
    archives[archselect].toCharArray(impresion,20); //convertir el string en array de caracteres
    imprimirArchivo(impresion); //imprimir el archivo
    Serial.print("\n\n\n\n");
    S2DEB = false;
    }
}

/////////////////////////////////FUNCIONES PROPIAS////////////////////////////////////////////////

void ListaNombres(File archivo){
  int n = 0; //para cada uno de los archivos que pueden existir
  while(true){
    File dummy = archivo.openNextFile(); //abre el siguiente archivo
    if(!dummy){ break;} //deja de hacerlo, no hay mas archivos
    archives[n] = dummy.name(); //agrega los nombres de los archivos
    n++; //lo coloca en el siguiente string
    }
  
  }

void imprimirArchivo(const char *nombre){
  File archivo; //dummy para la funcion
  archivo = SD.open(nombre,FILE_READ); //abre el archivo con el nombre en modo lectura
  if (archivo){ //si esta disponible
    Serial.println(nombre);
    while(archivo.available()){ //imprime su nombre y sus lineas hasta encontrar un vacio
      Serial.write(archivo.read());
      }
      archivo.close();
    }
  else{ //imprime que es error de apertura
    Serial.println("error de apertura");
    }
    
  
  }
