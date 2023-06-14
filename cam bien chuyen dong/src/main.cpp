#include <Arduino.h>
byte ledPin =  6;
byte PIR = 7;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(PIR, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte val = digitalRead(PIR);
  if(val == HIGH){
    digitalWrite(ledPin,HIGH);
    Serial.println("co nguoi qua");
  }
  else{
    digitalWrite(ledPin,LOW);
    Serial.println("khong co nguoi qua");
  }
}