
class Controlador {
  private:
    // //VARIABES  
    float Referencia = 0.0;
    float Kp = 1.0; 
    float Ti = 0.0;
    float Td = 1.0; 
    float Error = 0.0; 
    float Error_ant = 0.0;
    float E_integral = 0.0;

  public:
    Controlador(){};

    float control(float Salida, int Tm){
      float E_derivativo;
      float Control;

      Error_ant       = Error; 
      Error           = Referencia - Salida;

      //if(abs(Error)< 0.01){ Error = 0.0;}

      //INTEGRAL
      E_integral     = E_integral + ((Error*(Tm/1000.0)) + ((Tm/1000.0)*(Error - Error_ant))/2.0);
      //if(E_integral> 100.0){ E_integral=  100.0;}
      //if(E_integral<-100.0){ E_integral= -100.0;}

      //DERIVATIVO
      E_derivativo   = (Error - Error_ant)/(Tm/1000.0);
      //E_derivativo = (Salidad - Salidad_ant)/(Tm/1000.0);

      //CONTROL
      Control        = Kp*Error + Ti*E_integral + Td*E_derivativo;

      if(Control > 1.5) { Control =  1.5;}
      if(Control < -1.5){ Control = -1.5;}
 
      return Control;
    }

    void setKP(float newKP){
      Kp = newKP;
    }

    void setKI(float newKI){
      Ti = newKI;
    }

    void setKD(float newKD){
      Td = newKD;
    }
};