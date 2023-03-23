//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int rightPin = 3;
const int upPin = 4;
const int sPin = 5;
const int p1TurnPin = 6;
const int p2TurnPin = 7;

int rightState = 0;
int upState = 0;
int sState = 0;
int p1 = 0;
int p2 = 0;
int place = 0;
int pause = 0;
int reset = 0;
int resetMid = 0;
bool mExit = true;

//Using modulus determine the current players turn and it'll keep track of the turns for the whole game
int pTurn = 0;
int sTurn = 0;

long int t10min = 0;
long int tmin = 0;
long int t10sec = 0;
long int tsec = 0;
long int inc = 0;
long int totalTime = 0;

long int p1Time = 0;
long int p2Time = 0;

int p1msec = 0;
int p1sec = 0;
int p1min = 0;
int p2msec = 0;
int p2sec = 0;
int p2min = 0;


int hold = 0;
int pauseState = true;

unsigned long previousMillis = 0;


void pMenu() {
  lcd.clear();
  lcd.print("Set up");
  lcd.setCursor(0,1);
  lcd.print(t10min);
  lcd.setCursor(1,1);
  lcd.print(tmin);
  lcd.setCursor(2,1);
  lcd.print(":");
  lcd.setCursor(3,1);
  lcd.print(t10sec);
  lcd.setCursor(4,1);
  lcd.print(tsec);
  lcd.setCursor(8,0);

  if(place == 0) {
    lcd.print("10min");
  } else if(place == 1) {
    lcd.print("1min");
  } else if(place == 2) {
    lcd.print("10sec");
  } else if(place == 3) {
    lcd.print("1sec");
  } else if(place == 4) {
    lcd.print("Inc");
  }


  lcd.setCursor(8,1);
  lcd.print(inc);
  
  lcd.setCursor(11,1);

  if(sTurn % 2 == 0) {
    lcd.print("WB");
  } else {
    lcd.print("BW");
  }
}

void convertP1() {
  reset = digitalRead(upPin);
  if((p1msec <= -1) && (p1sec >= 0)) {
    p1msec = 59;
    p1sec--;
  }

  if((p1sec <= -1) && (p1min >= 0)) {
    p1sec = 59;
    p1min--;
  }

  if(p1sec >= 60) {
    p1min++;
    p1sec -= 60;
  }

  if(p1min <= -1 && sTurn % 2 == 0) {
    lcd.clear();
    lcd.print("Black wins");
    while(reset == HIGH) {
      reset = digitalRead(upPin);
      delay(1);
      if(reset == LOW) {
        setup();
      }
    }
  } else if(p2min <= -1) {
    lcd.clear();
    lcd.print("White wins");
    while(reset == HIGH) {
      reset = digitalRead(upPin);
      delay(1);
      if(reset == LOW) {
        setup();
      }
    }
  }


}

void convertP2() {
  reset = digitalRead(upPin);
  
  if((p2msec <= -1) && (p2sec >= 0)) {
    p2msec = 59;
    p2sec--;
  }

  if((p2sec <= -1) && (p2min >= 0)) {
    p2sec = 59;
    p2min--;
  }

  if(p2sec >= 60) {
    p2min++;
    p2sec -= 60;
  }

  if(p2min <= -1 && sTurn % 2 == 1) {
    lcd.clear();
    lcd.print("White wins");
    while(reset == HIGH) {
      reset = digitalRead(upPin);
      delay(1);
      if(reset == LOW) {
        setup();
      }
    }
    
  } else if(p1min <= -1) {
    lcd.clear();
    lcd.print("Black wins");
    while(reset == HIGH) {
      reset = digitalRead(upPin);
      delay(1);
      if(reset == LOW) {
        setup();
      }
    }
    setup();
  }

}

void menu() {
  rightState = digitalRead(rightPin);
  upState = digitalRead(upPin);
  sState = digitalRead(sPin);
  p1 = digitalRead(p1TurnPin);
  p2 = digitalRead(p2TurnPin);

  if(rightState == LOW) {
    place += 1;
   }
  if(place > 4) {
    place = 0;
  }
  
  if(upState == LOW && place == 0) {
    t10min += 1;
  }
  if(upState == LOW && place == 1) {
    tmin += 1;
  }
  if(upState == LOW && place == 2) {
    t10sec += 1;
  }
  if(upState == LOW && place == 3) {
    tsec += 1;
  }
  if(upState == LOW && place == 4) {
    inc += 1;
  }



  if(t10min > 5) {
    t10min = 0;
  }
  if(tmin > 9) {
    tmin = 0;
  }
  if(t10sec > 5) {
    t10sec = 0;
  }
  if(tsec > 9) {
    tsec = 0;
  }


  if(p1 == LOW) {
    sTurn = 0;
  } else if (p2 == LOW) {
    sTurn = 1;
  }

  
if(sState == LOW) {
  mExit = 0;
}


}


void pGame() {
  lcd.clear();
  if(sTurn % 2 == 0) {
  lcd.print("White");
  lcd.setCursor(7,0);
  lcd.print("||");
  lcd.setCursor(7,1);
  lcd.print("||");
  lcd.setCursor(11,0);
  lcd.print("Black");
  } else {
  lcd.print("Black");
  lcd.setCursor(7,0);
  lcd.print("||");
  lcd.setCursor(7,1);
  lcd.print("||");
  lcd.setCursor(11,0);
  lcd.print("White");
  }
}

void pCurrent() {
  if(sTurn % 2 == 0) {
  lcd.setCursor(0,1);
  if(p1min < 10) {
    lcd.print(0);
    lcd.print(p1min);
  } else {
    lcd.print(p1min);
  }
  lcd.print(":");
  if(p1sec < 10) {
    lcd.print(0);
    lcd.print(p1sec);
  } else {
    lcd.print(p1sec);
  }

  
  lcd.setCursor(11,1);
  if(p2min < 10) {
    lcd.print(0);
    lcd.print(p2min);
  } else {
    lcd.print(p2min);
  }
  lcd.print(":");
    if(p2sec < 10) {
      lcd.print(0);
      lcd.print(p2sec);
    } else {
      lcd.print(p2sec);
    }



     
  }
  
  else {
    lcd.setCursor(0,1);
  if(p2min < 10) {
    lcd.print(0);
    lcd.print(p2min);
  } else {
    lcd.print(p2min);
  }
  lcd.print(":");
  if(p2sec < 10) {
    lcd.print(0);
    lcd.print(p2sec);
  } else {
    lcd.print(p2sec);
  }
  lcd.setCursor(11,1);
  if(p1min < 10) {
    lcd.print(0);
    lcd.print(p1min);
  } else {
    lcd.print(p1min);
  }
  lcd.print(":");
    if(p1sec < 10) {
      lcd.print(0);
      lcd.print(p1sec);
    } else {
      lcd.print(p1sec);
    }
  }
}

void setup()
{
  pauseState = true;
  mExit = true;
  pinMode(p1TurnPin,INPUT_PULLUP);
  pinMode(p2TurnPin,INPUT_PULLUP);
  pinMode(rightPin,INPUT_PULLUP);
  pinMode(upPin,INPUT_PULLUP);
  pinMode(sPin,INPUT_PULLUP);
  lcd.init();// initialize the lcd 
  Serial.begin(9600);
  lcd.backlight();
  lcd.print("Chess Clock");
  delay(2000);

  while(mExit) {
    menu();
    pMenu();
    delay(150);
  }
  
  t10min = t10min * 10;
  t10sec = t10sec * 10;
  
  
  p1min = t10min + tmin;
  p2min = t10min + tmin;
  p1sec = t10sec + tsec;
  p2sec = t10sec + tsec;

  


  pGame();

  delay(150);
}

void loop()
{
      p1 = digitalRead(p1TurnPin);
      p2 = digitalRead(p2TurnPin);
      pause = digitalRead(sPin);

      //Code to start the game by white moving then the clock starts
      while(pauseState == true) {
        p1 = digitalRead(p1TurnPin);
        p2 = digitalRead(p2TurnPin);
        pCurrent();
        delay(1);
        if(sTurn % 2 == 1) {
          if(p1 == LOW) {
          pauseState = false;
          pTurn++;
        }
        } else if(sTurn % 2 == 0){
          if(p2 == LOW) {
            pauseState = false;
          }
        }
     }

  unsigned long currentMillis = millis();
  //Using blink without delay code to make no delay when turn is switched
  //Immediately switch the players turn, before there was a delay until the second ended.
  if(currentMillis - previousMillis >= 10) {
    previousMillis = currentMillis;
     if(sTurn % 2 == 0) {
      if(pTurn % 2 == 0) {
        p1msec -= 1;
      } else {
        p2msec -= 1;
      }
     } else {
      if(pTurn % 2 == 1) {
        p1msec -= 1;
      } else {
        p2msec -= 1;
      }
     }
  }


  if(p1 == LOW && pTurn % 2 == 0) {
    pTurn += 1;
    if(sTurn % 2 == 0) {
      p1sec += inc;
    } else {
      p2sec += inc;
    }
    
  }
  if(p2 == LOW && pTurn % 2 != 0) {
    pTurn += 1;
    if(sTurn % 2 == 1) {
      p1sec += inc;
    } else {
      p2sec += inc;
    }
  }


  

  //Code to pause the timer

  if(pause == LOW) {
    pauseState = true;
    delay(200);
    while(pauseState == true) {
    pause = digitalRead(sPin);
    resetMid = digitalRead(upPin);
    
    pCurrent();
    delay(1);
    if(pause == LOW) {
      pauseState = false;
      delay(200);
    }
    if(resetMid == LOW) {
      setup();
      delay(200);
    }
  }
  }

  
  pCurrent();
  convertP1();
  convertP2();
}
