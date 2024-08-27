class SensorLinea {
  private:
    //VARIABES  
    uint8_t _sensorPins[4] = {11,9,7,5};
    uint8_t _sensorInputsCount = 4;
    uint8_t _sensorCount = 16;
    uint16_t _sensorValues[16];
    uint8_t _samplesPerSensor = 4;
    uint16_t _lastPosition = 0;
    uint8_t _signalPin = 12;
    uint16_t _maxValue = 8191;
    bool lineWhite = false;
    uint16_t _calibrationMaximum[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint16_t _calibrationMinimum[16] = {8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191,8191};

  public:
    SensorLinea(){
      pinMode(11,OUTPUT);
      pinMode(9,OUTPUT);
      pinMode(7,OUTPUT);
      pinMode(5,OUTPUT);
    }

    uint16_t readLine(){
      bool onLine = false;
      uint32_t avg = 0; // this is for the weighted total
      uint16_t sum = 0; // this is for the denominator, which is <= 64000

      readCalibrated();

      for (uint8_t i = 0; i < _sensorCount; i++){
        uint16_t value = _sensorValues[i];
        if (lineWhite) { value = 1000 - value; }

        // keep track of whether we see the line at all
        if (value > 200) { onLine = true; }

        // only average in values that are above a noise threshold
        if (value > 50){
          avg += (uint32_t)value * (i * 1000);
          sum += value;
        }
      }

      if (!onLine)
      {
        // If it last read to the left of center, return 0.
        if (_lastPosition < (_sensorCount - 1) * 1000 / 2){
          return 0;
        }
        // If it last read to the right of center, return the max.
        else{
          return (_sensorCount - 1) * 1000;
        }
      }

      _lastPosition = avg / sum;
      return _lastPosition;
    }

    void readCalibrated(){
      // read the needed values
      read();

      for (uint8_t i = 0; i < _sensorCount; i++){
        uint16_t calmin, calmax;
        calmax = _calibrationMaximum[i];
        calmin = _calibrationMinimum[i];

        uint16_t denominator = calmax - calmin;
        int16_t value = 0;

        if (denominator != 0){
          value = (((int32_t)_sensorValues[i]) - calmin) * 1000 / denominator;
        }else{
          value = 0;
        }

        if (value < 0) { value = 0; }
        else if (value > 1000) { value = 1000; }

        _sensorValues[i] = value;
      }
    }

    void calibrateSeconds(int seconds){
      //Reset values calibration
      for (uint8_t i = 0; i < _sensorCount; i++){
        _calibrationMaximum[i] = 0;
        _calibrationMinimum[i] = 8191;
      }

      int Ti = millis();
      while(millis() - Ti < seconds){
        calibrate();
      }
    }

    void calibrate(){
      uint16_t maxSensorValues[_sensorCount];
      uint16_t minSensorValues[_sensorCount];

      for (uint8_t j = 0; j < 10; j++){
        read();

        for (uint8_t i = 0; i < _sensorCount; i++){
          // set the max we found THIS time
          if ((j == 0) || (_sensorValues[i] > maxSensorValues[i])){
            maxSensorValues[i] = _sensorValues[i];
          }

          // set the min we found THIS time
          if ((j == 0) || (_sensorValues[i] < minSensorValues[i])){
            minSensorValues[i] = _sensorValues[i];
          }
        }
      }

      // record the min and max calibration values
      for (uint8_t i = 0; i < _sensorCount; i++){
        // Update maximum only if the min of 10 readings was still higher than it
        // (we got 10 readings in a row higher than the existing maximum).
        if (minSensorValues[i] > _calibrationMaximum[i]){
          _calibrationMaximum[i] = minSensorValues[i];
        }

        // Update minimum only if the max of 10 readings was still lower than it
        // (we got 10 readings in a row lower than the existing minimum).
        if (maxSensorValues[i] < _calibrationMinimum[i]){
          _calibrationMinimum[i] = maxSensorValues[i];
        }
      }
    }

    void read(){
      for (uint8_t i = 0; i < _sensorCount; i ++){
        _sensorValues[i] = 0;
      }

      for (uint8_t j = 0; j < _samplesPerSensor; j++){
        for (uint8_t i = 0; i < _sensorCount; i ++){
          uint16_t m = 1;
          for (uint8_t k = 0; k < _sensorInputsCount; k++){
            digitalWrite(_sensorPins[k],i&(m&0xFF));
            m=m*2;
          }
          _sensorValues[i] += analogRead(_signalPin);
        }
      }

      for (uint8_t i = 0; i < _sensorCount; i ++){
        _sensorValues[i] = (_sensorValues[i] + (_samplesPerSensor >> 1))/_samplesPerSensor;
      }
    }

    uint16_t getCalibrationMaximum(int index){
      return _calibrationMaximum[index];
    }

    uint16_t getcalibrationMinimum(int index){
      return _calibrationMinimum[index];
    }

    void setCalibrationMaximum(int index, uint16_t value){
      _calibrationMaximum[index] = value;
    }

    void setCalibrationMinimum(int index, uint16_t value){
      _calibrationMinimum[index] = value;
    }

    void printCalibration(){
      Serial.print("Maximun: ");
      for (int i=0; i < _sensorCount ;i++){
        Serial.print(i+1); 
        Serial.print(":");
        Serial.print(_calibrationMaximum[i]);
        Serial.print(" | ");
      }
      Serial.println();
      Serial.print("Minimun: ");
      for (int i=0; i < _sensorCount ;i++){
        Serial.print(i+1); 
        Serial.print(":");
        Serial.print(_calibrationMinimum[i]);
        Serial.print(" | ");
      }
      Serial.println();
    }

    void printRead(){
      read();
      for (int i=0; i < _sensorCount ;i++){
        Serial.print(i+1); 
        Serial.print(":");
        Serial.print(_sensorValues[i]);
        Serial.print(" | ");
      }
      Serial.println();
    }
};