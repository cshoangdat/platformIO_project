#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = analogRead(A0);
  float temperature = (float)temp*500/1024;
  Serial.print(temperature);
  Serial.println(" do C");
  lcd.setCursor(0,0);
  lcd.print("nhiet do phong:");
  lcd.setCursor(0,1);
  lcd.print(temperature);
  delay(1000);
  lcd.clear();
}