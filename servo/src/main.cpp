#include <Arduino.h>
#include <Servo.h>
Servo myservo;
byte servoPin = 3;
int bientro;

void setup(){
  myservo.attach(servoPin);
  pinMode(A0,INPUT);
  Serial.begin(9600);
}

void loop(){
  bientro = analogRead(A0);
  int servo = map(bientro,0,1023,0,180);
  Serial.println(servo);
  myservo.write(servo);
  delay(100);
}