#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
int switchPin = 2;
int buttonPin = 4;
int slidePin = A0;
int buzzer = 3;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  pinMode(switchPin, INPUT_PULLUP); 
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Print a message on both lines of the LCD.
  while(digitalRead(buttonPin) == HIGH) {
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("To Start: ");
    lcd.setCursor(0,1);
    lcd.print("Press-it!");
  }
  randomSeed(analogRead(A1));
  lcd.clear();
  tone(buzzer, 1000);
  delay(250);
  noTone(buzzer);
  lcd.setCursor(0,0);
  lcd.print("Starting in: ");
  for(int i=3; i>0;i--) {
    lcd.setCursor(0,1);
    lcd.print(i);
    delay(1000);
  }
  lcd.clear();
  tone(buzzer, 5000);
  delay(100);
  noTone(buzzer);
  bool result = true;
  int interval = 2000;
  int score = 0;
  while(result && score < 100) {
    lcd.clear();
    int command = random(3);
    switch(command) {
      case 0:
        result = pressIt(interval);
        break;
      case 1:
        result = slideIt(interval);
        break;
      case 2:
        result = switchIt(interval);
        break;
    }
    if(result) {
      score++;
      lcd.setCursor(0,1);
      lcd.print(score);
      interval = interval - 5;
      delay(1000);
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("GAMEOVER");
      lcd.setCursor(0,1);
      lcd.print(score);
      delay(3000);
    }
  }
  lcd.clear();
  if(score == 99) {
    lcd.setCursor(0,0);
    lcd.print("YOU WIN!!!");
  }
} 

bool pressIt(int interval) {
  unsigned long previousMillis = millis();
  lcd.setCursor(0,0);
  lcd.print("Press-It!");
  tone(buzzer, 5000);
  bool switchIt = digitalRead(switchPin);
  int slideIt = analogRead(slidePin);
  while(millis() - previousMillis < interval) {
    if(digitalRead(buttonPin) == LOW) {
      noTone(buzzer);
      lcd.clear();
      return true;
    }
    else if(analogRead(slidePin) > slideIt + 10 || analogRead(slidePin) < slideIt - 10) {
      noTone(buzzer);
      lcd.clear();
      return false;
    }
    else if(digitalRead(switchPin) != switchIt) {
      noTone(buzzer);
      lcd.clear();
      return false;
    }
  }
  noTone(buzzer);
  lcd.clear();
  return false;
}

bool slideIt(int interval) {
  int previousV = analogRead(A0);
  bool switchIt = digitalRead(switchPin);
  bool pressIt = digitalRead(buttonPin);
  if(previousV <= 511) { //slide it right
    lcd.setCursor(0,0);
    lcd.print("Slide-It Left!");
    tone(buzzer, 500);
    unsigned long previou4sMillis = millis();
    while(millis() - previousMillis < interval) {
      if(analogRead(A0) >= 923) {
        noTone(buzzer);
        lcd.clear();
        return true;
      }
      else if(digitalRead(switchPin) != switchIt) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
      else if(digitalRead(buttonPin) != pressIt) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
    }
  }
  else {  //Slide it left
    lcd.setCursor(0,0);
    lcd.print("Slide-It Right!");
    tone(buzzer, 9000);
    unsigned long previousMillis = millis();
    while(millis() - previousMillis < interval) {
      if(analogRead(A0) <= 100) {
        noTone(buzzer);
        lcd.clear();
        return true;
      }
      else if(digitalRead(switchPin) != switchIt) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
      else if(digitalRead(buttonPin) != pressIt) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
    }
  }
  noTone(buzzer);
  lcd.clear();
  return false;
}

bool switchIt(int interval) {
  lcd.setCursor(0,0);
  lcd.print("Switch-It!");
  bool pressIt = digitalRead(buttonPin);
  int slideIt = analogRead(slidePin);
  tone(buzzer, 15000);
  unsigned long previousMillis = millis();
  if(digitalRead(switchPin) == HIGH) {
    while(millis() - previousMillis < interval) {
      if(digitalRead(switchPin) == LOW) {
        noTone(buzzer);
        lcd.clear();
        return true;
      }
      else if(digitalRead(buttonPin) != pressIt) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
      else if(analogRead(slidePin) > slideIt + 10 || analogRead(slidePin) < slideIt - 10) {
      noTone(buzzer);
      lcd.clear();
      return false;
      }
    }
  }
  else {
    while(millis() - previousMillis < interval) {
      if(digitalRead(switchPin) == HIGH) {
        noTone(buzzer);
        lcd.clear();
        return true;
      }
      else if(digitalRead(buttonPin) != pressIt) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
      else if(analogRead(slidePin) > slideIt + 10 || analogRead(slidePin) < slideIt - 10) {
        noTone(buzzer);
        lcd.clear();
        return false;
      }
    }
  }
  noTone(buzzer);
  lcd.clear();
  return false;
}
