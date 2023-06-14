#include <Arduino.h>
byte button1 = 8;
byte button2 = 9;
#define enA 10
#define in1 11
#define in2 12

void setup() {
  // put your setup code here, to run once:
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(enA,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void left_to_right(){
  analogWrite(enA,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2, LOW);
}

void right_to_left(){
  analogWrite(enA,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  left_to_right();
  byte button1_state = digitalRead(button1);
  Serial.println(button1_state);
  byte button2_state = digitalRead(button2);
  Serial.println(button2_state);
  if(button1_state == 1){
    left_to_right();
  }
  if(button2_state == 1){
    right_to_left();
  }
}

