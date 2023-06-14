#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);

const byte led = 7;
const int rainSensor = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(rainSensor,INPUT);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  int status = digitalRead(rainSensor);
  digitalWrite(led,status);
  lcd.setCursor(0,0);
  if(status == HIGH){
    lcd.print("troi khong mua");
  }
  else
    lcd.print("troi dang mua do");
  delay(1000);
  lcd.clear();
}