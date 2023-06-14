#include <Arduino.h>
#define enA A5
#define enB A4
#define in1 2
#define in2 3
#define in3 4
#define in4 5
#define sensorLeft A2
#define sensorRight A1
byte sensorLeft_value;
byte sensorRight_value;

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorLeft, INPUT);
  pinMode(sensorRight, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);
}
void forward();
void backward();
void stop();
void turnLeft();
void turnRight();

void loop() {
  // put your main code here, to run repeatedly:
  sensorRight_value = digitalRead(sensorRight);
  sensorLeft_value = digitalRead(sensorLeft);
  if(sensorRight_value == 0 && sensorLeft_value == 0)
  forward();
  else if(sensorLeft_value == 1 && sensorRight_value == 0)
  turnLeft();
  else if(sensorRight_value == 1 && sensorLeft_value == 0)
  turnRight();
  else
  stop();
}

void forward(){
  analogWrite(enA,150);
  analogWrite(enB,120);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void backward(){
  analogWrite(enA,150);
  analogWrite(enB,150);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void turnLeft(){
  analogWrite(enA,150);
  analogWrite(enB,150);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void turnRight(){
  analogWrite(enA,150);
  analogWrite(enB,150);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
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