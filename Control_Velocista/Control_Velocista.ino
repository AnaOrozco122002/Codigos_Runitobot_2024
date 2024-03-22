//Incluir Librerias
#include <QTRSensors16.h>
#include <ESP32Servo.h>


//Modulo de Inicio
const byte MInit=D6;
int Estado;

//TURBINA
//Creación del Objeto 
Servo myTurbina;

//PIN PARA EL CONTROL DE TURBINA
const byte Tur=D5;

//Variables para sensores
#define NUM_SENSORS             16  // Numero de sensores usados
#define NUM_SAMPLES_PER_SENSOR  3  // Numero de muestras
#define IN_PIN             A2  // PIN de entrada del multiplexor


// Inicialización del sensor, digitales D9,D10,D0,D1
QTRSensorsMux qtra((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, (unsigned char) IN_PIN, (unsigned char[]){D9,D10,D0,D1} );
unsigned int sensorValues[NUM_SENSORS];


//Variables para el controlador
float Tm = 9.0;                                            //tiempo de muestreo en mili segundos
float Referencia=0.0, Control=0.0, Kp = 2.5, Ti = 0.0, Td = 0.095; 
float Salida=0.0, Error=0.0, Error_ant=0.0;                       //variables de control
float offset = 1.0, Vmax = 0.0;
char caracter; String datos;   //  sintonizacion bluetooth
int d1, d2, d3,d4;                //  sintonizacion bluetooth
String S_Kp, S_Ti, S_Td, S_Vmax;       //  sintonizacion bluetooth
unsigned long int Tinicio = 0;

//CREACIÓN DE PWM
const uint16_t Frecuencia = 5000;
const byte Canales[] ={0,1};
const byte Resolucion = 10;

const int  PWMD = D7;                                             // Definición Pin 6 PWM Motor Derecho
const int  PWMI = D4;     

void setup() {
  Serial.begin(9600);

  //Inicialización de Pines
  Inicializacion_Pines();

  //Inicialización de Sensores
  Inicializacion_Sensores();

  //Creación del PWM
  CrearPWM();

  //Inicialización de la turbina
  Inicializacion_turbina();
}

void loop() {
  //if(Serial.available()) { Sintonia_Bluetooth();} 
  //Serial.println("Inicio");
  Estado=digitalRead(MInit);
  if(Estado == HIGH){
    Serial.println("Start");
  }
  else{
    Serial.println("Stop");
  }
  Tinicio    = millis();                                        // toma el valor en milisengundos
  Salida     = Lectura_Sensor();                                // funcion de lectura de la variable salida del  proceso
  Control    = Controlador(Referencia,Salida);                  // funcion de la ley de control 
  Esfuerzo_Control(Control);                                    // funcion encargada de enviar el esfuerzo de control
  Tm = Tiempo_Muestreo(Tinicio); 
  //Serial.println("Girar");
  myTurbina.write(150);

}

//Para leer el sensor
float Lectura_Sensor(void) {                                             
  Salida = (qtra.readLine(sensorValues)/7500.0) - 1.0;
  //Serial.println(Salida);
  return Salida;                                               // retorno la variable de salidad del proceso normalizada entre 0-1, al olgoritmo de control
}

//Controlador para Motoresu
float Controlador(float Referencia, float Salida) {                           // Funcion para la ley de control
  float E_derivativo;
  float E_integral;
  float Control;

  Error_ant      = Error; 
  Error          = Referencia - Salida;
  E_integral     = E_integral + ((Error*(Tm/1000.0)) + ((Tm/1000.0)*(Error - Error_ant))/2.0);
  E_integral     = ( E_integral > 100.0) ? 100.0 :  (E_integral < -100.0 ) ? -100 : E_integral;
  E_derivativo   = (Error - Error_ant)/(Tm/1000.0);
  Control        = Kp*( Error + Ti*E_integral + Td*E_derivativo );

  Control     = ( Control > 1.5) ? 1.5 :  (Control < -1.5 ) ? -1.5 : Control;
  //Serial.println(Control); 
  return Control;
}
void Esfuerzo_Control(float Control) {                            //envia el esfuerzo de control en forma de PWM
  float s1 , s2;

  s1  = (offset - Control);  
  s2  = (offset + Control);
  //Serial.print(s1); Serial.print("     "); Serial.println(s2);
  ledcWrite(Canales[0],   constrain(abs(s1), 0.0, 1.0)* Vmax);
  ledcWrite(Canales[1], constrain(abs(s2), 0.0, 1.0)* Vmax);

   if( s1 <= 0.0 ){// Motor Derecho
    digitalWrite(D8,LOW);}
  else{digitalWrite(D8,HIGH);}                   
   
  
  if( s2 <= 0.0 ){ //Motor Izquierdo
    digitalWrite(D3,LOW);}
  else{digitalWrite(D3,HIGH);}
} 

unsigned long int Tiempo_Muestreo(unsigned long int Tinicio){//, unsigned int Tm){ // Funcion que asegura que el tiempo de muestreo sea el mismo siempre
  unsigned long int T =millis()-Tinicio;
  return  T;
  }

void CrearPWM(){
  ledcSetup(Canales[0],Frecuencia,Resolucion); 
  ledcSetup(Canales[1],Frecuencia,Resolucion); 
  ledcAttachPin(PWMD,Canales[0]);
  ledcAttachPin(PWMI,Canales[1]);
}
void Inicializacion_turbina(){
  //Mensajes de Inicio
  Serial.println("-------------- Proceso de Calibracion de ESC --------------" );
  Serial.println("Iniciando ......");
  Serial.println("ATENCIÓN El motor Iniciara a Girar");
  ESP32PWM::allocateTimer(2);
  myTurbina.setPeriodHertz(50);              //frecuencia de la señal cuadrada
  myTurbina.attach(Tur, 1000, 2000);  //(pin,min us de pulso, máx us de pulso)
  myTurbina.write(0);                        //Preparación de la turbina
  delay(2000);
}
void Inicializacion_Sensores(){
  //Mensajes de Inicio
  Serial.println("-------------- Proceso de Calibracion de Los Sensores --------------" );
  Serial.println("Iniciando ......");
  //Calibración Inicial de Pines Sensor
  for (int i = 0; i < 100; i++){  // make the calibration take about 10 seconds
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  delay(2000);
}

void Inicializacion_Pines(){
  pinMode(PWMD,OUTPUT);
  pinMode(PWMI,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D8,OUTPUT);
  pinMode(MInit,INPUT);
}