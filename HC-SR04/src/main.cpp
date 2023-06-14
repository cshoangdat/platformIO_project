#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

const int Trig = 3;
const int Echo = 2;
const byte led = 4;

int getDistance(){
  digitalWrite(Trig,LOW);
  delayMicroseconds(3);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(5);
  digitalWrite(Trig,LOW);
  unsigned long duration = pulseIn(Echo,HIGH);
  int distance = (int)(duration/29.4/2);
  return distance;
}

void setup(){
  pinMode(led, OUTPUT);
  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop(){
  int distance = getDistance();
  lcd.setCursor(0,0);
  lcd.print("khoang cach: ");
  lcd.setCursor(0,1);
  lcd.print(distance);
  delay(500);
  if(distance<50){
    delay(2000);
    if(distance<50){
      digitalWrite(led,HIGH);
    }
  }
  else
  digitalWrite(led,LOW);
  lcd.clear();
}