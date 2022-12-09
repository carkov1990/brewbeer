#include <LiquidCrystal_I2C.h>

class LCD
{
  public : 
    void Init()
    {
      _lcd.init();           // инициализация
      _lcd.backlight();      // включить подсветку  
      _lines[0] = "";
      _lines[1] = "";
    }

    void Print(String line, byte index){
      if (index == 0){
        printfLine(line);
      }
      else {
        printsLine(line);
      }
    }

    void Print(String fLine, String sLine)
    {
      //Serial.println(fLine);
      printfLine(fLine);
      //Serial.println(sLine);
      printsLine(sLine);
    }

    void Clear()
    {
      _lcd.clear();
    }

    private : 
      String* _lines = new String[2];
      LiquidCrystal_I2C _lcd = LiquidCrystal_I2C(0x27, 16, 2);

      void printfLine(String fLine)
      {
        for (byte i = 0; i < 16; i++)
        {
            _lcd.setCursor(i, 0);  // столбец 1 строка 0
            if (fLine.length() <= i)
            {
                _lcd.print(" ");
            }
            else
            {
              if (fLine[i] != _lines[0][i])
              {
                _lcd.print(fLine[i]);
              }
            }
        }
        _lines[0] = fLine;
      }

      void printsLine(String sLine)
      {
        for (byte i = 0; i < 16; i++)
        {
            _lcd.setCursor(i, 1);  // столбец 1 строка 0
            if (sLine.length() <= i)
            {
                _lcd.print(" ");
            }
            else
            {
              if (sLine[i] != _lines[1][i])
              {
                _lcd.print(sLine[i]);
              }
            }
        }
        _lines[1] = sLine;
      }
};