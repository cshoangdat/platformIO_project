#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
const int dhtPin = 7;
const int dhtType = DHT11;
DHT dht(dhtPin, dhtType);

byte degree[8] = {
  0b01110,
  0b01010,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup(){
  lcd.init();
  lcd.backlight();
  lcd.createChar(1,degree);
  lcd.setCursor(0,0);
  lcd.print("Nhiet do:");
  lcd.setCursor(0,1);
  lcd.print("Do am:");
  dht.begin();
}

void loop(){
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if(isnan(temperature)||isnan(humidity)){}
  else{
  lcd.setCursor(10,0);
  lcd.print(round(temperature));
  lcd.print (" ");
  lcd.write(1);
  lcd.print("C");
  lcd.setCursor(10,1);
  lcd.print(round(humidity));
  lcd.print(" %");
  }
}