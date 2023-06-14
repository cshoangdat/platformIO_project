#include <Arduino.h>
unsigned long time ;
unsigned long startTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  startTime = millis();
  pinMode(4,OUTPUT);
  digitalWrite(4,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Time: ");
  if(millis() - startTime >500){
    if(digitalRead(4) == LOW)
    digitalWrite(4,1);
  else
  digitalWrite(4,0);
  }
  startTime = millis();
}