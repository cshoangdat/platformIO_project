#include <Arduino.h>
#define ENB 5
#define IN3 8
#define IN4 7
char blue;

void setup() {
  // put your setup code here, to run once:
  pinMode(ENB,OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENB,0);
  pinMode(blue, INPUT);
  Serial.begin(9600);
}
void forward();
void backward();
void stop();

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
   blue = Serial.read();
    switch(blue){
      case 'F':
      forward();
      break;
      case 'B':
      backward();
      break;
      default:
      stop();
      break;
    }
  }
  
}

void forward() {
  analogWrite(ENB,255);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void backward() {
  analogWrite(ENB,255);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void stop(){
  analogWrite(ENB,0);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}