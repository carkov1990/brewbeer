class Music
{
  public: 
    Music(int speaker){
      _speaker = speaker;
    }

    void Play()
    {
      int times[] = {
        350, 350, 350, 250, 100, 350, 250, 100, 700,
        350, 350, 350, 250, 100, 350, 250, 100, 700,
        350, 250, 100, 350, 250, 100, 100, 100, 450,
        150, 350, 250, 100, 100, 100, 450,
        150, 350, 250, 100, 750
      };

      int notes [] = {
        392, 392, 392, 311, 466, 392, 311, 466, 392,
        587, 587, 587, 622, 466, 369, 311, 466, 392,
        784, 392, 392, 784, 739, 698, 659, 622, 659,
        415, 554, 523, 493, 466, 440, 466,
        311, 369, 311, 466, 392
      };

      for (int i = 0; i < 10; i++){
        tone(_speaker, notes[i], times[i]*2);
        delay(times[i]*2);
        noTone(_speaker);
      }
    }

  private:
    int _speaker ;
};