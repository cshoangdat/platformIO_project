#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D4, D5); //RX,TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  if(Serial.available()>0){
    String input = Serial.readString();
    mySerial.println(input);
  }
  if(mySerial.available()>1){
    String input = mySerial.readString();
    Serial.println(input);
  }
  delay(20);
  // put your main code here, to run repeatedly:
}