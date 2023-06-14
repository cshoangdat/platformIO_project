#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte state = digitalRead(3);
  digitalWrite(4,state);
}