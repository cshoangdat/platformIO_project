#include <Arduino.h>
const byte relay = 7;
const byte button = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);
  pinMode(relay,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte buttonstate = digitalRead(button);
  if(buttonstate == 0 ){
    delay(500);
    if(buttonstate == 0)
    digitalWrite(relay,HIGH);
  }
  else
  digitalWrite(relay, LOW);
}