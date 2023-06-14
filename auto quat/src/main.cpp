#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
LiquidCrystal_I2C lcd(0x27,16,2);
const int DHTpin = 6;
const int DHTType = DHT11;
DHT dht(DHTpin,DHTType);
byte fan = 7;
int count = 0;

byte degree[8]={
  0b11111,
  0b10001,
  0b10001,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Nhiet do: ");
  lcd.createChar(1,degree);
  lcd.setCursor(0,1);
  lcd.print("Quat: ");
  lcd.setCursor(8,1);
  lcd.print("dang tat");
  pinMode(7,OUTPUT);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = dht.readTemperature();
  delay(2000);
  lcd.setCursor(10,0);
  lcd.print(temp,1);
  lcd.write(1);
  lcd.print("C");
  lcd.setCursor(8,1);
  if(temp >= 30){
  lcd.print("dang bat");
  digitalWrite(fan,HIGH);
  count = 0;
  }
  else{
    count++;
    if(count >= 4){
    digitalWrite(fan,LOW);
    lcd.print("dang tat");
    }
  }
}