#include <ESP32Servo.h>

#define pwmTurbina  15

class Turbina {
  private:
    // VARIABES  
    Servo myTurbina;

  public:
    Turbina(){
      //Configuración de turbina
      ESP32PWM::allocateTimer(2);
      myTurbina.setPeriodHertz(60);              //frecuencia de la señal cuadrada
      myTurbina.attach(pwmTurbina, 1000, 2000);  //(pin,min us de pulso, máx us de pulso)
      myTurbina.writeMicroseconds(1000);
      delay(1000);
    }

    void start(int vel){
      myTurbina.writeMicroseconds(vel);
    }

    void activateTurbine(){
      myTurbina.attach(pwmTurbina, 1000, 2000);
    }

    void desactivateTurbine(){
      myTurbina.detach();
    }
};