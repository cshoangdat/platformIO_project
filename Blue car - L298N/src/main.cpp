#include <Arduino.h>
#define enA A0
#define enB A1
#define in1 4
#define in2 5
#define in3 6
#define in4 7
char blue;

void setup(){
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
}

void forward(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}
void backward(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}
void right(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}
void left(){
  analogWrite(enA,255);
  analogWrite(enB,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}
void stop(){
  analogWrite(enA,0);
  analogWrite(enB,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void loop(){
  if(Serial.available()>0){
    blue = Serial.read();
    switch(blue){
      case 'F':
      forward();
      Serial.print("forward");
      break;
      case 'B':
      backward();
      Serial.print("backward");
      break;
      case 'L':
      left();
      Serial.print("left");
      break;
      case 'R':
      right();
      Serial.print("right");
      break;
      case 'S':
      Serial.print("stop");
      stop();
      break;
    }
  }
}

