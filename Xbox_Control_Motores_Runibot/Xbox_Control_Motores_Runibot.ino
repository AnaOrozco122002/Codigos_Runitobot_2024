
//Librerias
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

//MACS de Controles 

//XboxSeriesXControllerESP32_asukiaaa::Core xboxController("14:CB:65:F7:18:AC");  //Miguel
//XboxSeriesXControllerESP32_asukiaaa::Core xboxController("14:cb:65:b9:1f:cf"); //felipe
//XboxSeriesXControllerESP32_asukiaaa::Core xboxController("F4:6A:D7:A6:A8:D9"); //Angieth
XboxSeriesXControllerESP32_asukiaaa::Core xboxController("3C:FA:06:48:E3:CB"); //Anaso

//Varaibles

//Valores del Control
float LHoN=0,RTrig=0,LTrig=0;

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
bool Turbo = false, in=true, Co=true;
const byte Led=2;

void setup() {
  //Inicialización de la Comunicación Serial
  Serial.begin(115200);

  //Inicialización de la libreria para la Comunicación del Control de Xbox Series x/s
  xboxController.begin();

  //Inicialización de los Pines para el Control de los Motores
  Inicialiar_Pines();

  //Creación del PWM
  CrearPWM();
}

void loop() {
  // Inicialización del Control
  xboxController.onLoop();

  //Inicio de Logica
  if (xboxController.isConnected()) {

    //Esperar una Respuesta del Control
    if (xboxController.isWaitingForFirstNotification()) {
      Serial.println("Esperando por Notificacion");
    }
    else {

      //Control Conectado

      //Leer valores del Control
      uint16_t joystickMax = XboxControllerNotificationParser::maxJoy; //Maximo del Joystick
      LHoN=(float)xboxController.xboxNotif.joyLHori / joystickMax; //Joystick Izquierdo Horizontal Normalizado
      RTrig=xboxController.xboxNotif.trigRT;//Gatillo RT
      LTrig=xboxController.xboxNotif.trigLT;//Gatillo LT


      //Derecha
      if (LHoN >0.6) {

        //Sin gatillos
        if(!(RTrig >50 or LTrig > 50)){
          //Motor A
          ledcWrite(Canales[0],50);
          digitalWrite(MotorA[1],LOW);
          digitalWrite(MotorA[2],HIGH);
          //Motor B 
          ledcWrite(Canales[1],500);
          digitalWrite(MotorB[1],HIGH);
          digitalWrite(MotorB[2],LOW);
          //Motor C
          ledcWrite(Canales[2],500);
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
        if(RTrig > 50 ){

          //Sin Turbo
          if(!Turbo){

            //Velocidades de los Motores
            velocidades[0]=RTrig-700;
            velocidades[1]=RTrig;
            velocidades[2]=RTrig;
            velocidades[3]=RTrig-700;
            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[0] = (velocidades[0] > 100) ? 100 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[1] = (velocidades[1] > 500) ? 500 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[2] = (velocidades[2] > 500) ? 500 : velocidades[2];
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
            velocidades[0]=RTrig-700;
            velocidades[1]=RTrig;
            velocidades[2]=RTrig;
            velocidades[3]=RTrig-700;

            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[1] = (velocidades[1] > 10 && velocidades[1]< 500) ? velocidades[1]+500 : (velocidades[1] >= 1000) ? 1000 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[2] = (velocidades[2] > 10 && velocidades[2]< 500) ? velocidades[2]+500 : (velocidades[2] >= 1000) ? 1000 : velocidades[2];
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
        if(LTrig>50){
          if(!Turbo){

            
            //Velocidades de los Motores
            velocidades[0]=LTrig-700;
            velocidades[1]=LTrig;
            velocidades[2]=LTrig;
            velocidades[3]=LTrig-700;
            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[0] = (velocidades[0] > 100) ? 100 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[1] = (velocidades[1] > 500) ? 500 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[2] = (velocidades[2] > 500) ? 500 : velocidades[2];
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
            velocidades[0]=LTrig-700;
            velocidades[1]=LTrig;
            velocidades[2]=LTrig;
            velocidades[3]=LTrig-700;

            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[1] = (velocidades[1] > 10 && velocidades[1]< 500) ? velocidades[1]+500 : (velocidades[1] >= 1000) ? 1000 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[2] = (velocidades[2] > 10 && velocidades[2]< 500) ? velocidades[2]+500 : (velocidades[2] >= 1000) ? 1000 : velocidades[2];
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
      else if(LHoN <0.4){

        //Sin gatillos
        if(!(RTrig >50 or LTrig > 50)){
          //Motor A
          ledcWrite(Canales[0],500);
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
          ledcWrite(Canales[3],500);
          digitalWrite(MotorD[1],HIGH);
          digitalWrite(MotorD[2],LOW);
          //Impresión
          Serial.println("Izquierda Sin Gatillos");
        }
        //Adelante

        if(RTrig>50){
          if(!Turbo){

            //Velocidades de los Motores
            velocidades[0]=RTrig;
            velocidades[1]=RTrig-700;
            velocidades[2]=RTrig-700;
            velocidades[3]=RTrig;
            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[0] = (velocidades[0] > 500) ? 500 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[1] = (velocidades[1] > 100) ? 100 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[2] = (velocidades[2] > 100) ? 100 : velocidades[2];
            velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
            velocidades[3] = (velocidades[3] > 500) ? 500 : velocidades[3];

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
            velocidades[0]=RTrig;
            velocidades[1]=RTrig-700;
            velocidades[2]=RTrig-700;
            velocidades[3]=RTrig;

            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[0] = (velocidades[0] > 10 && velocidades[0]< 500) ? velocidades[0]+500 : (velocidades[0] >= 1000) ? 1000 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
            velocidades[3] = (velocidades[3] > 10 && velocidades[3]< 500) ? velocidades[3]+500 : (velocidades[3] >= 1000) ? 1000 : velocidades[3];
            

          
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

        if(LTrig>50){
          if(!Turbo){

            
            //Velocidades de los Motores
            velocidades[0]=LTrig;
            velocidades[1]=LTrig-700;
            velocidades[2]=LTrig-700;
            velocidades[3]=LTrig;
            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[0] = (velocidades[0] > 500) ? 500 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[1] = (velocidades[1] > 100) ? 100 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[2] = (velocidades[2] > 100) ? 100 : velocidades[2];
            velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
            velocidades[3] = (velocidades[3] > 500) ? 500 : velocidades[3];

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
            velocidades[0]=LTrig;
            velocidades[1]=LTrig-700;
            velocidades[2]=LTrig-700;
            velocidades[3]=LTrig;

            //Limitaciones de los Motores
            velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
            velocidades[0] = (velocidades[0] > 10 && velocidades[0]< 500) ? velocidades[0]+500 : (velocidades[0] >= 1000) ? 1000 : velocidades[0];
            velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
            velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
            velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
            velocidades[3] = (velocidades[3] > 10 && velocidades[3]< 500) ? velocidades[3]+500 : (velocidades[3] >= 1000) ? 1000 : velocidades[3];
            

          
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
      else if(RTrig>50 ){
        if(!Turbo){

          //Velocidades de los Motores
          velocidades[0]=RTrig;
          velocidades[1]=RTrig;
          velocidades[2]=RTrig;
          velocidades[3]=RTrig;
          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 800) ? 800 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 800) ? 800 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 800) ? 800 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 800) ? 800 : velocidades[3];

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
          velocidades[0]=RTrig;
          velocidades[1]=RTrig;
          velocidades[2]=RTrig;
          velocidades[3]=RTrig;

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
      else if(LTrig>50){
        if(!Turbo){
          //Velocidades de los Motores
          velocidades[0]=LTrig;
          velocidades[1]=LTrig;
          velocidades[2]=LTrig;
          velocidades[3]=LTrig;
          //Limitaciones de los Motores
          velocidades[0] = (velocidades[0] < 0) ? 0 : velocidades[0];
          velocidades[0] = (velocidades[0] > 800) ? 800 : velocidades[0];
          velocidades[1] = (velocidades[1] < 0) ? 0 : velocidades[1];
          velocidades[1] = (velocidades[1] > 800) ? 800 : velocidades[1];
          velocidades[2] = (velocidades[2] < 0) ? 0 : velocidades[2];
          velocidades[2] = (velocidades[2] > 800) ? 800 : velocidades[2];
          velocidades[3] = (velocidades[3] < 0) ? 0 : velocidades[3];
          velocidades[3] = (velocidades[3] > 800) ? 800 : velocidades[3];

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
          velocidades[0]=LTrig;
          velocidades[1]=LTrig;
          velocidades[2]=LTrig;
          velocidades[3]=LTrig;

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
      if(!(RTrig >50 or LTrig > 50)){
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
      if(xboxController.xboxNotif.btnRB  && Co){
          Turbo=!Turbo;
          Co=false;
          digitalWrite(Led,HIGH);
          Serial.print("Turbo: ");
          Serial.println(Turbo);
      }
      //Apagar Turbo
      else if(xboxController.xboxNotif.btnLB && !Co){
          Turbo=!Turbo;
          Co=true;
          digitalWrite(Led,LOW);
          Serial.print("Sin Turbo: ");
          Serial.println(Turbo);
        }


    }
  }
  else {
    Serial.println("Sin Conexion");
    if (xboxController.getCountFailedConnection() > 2) {
      ESP.restart();
    }
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
  pinMode(Led,OUTPUT);
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


