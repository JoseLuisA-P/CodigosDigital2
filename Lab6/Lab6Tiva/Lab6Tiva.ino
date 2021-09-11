/*
  Laboratorio #6: Juego de Carreras
  Hardware: Tiva C TM4C123G
  Creador: Jose Alvarez
  Fecha: 9 de septiembre de 2021
  Modificacion: 9 de septiembre de 2021
  
*/


bool Comp1[9] = {false,false,false,false,false,false,false,false,true};
bool Comp2[9] = {false,false,false,false,false,false,false,false,true};

int pushB1 = 31; //button 1 placa
int pushB2 = 17;//button 2 placa

bool S1DEB = false;  //antirebote software B1
bool S2DEB = false;  //antirebote software B2

int ESTADO = 0; //estado del juego

//Prototipos de funciones
void Aumento(bool Cambio[]); 
void CheckB1(void);
void CheckB2(void);

//Configuracion general del uC
void setup() {
  Serial.begin(9600); //serial a 9600
  pinMode(pushB1, INPUT_PULLUP); //Boton con PullUp
  pinMode(pushB2, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT); //Led rojo como salida
  pinMode(GREEN_LED, OUTPUT); //Led verde como salida
  //pines de salida del J1
  pinMode(PA_2,OUTPUT);
  pinMode(PA_3,OUTPUT);
  pinMode(PA_4,OUTPUT);
  pinMode(PB_6,OUTPUT);
  //pines de salida del J2
  pinMode(PC_4,OUTPUT);
  pinMode(PC_5,OUTPUT);
  pinMode(PC_6,OUTPUT);
  pinMode(PC_7,OUTPUT);
}

//Loop continuo
void loop() {
  switch(ESTADO){
    case 0: //esperando la señal para comenzar
    if(digitalRead(pushB1) == LOW) ESTADO = 1; //Comienza el juego 
      break;
    case 1: //simulacion semaforo antes de comenzar
      for(int i=0; i<9; i++){ //Inicializar los valores
      Comp1[i] = false;
      Comp2[i] = false;
      }
      Comp1[8] = true;
      Comp2[8] = true;
      digitalWrite(RED_LED, HIGH); //Secuencia de luces
      digitalWrite(GREEN_LED, LOW);
      delay(1000);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      delay(1000);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      delay(1000);
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      ESTADO = 2;
      break;
    case 2: //ya comenzo la carrera y saca quien gano
      CheckB1(); //revisar botones y modificar la lista
      CheckB2();
      if(Comp1[0]== true) { //Ver quien gano e identificarlo
        ESTADO = 0;
        digitalWrite(RED_LED, HIGH);
        Serial.println("El jugador 1 GANO (ROJO)\n\n\n\n\n");
      }
      else if (Comp2[0]== true) {
        ESTADO = 0;
        digitalWrite(GREEN_LED, HIGH);
        Serial.println("El jugador 2 GANO (VERDE)\n\n\n\n\n");
      }
      digitalWrite(PA_2,Comp1[0]);
      digitalWrite(PA_3,Comp1[1]);
      digitalWrite(PA_4,Comp1[2]);
      digitalWrite(PB_6,Comp1[3]);

      digitalWrite(PC_4,Comp2[0]);
      digitalWrite(PC_5,Comp2[1]);
      digitalWrite(PC_6,Comp2[2]);
      digitalWrite(PC_7,Comp2[3]);
      break;
    
    }
 
}

void Aumento(bool Cambio[]){ //Cambia el orden de los valores en la lista de forma ciclica
  for (int i=0; i<8; i++){
    bool temp;
    temp = Cambio[i+1];
    Cambio[i+1] = Cambio[i];
    Cambio[i] = temp;
    }
}

void CheckB1(void){
    //Mira si se presiono el boton con un antirebotes e imprime el nuevo valor de las listas
    if(digitalRead(pushB1) == LOW){ 
    S1DEB = true;
    }
    
   if(digitalRead(pushB1)== HIGH && S1DEB == true){
      Aumento(Comp1);
      S1DEB = false;
      
      for(int i=0; i<9; i++){
      Serial.print(int(Comp1[i]));
      }
      Serial.println("");
      
      for(int i=0; i<9; i++){
      Serial.print(int(Comp2[i]));
      }
      Serial.println("\n\n\n\n\n");  
    }
}

void CheckB2(void){
    //Mira si se presiono el boton con un antirebotes e imprime el nuevo valor de las listas
    if(digitalRead(pushB2) == LOW){
    S2DEB = true;
    }
    
   if(digitalRead(pushB2)== HIGH && S2DEB == true){
      Aumento(Comp2);
      S2DEB = false;
      
      for(int i=0; i<9; i++){
      Serial.print(int(Comp1[i]));
      }
      Serial.println("");
      
      for(int i=0; i<9; i++){
      Serial.print(int(Comp2[i]));
      }
      Serial.println("\n\n\n\n\n");  
    }
}
