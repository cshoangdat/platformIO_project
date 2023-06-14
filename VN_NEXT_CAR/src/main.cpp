#include <Arduino.h>
#define IN3_1 2
#define IN4_1 3
#define IN3_2 4
#define IN4_2 5
#define IN3_3 6
#define IN4_3 7
#define IN3_4 9
#define IN4_4 8
char blue;
void setup() {
  // put your setup code here, to run once:
  pinMode(IN3_1, OUTPUT);
  pinMode(IN3_2, OUTPUT);
  pinMode(IN3_3, OUTPUT);
  pinMode(IN3_4, OUTPUT);
  pinMode(IN4_1, OUTPUT);
  pinMode(IN4_2, OUTPUT);
  pinMode(IN4_3, OUTPUT);
  pinMode(IN4_4, OUTPUT);
  Serial.begin(9600);
  digitalWrite(IN3_1, LOW);
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN3_3, LOW);
  digitalWrite(IN3_4, LOW);
  digitalWrite(IN4_1, LOW);
  digitalWrite(IN4_2, LOW);
  digitalWrite(IN4_3, LOW);
  digitalWrite(IN4_4, LOW);
}

void forward() {
      digitalWrite(IN3_1, HIGH);
      digitalWrite(IN4_1, LOW);
      digitalWrite(IN3_2, HIGH);
      digitalWrite(IN4_2, LOW);
      digitalWrite(IN3_3, HIGH);
      digitalWrite(IN4_3, LOW);
      digitalWrite(IN3_4, HIGH);
      digitalWrite(IN4_4, LOW);
}

void backward() {
      digitalWrite(IN3_1, LOW);
      digitalWrite(IN4_1, HIGH);
      digitalWrite(IN3_2, LOW);
      digitalWrite(IN4_2, HIGH);
      digitalWrite(IN3_3, LOW);
      digitalWrite(IN4_3, HIGH);
      digitalWrite(IN3_4, LOW);
      digitalWrite(IN4_4, HIGH);
}

void left() {
      digitalWrite(IN3_1, LOW);
      digitalWrite(IN4_1, HIGH);
      digitalWrite(IN3_2, HIGH);
      digitalWrite(IN4_2, LOW);
      digitalWrite(IN3_3, HIGH);
      digitalWrite(IN4_3, LOW);
      digitalWrite(IN3_4, LOW);
      digitalWrite(IN4_4, HIGH);
}

void right(){
      digitalWrite(IN3_1, HIGH);
      digitalWrite(IN4_1, LOW);
      digitalWrite(IN3_2, LOW);
      digitalWrite(IN4_2, HIGH);
      digitalWrite(IN3_3, LOW);
      digitalWrite(IN4_3, HIGH);
      digitalWrite(IN3_4, HIGH);
      digitalWrite(IN4_4, LOW);
}

void stop(){
      digitalWrite(IN3_1, LOW);
      digitalWrite(IN4_1, LOW);
      digitalWrite(IN3_2, LOW);
      digitalWrite(IN4_2, LOW);
      digitalWrite(IN3_3, LOW);
      digitalWrite(IN4_3, LOW);
      digitalWrite(IN3_4, LOW);
      digitalWrite(IN4_4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    blue = Serial.read();
    switch(blue){
      case 'F':
        forward();
      break;
      case 'B':
        backward();
      break;
      case 'L':
        left();
      break;
      case 'R':
        right();
      break;
      case 'S':
        stop();
      break;
      default:
      stop();
      break;
    }
    Serial.println(blue);
  }

}