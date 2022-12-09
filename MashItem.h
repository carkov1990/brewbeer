#include <microDS18B20.h>
MicroDS18B20<12> sensor;  // датчик на D2

class MashItem
{
  public: 
    MashItem(Button* btnUp, Button* btnDown, Button* btnOk)
    {
      _btnUp = btnUp;
      _btnDown = btnDown;
      _btnOk = btnOk;
    }

    void Do(String* lines)
    {
      if (currentState == 0){ //0- выбор кол-ва пауз
        SetMashCount(lines);
      }
      else if (currentState == 1){ //1- выбор температуры паузы
        SetTempPause(lines);
      }
      else if (currentState == 2){ //2- выбор времени паузы
        SetTimePause(lines);
      } 
      else if (currentState == 3){ //3- нагрев
        Heating(lines);
      }
    }

  private:
    Music music = Music(SPEAKER);
    byte iterator = 0;
    byte currentState = 0; 
    byte mashCount = 1;
    byte* mashTemp = new byte[10] { 45, 52, 62, 72, 78, 0, 0, 0, 0, 0 };
    byte* mashTime = new byte[10] { 5, 15, 60, 15, 5, 0, 0, 0, 0, 0 };
    long time = 0;
    long lastTick = 0; 
    long lastTemp = 0;
    long releOn = 0;
    long maintenance = 0;
    bool isFinished = false;
    String timeStr = "00:00:00 00C T00";
    String tenStr =  "pw 100 off 99.99";
    byte power = 100;
    byte powerTime = 20;
    float sensorTemp = 0;    
  
    Button* _btnUp;
    Button* _btnDown;
    Button* _btnOk;

    void SetMashCount(String* lines){
      if (_btnUp->click()){
        if (mashCount == 10){
          mashCount = 10;
        }
        else {
          mashCount++;
        }
      }else if (_btnDown->click()) {
        if (mashCount == 1){
          mashCount = 1;
        }
        else {
          mashCount--;
        }
      }else if (_btnOk->click()){
        currentState++;
      }
      lines[0] = "Select mash cnt";
      lines[1] = String(mashCount);
    }

    void SetTempPause(String* lines)
    {
      if (_btnUp->click()){
        if (mashTemp[iterator] == 100){
          mashTemp[iterator] = 100;
        }
        else {
          mashTemp[iterator]++;
        }
      }else if (_btnDown->click()) {
        if (mashTemp[iterator] == 1){
          mashTemp[iterator] = 1;
        }
        else {
          mashTemp[iterator]--;
        }
      }else if (_btnOk->click()){
        currentState++;
      }
      lines[0] ="Select mash t";
      lines[1] = String(mashTemp[iterator]);
    }

    void SetTimePause(String* lines)
    {
      if (_btnUp->click()){
        if (mashTime[iterator] == 90){
          mashTime[iterator] = 90;
        }
        else {
          mashTime[iterator]++;
        }
      }else if (_btnDown->click()) {
        if (mashTime[iterator] == 1){
          mashTime[iterator] = 1;
        }
        else {
          mashTime[iterator]--;
        }
      }else if (_btnOk->click()){
        if (iterator+1 == mashCount){
          lastTick = millis();
          releOn = millis();
          iterator = 0;
          currentState++;
        }
        else {
          iterator++;
          currentState--;
        }
        
      }
      lines[0] ="Select mash time";
      lines[1] = String(mashTime[iterator]);
    }

    void Heating(String* lines)
    {
      if (millis()-lastTick >= 1000){
        lastTick = millis();
        time++;
      }
      if (millis() - lastTemp >= 800){
        lastTemp = millis();
        // читаем прошлое значение
        if (sensor.readTemp())
        {
          sensorTemp = sensor.getTemp();
          CurrentTemp(sensorTemp);
        }
        sensor.requestTemp();
      }
      if (_btnUp->click()){
        if (power < 100){
          power = power + 25;
          powerTime = powerTime + 5;
        }
      }else if (_btnDown->click()) {
        if (power > 25){
          power = power - 25;
          powerTime = powerTime - 5;
        }
      }else if (_btnOk->click()){
      }

      if (iterator == mashCount)
      {
          isFinished = true;
          music.Play();
          iterator--;
      }

      if (sensorTemp < mashTemp[iterator]){
        Rele();
      }
      else{
        Maintenance();        
      }
      GetTime();
      MashTemp();
      MashTime();
      TenPower();
      lines[0] = timeStr;
      lines[1] = tenStr;
    }

    void Maintenance()
    {
        digitalWrite(RELE, LOW);
        tenStr[8] = 'f';
        tenStr[9] = 'f';
        if (isFinished)
        {
          iterator++;
          mashTime[iterator] = 0;
        }
        if (maintenance == 0)
        {
          maintenance = millis();
        }
        else if ((millis() - maintenance) >= 60000)
        {
          maintenance = millis();
          mashTime[iterator]--;
          if (mashTime[iterator] == 0){
            maintenance = 0;
            iterator++;
          }
        }
    }

    void Rele()
    {
      if (millis() - releOn < powerTime * 1000){
        digitalWrite(RELE, HIGH);
        tenStr[8] = 'n';
        tenStr[9] = ' ';
      }
      else if (millis() - releOn > 20000) {
        releOn = millis();
      }
      else {
        digitalWrite(RELE, LOW);
        tenStr[8] = 'f';
        tenStr[9] = 'f';
      }
    }

    void MashTemp()
    {
      timeStr[9] = mashTemp[iterator]/10+48;
      timeStr[10] = mashTemp[iterator]%10+48;
    }

    void MashTime()
    {
      timeStr[14] = mashTime[iterator]/10+48;
      timeStr[15] = mashTime[iterator]%10+48;
    }

    void GetTime()
    {
      byte h = (time/3600); 
        timeStr[0] = h/10+48;
        timeStr[1] = h%10+48;
		  byte m = (time -(3600*h))/60;
        timeStr[3] = m/10+48;
        timeStr[4] = m%10+48;
	  	byte s = (time -(3600*h)-(m*60));
        timeStr[6] = s/10+48;
        timeStr[7] = s%10+48;
    }

    void TenPower()
    {
      tenStr[3] = power/100+48;
      tenStr[4] = power%100/10+48;
      tenStr[5] = power%10+48;
    }

    void CurrentTemp(float t)
    {
      int tInt = t * 100;
      tenStr[11] = tInt/1000+48;
      tInt = tInt % 1000;
      tenStr[12] = tInt/100+48;
      tInt = tInt % 100;
      tenStr[14] = tInt/10+48;;
      tenStr[15] = tInt%10+48;;;
    }
};