/*
    Fusion de Códigos 
    15-Marzo-2024
*/

/* ----------------------------- LIBRERIAS ---------------------------------------- */
#include "SensorLinea.h"
#include "Controlador.h"
#include "AccionControl.h"
#include "Turbina.h"
#include <WiFi.h>
#include <WebServer.h>

#include <EEPROM.h>
#define EEPROM_SIZE 512
#define EEPROM_ADDR_CALIBRATION 0
struct CalibrationStruct {
  uint16_t calibrationMaximumEEPROM[16];
  uint16_t calibrationMinimumEEPROM[16];
};

/* ----------------------------- PINES ---------------------------------------- */
#define       pwmTurbina  15
#define       pinStart    13     //Señal de inicio del Modulo de arranque
#define       pinStop     14     //Señal de fin del Modulo de arranque
#define       pinEncoderA 16     //Pin de interrupción
#define       pinEncoderB 40     //Pin de interrupción    


/* ----------------------------- VARIABLES ---------------------------------------- */
// Credenciales WIFI.
const char *ssid = "Velocista";
const char *password = "12345678";

int Tinicio = 0;
int Tm = 0;

/* ----------------------------- OBJETOS ---------------------------------------- */
SensorLinea sensorLinea;
Controlador controlador;
AccionControl accionControl;
Turbina turbina;
WebServer server(80);


//-------------------------------------------------------- SETUP()
void setup() { 
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();

  server.on("/", handleRoot);
  server.on("/CONSTANTES", constantes);
  server.on("/CALIBRAR", calibrar);
  server.on("/INICIAR", iniciar);
  server.begin();
  
  //Lee las valores de calibracion guardados
  readCalibrationEEPROM();
}

//---------------------------LOOP() ----------------------------------
void loop(){

  // Tm = millis()- Tinicio;
  // Tinicio = millis();
  // float Salida  = ( ((float)sensorLinea.readLine() )/7500)-1.0;
  // float Control = controlador.control(Salida, Tm);
  // accionControl.accionControl(Control);
  server.handleClient();
  
  // server.handleClient();
  // digitalWrite(ain1,LOW);
  // digitalWrite(ain2,LOW);
  // digitalWrite(bin1,LOW);
  // digitalWrite(bin2,LOW);
}
//------------------------- FUNCIONES AUXILIARES() ------------------------

void iniciar(){
  server.send(200, "text/plain", "OK");
  int TiempoInicio = millis();
  while(true){
    Tm = millis()- Tinicio;
    Tinicio = millis();
    float Salida  = ( ((float)sensorLinea.readLine() )/7500)-1.0;
    float Control = controlador.control(Salida, Tm);
    accionControl.accionControl(Control);
    server.handleClient();
  }
  accionControl.stopEngines();
  turbina.start(1000);
}

void constantes() {
  float Kp = server.arg("KP").toFloat();
  float Td = server.arg("TD").toFloat();
  float Ti = server.arg("TI").toFloat();
  float Vmax = server.arg("VMAX").toFloat();
  float Offset = server.arg("OFFSET").toFloat();
  int turbina1 = server.arg("TURBINA").toInt();

  controlador.setKP(Kp);
  controlador.setKD(Td);
  controlador.setKI(Ti);
  accionControl.setVmax(Vmax);
  accionControl.setOffset(Offset);
  turbina.start(turbina1);
  server.send(200, "text/plain", "OK");

  Serial.print("KP: ");
  Serial.println(Kp);
  Serial.print("TD: ");
  Serial.println(Td);
  Serial.print("TI: ");
  Serial.println(Ti);
  Serial.print("VMAX: ");
  Serial.println(Vmax);
  Serial.print("Offset: ");
  Serial.println(Offset);
  Serial.print("turbina: ");
  Serial.println(turbina1);
}

void calibrar() {

  server.send(200, "text/plain", "OK");

  //Desactiva la turbina para encender el led
  turbina.desactivateTurbine();
  pinMode(pwmTurbina,OUTPUT);
  digitalWrite(pwmTurbina,HIGH);

  sensorLinea.calibrateSeconds(10000);

  //Activa la turbina y apaga el led
  digitalWrite(pwmTurbina,LOW);
  turbina.activateTurbine();

  //Guarda los valores en la EEPROM
  saveCalibrationEEPROM();
}

void handleRoot() {
  const String Pagina =  R"====(<!DOCTYPE html>
    <html>
      <head>
        <meta charset='UTF-8'>
        <title> Servidor velocista </title>
      </head>
      <style>
        body {
          display: flex;
          flex-direction: column;
          align-items: center;
        }
        .Input_Constantes {
          display: flex;
          height: 30px;
          margin: 10px;
          align-items: center;
        }
        .Input_title {
          padding-right: 10px;
        }
        .Boton {
          padding: 5px 5px;
          margin: 10px 20px;
        }
      </style>
      <body>
        <h1> Servidor velocista </h1>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> KP </H4>
          <input id='KP' type='number' >
        </div>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> KD </H4>
          <input id='KD' type='number' >
        </div>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> KI </H4>
          <input id='KI' type='number' >
        </div>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> Vmax </H4>
          <input id='VMAX' type='number' >
        </div>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> OFFSET </H4>
          <input id='OFFSET' type='number' >
        </div>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> TURBINA </H4>
          <input id='TURBINA' type='number' >
        </div>
        <div class='Input_Constantes'>
          <H4 class='Input_title'> DISTANCIA </H4>
          <input id='DISTANCIA' type='number' >
        </div>
        <div>
          <button class='Boton' onclick='CalibrarSensor()'> Calibrar sensor </button>
          <button class='Boton' onclick='EnvierConstantes()'> Enviar constantes </button>
        </div>
        <div>
          <button class='Boton' onclick='Iniciar()'> Iniciar robot </button>
        </div>
      </body>
      <script>
        function EnvierConstantes() {
          const KP = document.getElementById('KP').value;
          const KD = document.getElementById('KD').value;
          const KI = document.getElementById('KI').value;
          const VMAX = document.getElementById('VMAX').value;
          const OFFSET = document.getElementById('OFFSET').value;
          const TURBINA = document.getElementById('TURBINA').value;


          const href = window.location.href;
          const url = href + 'CONSTANTES?' + 'KP=' + KP + '&' + 'TD=' + KD + '&' + 'TI=' + KI + '&' + 'VMAX=' + VMAX + '&' + 'OFFSET=' + OFFSET + '&' + 'TURBINA=' + TURBINA
          
          fetch(url)
            .then(response => {
              if (!response.ok) {
                window.alert('Error al guardar constantes');
              }else{
                window.alert('Constantes guardadas');
              }
            })
        }

        function Iniciar() {

          const href = window.location.href;
          const url = href + 'INICIAR'

          fetch(url)
            .then(response => {
              if (!response.ok) {
                window.alert('Error en la solicitud');
              }else{
                window.alert('Robot iniciado');
              }
            })

        }

        function CalibrarSensor() {

          const href = window.location.href;
          const url = href + 'CALIBRAR'

          fetch(url)
            .then(response => {
              if (!response.ok) {
                window.alert('Error al calibrar');
              }else{
                window.alert('Calibrando');
              }
            })
            
        }
      </script>
    </html>)====";
  server.send(200, "text/html", Pagina);
}

void saveCalibrationEEPROM(){
  CalibrationStruct data;
  for (int i=0; i < 16 ;i++){
    data.calibrationMaximumEEPROM[i] = sensorLinea.getCalibrationMaximum(i);
    data.calibrationMinimumEEPROM[i] = sensorLinea.getcalibrationMinimum(i);
  }
  EEPROM.put(EEPROM_ADDR_CALIBRATION, data);
  EEPROM.commit();
  sensorLinea.printCalibration();
}

void readCalibrationEEPROM(){
  CalibrationStruct data;
  EEPROM.get(EEPROM_ADDR_CALIBRATION, data);
  for (int i=0; i < 16 ;i++){
    sensorLinea.setCalibrationMaximum(i, data.calibrationMaximumEEPROM[i]);
    sensorLinea.setCalibrationMinimum(i, data.calibrationMinimumEEPROM[i]);
  }
  sensorLinea.printCalibration();
}
