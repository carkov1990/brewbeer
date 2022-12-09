#include "Button.h"

class ModeItem
{
  public: 
    byte Mode = 0;

    ModeItem(Button* btnUp, Button* btnDown, Button* btnOk)
    {
      _btnUp = btnUp;
      _btnDown = btnDown;
      _btnOk = btnOk;
    }

    byte Do()
    {
      if (_btnUp->click()){
        Serial.println("Click up");
        if (Mode==2){
          Mode = 0;
        }
        else {
          Mode++;
        }
      }else if (_btnDown->click()) {
        Serial.println("Click down");
        if (Mode==0){
          Mode = 2;
        }
        else {
          Mode--;
        }
      }else if (_btnOk->click()){
        Serial.println("Click ok");
        return Mode+1;
      }
      return 0;
    } 

  private:
    Button* _btnUp;
    Button* _btnDown;
    Button* _btnOk;
};