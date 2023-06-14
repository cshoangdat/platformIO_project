#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("dien ap ra: ");
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int varRes = analogRead(A0);
  int voltage = map(varRes,0,1023,0,5000);
  Serial.write(voltage);
  lcd.setCursor(3,1);
  lcd.write(Serial.read());
  delay(2000);
 }