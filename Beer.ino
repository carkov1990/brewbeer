#define SPEAKER 10
#define BUTTON_OK 2
#define BUTTON_UP 3
#define BUTTON_DOWN 4
#define RELE 13

#include "LCD.h"
#include "Music.h"
#include "ModeItem.h"
#include "MashItem.h"
#include "HoppingItem.h"
#include "RecipeItem.h"

LCD lcd = LCD();
Music music = Music(SPEAKER);
Button btnUp = Button(BUTTON_UP);
Button btnDown = Button(BUTTON_DOWN);
Button btnOk = Button(BUTTON_OK);

ModeItem modeItem = ModeItem(&btnUp, &btnDown, &btnOk);
MashItem mashItem = MashItem(&btnUp, &btnDown, &btnOk);
HoppingItem hoppingItem = HoppingItem(&btnUp, &btnDown, &btnOk);
RecipeItem recipeItem = RecipeItem(&btnUp, &btnDown, &btnOk);

String step[] = {"Mode", "Mashing", "Hopping", "Recipe"};
String mode[] = { "Mashing", "Hopping", "Recipe" };
byte currentState = 0;
String* lcdLines = new String[2] { "", "" };

void setup()
{
  Serial.begin(9600);
  lcd.Init();
  lcd.Print("Welcome to", "Brewbeer machine");
  music.Play();
  lcd.Print("Select mode:", mode[0]);
  pinMode(RELE, OUTPUT);
}

void loop()
{
  if (currentState == 0){
      byte cMode = modeItem.Mode;
      currentState = modeItem.Do();
      if (cMode != modeItem.Mode)
      {
        lcd.Print(mode[modeItem.Mode], 1);
      }
  }
  if (currentState == 1){
    mashItem.Do(lcdLines);
    lcd.Print(lcdLines[0],lcdLines[1]);
  }
  if (currentState == 2){
    String* lines = hoppingItem.Do();
    lcd.Print(lines[0], lines[1]);
  }
  if (currentState == 3){
    String* lines = recipeItem.Do();
    lcd.Print(lines[0], lines[1]);
  }
}