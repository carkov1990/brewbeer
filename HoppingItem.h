class HoppingItem
{
  public: 
    HoppingItem(Button* btnUp, Button* btnDown, Button* btnOk)
    {
      _btnUp = btnUp;
      _btnDown = btnDown;
      _btnOk = btnOk;
    }

    String* Do()
    {

    }

  private:
    Button* _btnUp;
    Button* _btnDown;
    Button* _btnOk;
};