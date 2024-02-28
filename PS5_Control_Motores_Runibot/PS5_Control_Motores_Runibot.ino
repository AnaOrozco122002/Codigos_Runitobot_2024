//Librerias
#include <ps5.h>
#include <ps5Controller.h>
#include <ps5_int.h>


//Varaibles

//Valores del Control
float LHoN=0,R2=0,L2=0;

//Controlador Motores
const byte MotorA[]={13,27,12};
const byte MotorB[]={5,19,18};
const byte MotorC[]={23,21,22};
const byte MotorD[]={32,26,25};

//Creación del PWM para la ESP32
const uint16_t Frecuencia = 5000;
const byte Resolucion = 10;
const byte Canales[] ={0,1,2,3};

//Variables de Control de Velocidad
uint16_t velocidades[]={0,0,0,0};

//Variables para Utilización del Turbo
bool Turbo = false, in=true, Co=true, bR2,bL2;
const byte Led=2;

void setup() {
  //Inicialización de la Comunicación Serial
  Serial.begin(115200);

  //Inicialización de la libreria para la Comunicación del Control de PS5
  ps5.begin("E8:47:3A:3D:07:A5");

  //Inicialización de los Pines para el Control de los Motores
  Inicialiar_Pines();

  //Creación del PWM
  CrearPWM();
}

void loop() {

  //Inicio de Logica
  if (ps5.isConnected()) {


    //Control Conectado

    //Leer valores del Control
    LHoN=ps5.LStickX(); //Joystick Izquierdo Horizontal Normalizado
    R2=floor(((float)ps5.R2Value()/255)*1023);//Gatillo RT
    L2=floor(((float)ps5.L2Value()/255)*1023);//Gatillo LT
    bR2=ps5.R2();
    bL2=ps5.L2();
    //Derecha
    if (LHoN >20) {

      //Sin gatillos
      if(!(bR2 or bL2)){
        //Motor A
        ledcWrite(Canales[0],50);
        digitalWrite(MotorA[1],LOW);
        digitalWrite(MotorA[2],HIGH);
        //Motor B 
        ledcWrite(Canales[1],300);
        digitalWrite(MotorB[1],HIGH);
        digitalWrite(MotorB[2],LOW);
        //Motor C
        ledcWrite(Canales[2],300);
        digitalWrite(MotorC[1],HIGH);
        digitalWrite(MotorC[2],LOW);
        //Motor D
        ledcWrite(Canales[3],50);
        digitalWrite(MotorD[1],LOW);
        digitalWrite(MotorD[2],HIGH);
        //Impresión
        Serial.println("Derecha Sin Gatillos");
      }

      //Adelante
      if(bR2){

        //Sin Turbo
        if(!Turbo){

          //Velocidades de los Motores
          velocidades[0]=R2-700;
          velocidades[1]=R2;
          velocidades[2]=R2;
          velocidades[3]=R2-700;
          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 100) ? 100 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 350) ? 350 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 350) ? 350 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 100) ? 100 : velocidades[3];

          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],LOW);
          digitalWrite(MotorA[2],HIGH);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],HIGH);
          digitalWrite(MotorB[2],LOW);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],HIGH);
          digitalWrite(MotorC[2],LOW);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],LOW);
          digitalWrite(MotorD[2],HIGH);
          //Impresión
          Serial.println("Derecha");
        }
        if(Turbo){
          //Velocidades de los Motores
          velocidades[0]=R2-700;
          velocidades[1]=R2;
          velocidades[2]=R2;
          velocidades[3]=R2-700;

          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 10 && velocidades[1]< 350) ? velocidades[1]+350 : (velocidades[1] >= 700) ? 700 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 10 && velocidades[2]< 350) ? velocidades[2]+350 : (velocidades[2] >= 700) ? 700 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];

        
          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],LOW);
          digitalWrite(MotorA[2],HIGH);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],HIGH);
          digitalWrite(MotorB[2],LOW);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],HIGH);
          digitalWrite(MotorC[2],LOW);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],LOW);
          digitalWrite(MotorD[2],HIGH);
          //Impresión
          Serial.println("Derecha Turbo");
        }
      }
      //Atrás
      else if(bL2){
        if(!Turbo){

          
          //Velocidades de los Motores
          velocidades[0]=L2-700;
          velocidades[1]=L2;
          velocidades[2]=L2;
          velocidades[3]=L2-700;
          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 100) ? 100 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 350) ? 350 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 350) ? 350 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 100) ? 100 : velocidades[3];

          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],HIGH);
          digitalWrite(MotorA[2],LOW);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],LOW);
          digitalWrite(MotorB[2],HIGH);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],LOW);
          digitalWrite(MotorC[2],HIGH);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],HIGH);
          digitalWrite(MotorD[2],LOW);
          //Impresión
          Serial.println("Derecha Atras");
        }
        if(Turbo){

          //Velocidades de los Motores
          velocidades[0]=L2-700;
          velocidades[1]=L2;
          velocidades[2]=L2;
          velocidades[3]=L2-700;

          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 10 && velocidades[1]< 350) ? velocidades[1]+350 : (velocidades[1] >= 700) ? 700 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 10 && velocidades[2]< 350) ? velocidades[2]+350 : (velocidades[2] >= 700) ? 700 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];

        
          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],HIGH);
          digitalWrite(MotorA[2],LOW);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],LOW);
          digitalWrite(MotorB[2],HIGH);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],LOW);
          digitalWrite(MotorC[2],HIGH);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],HIGH);
          digitalWrite(MotorD[2],LOW);
          //Impresión
          Serial.println("Derecha Atras Turbo");
        }  
      }
      
    }

    //Izquierda
    else if(LHoN <-20){

      //Sin gatillos
      if(!(bR2 or bL2)){
        //Motor A
        ledcWrite(Canales[0],300);
        digitalWrite(MotorA[1],HIGH);
        digitalWrite(MotorA[2],LOW);
        //Motor B 
        ledcWrite(Canales[1],50);
        digitalWrite(MotorB[1],LOW);
        digitalWrite(MotorB[2],HIGH);
        //Motor C
        ledcWrite(Canales[2],50);
        digitalWrite(MotorC[1],LOW);
        digitalWrite(MotorC[2],HIGH);
        //Motor D
        ledcWrite(Canales[3],300);
        digitalWrite(MotorD[1],HIGH);
        digitalWrite(MotorD[2],LOW);
        //Impresión
        Serial.println("Izquierda Sin Gatillos");
      }
      //Adelante

      if(bR2){
        if(!Turbo){

          //Velocidades de los Motores
          velocidades[0]=R2;
          velocidades[1]=R2-700;
          velocidades[2]=R2-700;
          velocidades[3]=R2;
          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 350) ? 350 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 100) ? 100 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 100) ? 100 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 350) ? 350 : velocidades[3];

          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],HIGH);
          digitalWrite(MotorA[2],LOW);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],LOW);
          digitalWrite(MotorB[2],HIGH);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],LOW);
          digitalWrite(MotorC[2],HIGH);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],HIGH);
          digitalWrite(MotorD[2],LOW);
          //Impresión
          Serial.println("Izquierda");
        }
        if(Turbo){

          //Velocidades de los Motores
          velocidades[0]=R2;
          velocidades[1]=R2-700;
          velocidades[2]=R2-700;
          velocidades[3]=R2;

          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 10 && velocidades[0]< 350) ? velocidades[0]+350 : (velocidades[0] >= 700) ? 700 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 10 && velocidades[3]< 350) ? velocidades[3]+350 : (velocidades[3] >= 700) ? 700 : velocidades[3];
          

        
          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],HIGH);
          digitalWrite(MotorA[2],LOW);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],LOW);
          digitalWrite(MotorB[2],HIGH);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],LOW);
          digitalWrite(MotorC[2],HIGH);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],HIGH);
          digitalWrite(MotorD[2],LOW);
          //Impresión
          Serial.println("Izquierda Turbo");
        }
      }

      //Atrás

      if(bL2){
        if(!Turbo){

          
          //Velocidades de los Motores
          velocidades[0]=L2;
          velocidades[1]=L2-700;
          velocidades[2]=L2-700;
          velocidades[3]=L2;
          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 350) ? 350 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 100) ? 100 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 100) ? 100 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 350) ? 350 : velocidades[3];

          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],LOW);
          digitalWrite(MotorA[2],HIGH);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],HIGH);
          digitalWrite(MotorB[2],LOW);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],HIGH);
          digitalWrite(MotorC[2],LOW);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],LOW);
          digitalWrite(MotorD[2],HIGH);
          //Impresión
          Serial.println("Izquierda Atras");
        }
        if(Turbo){

          
          //Velocidades de los Motores
          velocidades[0]=L2;
          velocidades[1]=L2-700;
          velocidades[2]=L2-700;
          velocidades[3]=L2;

          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 10 && velocidades[0]< 350) ? velocidades[0]+350 : (velocidades[0] >= 700) ? 700 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 10 && velocidades[3]< 350) ? velocidades[3]+350 : (velocidades[3] >= 700) ? 700 : velocidades[3];
          

        
          //Motor A
          ledcWrite(Canales[0],velocidades[0]);
          digitalWrite(MotorA[1],LOW);
          digitalWrite(MotorA[2],HIGH);
          //Motor B 
          ledcWrite(Canales[1],velocidades[1]);
          digitalWrite(MotorB[1],HIGH);
          digitalWrite(MotorB[2],LOW);
          //Motor C
          ledcWrite(Canales[2],velocidades[2]);
          digitalWrite(MotorC[1],HIGH);
          digitalWrite(MotorC[2],LOW);
          //Motor D
          ledcWrite(Canales[3],velocidades[3]);
          digitalWrite(MotorD[1],LOW);
          digitalWrite(MotorD[2],HIGH);
          //Impresión
          Serial.println("Izquierda Atras Turbo");
        }
      }
    }

    //Adelante
    else if(bR2 ){
      if(!Turbo){

        //Velocidades de los Motores
        velocidades[0]=R2;
        velocidades[1]=R2;
        velocidades[2]=R2;
        velocidades[3]=R2;
        Serial.print("R2: ");
        Serial.println(R2);

        //Limitaciones de los Motores
        velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
        velocidades[0] = (velocidades[0] > 600) ? 600 : velocidades[0];
        velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
        velocidades[1] = (velocidades[1] > 600) ? 600 : velocidades[1];
        velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
        velocidades[2] = (velocidades[2] > 600) ? 600 : velocidades[2];
        velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
        velocidades[3] = (velocidades[3] > 600) ? 600 : velocidades[3];

        //Motor A
        ledcWrite(Canales[0],velocidades[0]);
        digitalWrite(MotorA[1],HIGH);
        digitalWrite(MotorA[2],LOW);
        //Motor B 
        ledcWrite(Canales[1],velocidades[1]);
        digitalWrite(MotorB[1],HIGH);
        digitalWrite(MotorB[2],LOW);
        //Motor C
        ledcWrite(Canales[2],velocidades[2]);
        digitalWrite(MotorC[1],HIGH);
        digitalWrite(MotorC[2],LOW);
        //Motor D
        ledcWrite(Canales[3],velocidades[3]);
        digitalWrite(MotorD[1],HIGH);
        digitalWrite(MotorD[2],LOW);
        //Impresión
        Serial.println("Adelante");
      }else if(Turbo){
        //Velocidades de los Motores
        velocidades[0]=R2;
        velocidades[1]=R2;
        velocidades[2]=R2;
        velocidades[3]=R2;

        //Limitaciones de los Motores
        velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
        velocidades[0] = (velocidades[0] > 10 && velocidades[0]< 500) ? velocidades[0]+500 : (velocidades[0] >= 1000) ? 1000 : velocidades[0];
        velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
        velocidades[1] = (velocidades[1] > 10 && velocidades[1]< 500) ? velocidades[1]+500 : (velocidades[1] >= 1000) ? 1000 : velocidades[1];
        velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
        velocidades[2] = (velocidades[2] > 10 && velocidades[2]< 500) ? velocidades[2]+500 : (velocidades[2] >= 1000) ? 1000 : velocidades[2];
        velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
        velocidades[3] = (velocidades[3] > 10 && velocidades[3]< 500) ? velocidades[3]+500 : (velocidades[3] >= 1000) ? 1000 : velocidades[3];
        

      
        //Motor A
        ledcWrite(Canales[0],velocidades[0]);
        digitalWrite(MotorA[1],HIGH);
        digitalWrite(MotorA[2],LOW);
        //Motor B 
        ledcWrite(Canales[1],velocidades[1]);
        digitalWrite(MotorB[1],HIGH);
        digitalWrite(MotorB[2],LOW);
        //Motor C
        ledcWrite(Canales[2],velocidades[2]);
        digitalWrite(MotorC[1],HIGH);
        digitalWrite(MotorC[2],LOW);
        //Motor D
        ledcWrite(Canales[3],velocidades[3]);
        digitalWrite(MotorD[1],HIGH);
        digitalWrite(MotorD[2],LOW);
        //Impresión
        Serial.println("Adelante Turbo");
      }
    }
    //Atrás
    else if(bL2){
      if(!Turbo){
        //Velocidades de los Motores
        velocidades[0]=L2;
        velocidades[1]=L2;
        velocidades[2]=L2;
        velocidades[3]=L2;
        Serial.print("L2: ");
        Serial.println(L2);
        //Limitaciones de los Motores
        velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
        velocidades[0] = (velocidades[0] > 600) ? 600 : velocidades[0];
        velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
        velocidades[1] = (velocidades[1] > 600) ? 600 : velocidades[1];
        velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
        velocidades[2] = (velocidades[2] > 600) ? 600 : velocidades[2];
        velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
        velocidades[3] = (velocidades[3] > 600) ? 600 : velocidades[3];

        //Motor A
        ledcWrite(Canales[0],velocidades[0]);
        digitalWrite(MotorA[1],LOW);
        digitalWrite(MotorA[2],HIGH);
        //Motor B 
        ledcWrite(Canales[1],velocidades[1]);
        digitalWrite(MotorB[1],LOW);
        digitalWrite(MotorB[2],HIGH);
        //Motor C
        ledcWrite(Canales[2],velocidades[2]);
        digitalWrite(MotorC[1],LOW);
        digitalWrite(MotorC[2],HIGH);
        //Motor D
        ledcWrite(Canales[3],velocidades[3]);
        digitalWrite(MotorD[1],LOW);
        digitalWrite(MotorD[2],HIGH);
        //Impresión
        Serial.println("Atras");
      }else if(Turbo){

        //Velocidades de los Motores
        velocidades[0]=L2;
        velocidades[1]=L2;
        velocidades[2]=L2;
        velocidades[3]=L2;

        //Limitaciones de los Motores
        velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
        velocidades[0] = (velocidades[0] > 10 && velocidades[0]< 500) ? velocidades[0]+500 : (velocidades[0] >= 1000) ? 1000 : velocidades[0];
        velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
        velocidades[1] = (velocidades[1] > 10 && velocidades[1]< 500) ? velocidades[1]+500 : (velocidades[1] >= 1000) ? 1000 : velocidades[1];
        velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
        velocidades[2] = (velocidades[2] > 10 && velocidades[2]< 500) ? velocidades[2]+500 : (velocidades[2] >= 1000) ? 1000 : velocidades[2];
        velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
        velocidades[3] = (velocidades[3] > 10 && velocidades[3]< 500) ? velocidades[3]+500 : (velocidades[3] >= 1000) ? 1000 : velocidades[3];
        

      
        //Motor A
        ledcWrite(Canales[0],velocidades[0]);
        digitalWrite(MotorA[1],LOW);
        digitalWrite(MotorA[2],HIGH);
        //Motor B 
        ledcWrite(Canales[1],velocidades[1]);
        digitalWrite(MotorB[1],LOW);
        digitalWrite(MotorB[2],HIGH);
        //Motor C
        ledcWrite(Canales[2],velocidades[2]);
        digitalWrite(MotorC[1],LOW);
        digitalWrite(MotorC[2],HIGH);
        //Motor D
        ledcWrite(Canales[3],velocidades[3]);
        digitalWrite(MotorD[1],LOW);
        digitalWrite(MotorD[2],HIGH);
        //Impresión
        Serial.println("Atras Turbo");
      }
    }

    //Detenido
    if(!(bR2 or bL2)){
      //Motor A
      digitalWrite(MotorA[1],LOW);
      digitalWrite(MotorA[2],LOW);
      //Motor B 
      digitalWrite(MotorB[1],LOW);
      digitalWrite(MotorB[2],LOW);
      //Motor C
      digitalWrite(MotorC[1],LOW);
      digitalWrite(MotorC[2],LOW);
      //Motor D
      digitalWrite(MotorD[1],LOW);
      digitalWrite(MotorD[2],LOW);
      //Impresión
      //Serial.println("Detenido");
    }


    //Iniciar Turbo
    if(ps5.R1()  && Co){
        Turbo=!Turbo;
        Co=false;
        digitalWrite(Led,HIGH);
        Serial.print("Turbo: ");
        Serial.println(Turbo);
    }
    //Apagar Turbo
    else if(ps5.L1() && !Co){
        Turbo=!Turbo;
        Co=true;
        digitalWrite(Led,LOW);
        Serial.print("Sin Turbo: ");
        Serial.println(Turbo);
      }


    
  }
  else {
    Serial.println("Sin Conexion");
  }

}

void Inicialiar_Pines(){
  pinMode(MotorA[0],OUTPUT);
  pinMode(MotorA[1],OUTPUT);
  pinMode(MotorA[2],OUTPUT);
  pinMode(MotorB[0],OUTPUT);
  pinMode(MotorB[1],OUTPUT);
  pinMode(MotorB[2],OUTPUT);
  pinMode(MotorC[0],OUTPUT);
  pinMode(MotorC[1],OUTPUT);
  pinMode(MotorC[2],OUTPUT);
  pinMode(MotorD[0],OUTPUT);
  pinMode(MotorD[1],OUTPUT);
  pinMode(MotorD[2],OUTPUT);
}

void CrearPWM(){
  for(int i=0;i<4;i++){
    ledcSetup(Canales[i],Frecuencia,Resolucion); 
  }
  ledcAttachPin(MotorA[0],Canales[0]);
  ledcAttachPin(MotorB[0],Canales[1]);
  ledcAttachPin(MotorC[0],Canales[2]);
  ledcAttachPin(MotorD[0],Canales[3]);
}


