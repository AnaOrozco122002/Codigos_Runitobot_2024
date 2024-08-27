
#define       ain1        35     
#define       ain2        37
#define       pwmA        33     //PWM motor A
#define       stbyA       39     //habilitación de motor A
#define       bin1        34
#define       bin2        21
#define       pwmB        17     //PWM motor B
#define       stbyB       36     //habilitación de motor B
#define canalA 0
#define canalB 1

class AccionControl {
  private:
    // VARIABES  
    float offset = 0.0;
    float Vmax =  0.0;

    const int  frecuencia = 5000;
    const byte resolucion = 10;
    int        pmwMotor=pow(2,resolucion)-1;

  public:
    AccionControl(){
      //Configuración de motores
      pinMode(ain1,OUTPUT);
      pinMode(ain2,OUTPUT);
      pinMode(stbyA,OUTPUT);
      ledcSetup(canalA, frecuencia, resolucion);
      ledcAttachPin(pwmA, canalA);
      
      pinMode(bin1,OUTPUT);
      pinMode(bin2,OUTPUT);
      pinMode(stbyB,OUTPUT);  
      ledcSetup(canalB, frecuencia, resolucion);
      ledcAttachPin(pwmB, canalB);

      digitalWrite(stbyA,HIGH); //Habilita Tb6612
      digitalWrite(stbyB,HIGH); //Habilita Tb6612
    };

    void accionControl(float Control){
      float s1 , s2;
      int vel1, vel2;

      s1  = (offset - Control);  
      s2  = (offset + Control);
      vel1 = constrain(abs(s1), 0.0, 1.0)* Vmax;
      vel2 = constrain(abs(s2), 0.0, 1.0)* Vmax;

      ledcWrite(canalA, vel1);
      ledcWrite(canalB, vel2);

      if( s1 <= 0.0 ){// Motor Derecho
        digitalWrite(ain1,LOW);
        digitalWrite(ain2,HIGH);
      } else{
        digitalWrite(ain1,HIGH);
        digitalWrite(ain2,LOW);
      }                   
      
      if( s2 <= 0.0 ){ //Motor Izquierdo
        digitalWrite(bin1,LOW);
        digitalWrite(bin2,HIGH);
      } else{
        digitalWrite(bin1,HIGH);
        digitalWrite(bin2,LOW);
      }
    }

    void stopEngines(){
      digitalWrite(ain1,LOW);
      digitalWrite(ain2,LOW);
      digitalWrite(bin1,LOW);
      digitalWrite(bin2,LOW);
    }

    void setOffset(float newOffser){
      offset = newOffser;
    }

    void setVmax(float newVmax){
      Vmax = newVmax;
    }
};