#include <SPI.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <RH_ASK.h>
IRsend irsend;
RH_ASK driver;
const int b1 = 8, b2 = 9, irled = 8, roled = 7;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int freq = 100;
int clock1 = 0;
int MB = 10;
int bleft = 10;
int isrelo = 0;
int retimer = 0;

int health = 7;
int onb1 = 0;
int onb2 = 0;
int score = 0;
int timer = 0;
int i;
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(b2, INPUT);
  pinMode(b1, INPUT);
  pinMode(irled, OUTPUT);
  // Print a message to the LCD.
  Serial.begin(9600);
}

void loop() {
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      // Message with a good checksum received, dump it.
      Serial.print("Message: ");
      if (score < 10)
      {
        score += 1;
      }
      Serial.println((char*)buf);
    }
  int but1 = digitalRead(b1);
  int but2 = digitalRead(b2);
  Serial.println(but1);
  if (but1 == 1 && onb1 == 0)
  {
    if (isrelo == 0)
    {
      Serial.println("1P");
      if (bleft > 0)
      {
        Serial.println("FIRE");
                digitalWrite(irled, HIGH);
                delay(200);
                digitalWrite(irled, LOW);
//        for (i = 0; i < 100; i++) {
//          irsend.sendSony(0xa90, 12);
//          delay(40);
//        }
        bleft -= 1;
        if (bleft == 0)
        {
          bleft = 10;
          retimer = 500;
          isrelo = 1;
        }
      }
    }
    onb1 = 1;
  }
  if (but1 == 0)
  {
    onb1 = 0;
  }
  if (but2 == 1 && onb2 == 0)
  {
    Serial.println("2P");
    bleft = 10;
    retimer = 500;
    isrelo = 1;
    onb2 = 1;
  }
  if (but2 == 0)
  {
    onb2 = 0;
  }
  if (retimer > 0)
  {
    digitalWrite(roled, HIGH);
    retimer -= 1;
  }
  else {
    digitalWrite(roled, LOW);
    isrelo = 0;
  }
  if (clock1 > freq)
  {
    //    lcd.clear();
    //    delay(10);
    lcd.setCursor(0, 0);
    lcd.print("Health:[");
    for (i = 1; i <= health; i++)
    {
      lcd.print(char(255));
    }
    for (i = health + 1; i <= 7; i++)
    {
      lcd.print(" ");
    }
    lcd.setCursor(15, 0);
    lcd.print("]");
    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print(score);
    lcd.setCursor(8, 1);
    if (retimer > 0)
    {
      lcd.print(" Reload");
    }
    else {
      ;
      lcd.print(" A:");
      lcd.print(bleft);
      lcd.print("  ");
    }
    clock1 = 0;
  }
  clock1 += 1;
}
