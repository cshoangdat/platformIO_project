#include <Arduino.h>
#define enA A5
#define enB A4
#define in1 2
#define in2 3
#define in3 4
#define in4 5
#define sensorLeft 6
#define sensorRight 11
#define sensorBack 7
byte sensorLeft_value;
byte sensorRight_value;
byte sensorBack_value;

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorLeft,INPUT);
  pinMode(sensorRight,INPUT);
  pinMode(sensorBack,INPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  Serial.begin(9600);
}

void forward();
void backward();
void stop();
void turnLeft();
void turnRight();
void stop_and_backward();
void delayTurnRight();
void delayTurnLeft();

void loop() {
  // put your main code here, to run repeatedly:
  sensorLeft_value = digitalRead(sensorLeft);
  sensorRight_value = digitalRead(sensorRight);
  sensorBack_value = digitalRead(sensorBack);
  if(sensorRight_value == 0 && sensorLeft_value == 0){
    forward();
    Serial.println("di thang");
  }
  else if(sensorRight_value == 0 && sensorLeft_value ==1){
    stop_and_backward();
    delayTurnRight();
    Serial.println("queo phai");
  }
  else if(sensorRight_value == 1 && sensorLeft_value == 0){
    stop_and_backward();
    delayTurnLeft();
    Serial.println("queo trai");
  }
  else{
    stop_and_backward();
    delayTurnRight();
    Serial.println("dung va lui");
  }
}

void forward(){
  analogWrite(enA,150);
  analogWrite(enB,150);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void backward(){
  analogWrite(enA,150);
  analogWrite(enB,150);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void turnRight(){
  analogWrite(enA,150);
  analogWrite(enB,150);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void turnLeft(){
  analogWrite(enA,150);
  analogWrite(enB,150);
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

void stop_and_backward(){ //delay lui va ktr cam bien sau 
  unsigned long startTime = millis();
  while(sensorBack_value == 0 && millis() - startTime <= 300){
    backward();
    digitalRead(sensorBack_value);
  }
  stop();
  delay(500);
}

void delayTurnRight(){
 unsigned long startTime = millis();
 while(sensorRight_value == 0 && millis() - startTime <= 500){
   turnRight();
   sensorRight_value = digitalRead(sensorRight);
 }
 if(sensorRight_value == 1 || millis() - startTime > 500){
   stop();
   delay(500);
 }
}

void delayTurnLeft(){
 unsigned long startTime = millis();
 while(sensorLeft_value == 0 && millis() - startTime <= 500){
   turnRight();
   sensorLeft_value = digitalRead(sensorLeft);
 }
 if(sensorLeft_value == 1 || millis() - startTime > 500){
   stop();
   delay(500);
 }
}
