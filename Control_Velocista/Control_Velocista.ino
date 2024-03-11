//Incluir Librerias
#include <QTRSensors16.h>
#include <ESP32Servo.h>


//TURBINA
//Creación del Objeto 
Servo Turbina;

//PIN PARA EL CONTROL DE TURBINA
const byte Tur=D7;
float tip=0,tip2=0,aux=999999999;
bool contur=true;

//Variables para sensores
#define NUM_SENSORS             16  // Numero de sensores usados
#define NUM_SAMPLES_PER_SENSOR  3  // Numero de muestras
#define IN_PIN             A1  // PIN de entrada del multiplexor


// Inicialización del sensor, digitales D8,D9,D5,D6
QTRSensorsMux qtra((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, (unsigned char) IN_PIN, (unsigned char[]){D8,D9,D5,D6} );
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

const int  PWMD = D4;                                             // Definición Pin 6 PWM Motor Derecho
const int  PWMI = D3;     

void setup() {
  Serial.begin(9600);
  Serial.println("*************** Proceso de Calibracion de Los Sensores ***************" );
  //Calibración Inicial de Pines Sensor
  for (int i = 0; i < 100; i++){  // make the calibration take about 10 seconds
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  
  //Creación del PWM
  CrearPWM();

  //Inicialización de la turbina
  Inicializacion_turbina();
}

void loop() {
  //if(Serial.available()) { Sintonia_Bluetooth();} 
  //Serial.println("Inicio");
  Tinicio    = millis();                                        // toma el valor en milisengundos
  Salida     = Lectura_Sensor();                                // funcion de lectura de la variable salida del  proceso
  Control    = Controlador(Referencia,Salida);                  // funcion de la ley de control 
  Esfuerzo_Control(Control);                                    // funcion encargada de enviar el esfuerzo de control
  Tm = Tiempo_Muestreo(Tinicio); 
  tip= millis();
  Serial.println("Girar");
  Turbina.write(180);
  delay(5000);
  Serial.println("Parar");
  Turbina.write(0);
  delay(5000);


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
    digitalWrite(D10,LOW);}
  else{digitalWrite(D10,HIGH);}                   
   
  
  if( s2 <= 0.0 ){ //Motor Izquierdo
    digitalWrite(D2,LOW);}
  else{digitalWrite(D2,HIGH);}
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
  //Asignar PIN
  Turbina.setPeriodHertz(50);
  Turbina.attach(Tur);//salida PW hacia el contralador de V
  //Prueba Del Motor
  Turbina.write(180);
  delay(1000);
  Turbina.write(0);
}
