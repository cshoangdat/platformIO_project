#include <Arduino.h>
#include <Servo.h>

Servo myServo;
#define enA A5
#define enB A4
#define in1 2
#define in2 3
#define in3 4
#define in4 5
#define servoPin 10
#define trigPin 12
#define echoPin 13
unsigned long distance;
byte limit = 30;
unsigned long distance_right;
unsigned long distance_left;
int time;

void setup(){
  myServo.attach(servoPin);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  myServo.write(60);
  delay(500);
  Serial.begin(9600);
}

void forward();
void backward();
void turnLeft();
void turnRight();
void stop();
void distance_mesure();
void turnServoToTheLeft();
void turnServoToTheRight();
void resetServo();

void loop(){
  distance = 0;
  distance_mesure();
  if(distance >= limit || distance == 0){
    forward();
  }
  else{
    stop();
    delay(500);
    turnServoToTheRight();
    distance_mesure();
    distance_right = distance;
    Serial.println(distance_right);
    resetServo();
    turnServoToTheLeft();
    distance_mesure();
    distance_left = distance;
    Serial.println(distance_left);
    resetServo();
    if(distance_left < 25 && distance_right < 25){
      backward();
      delay(400);
      stop();
      delay(400);
      Serial.println("dung va lui");
    }
    else{
      if(distance_right > distance_left){
      backward();
      delay(400);
      stop();
      delay(400);
      turnRight();
      delay(400);
      stop();
      delay(400);
      Serial.println("queo phai");
      }
      else{
      backward();
      delay(400);
      stop();
      delay(400);
      turnLeft();
      delay(400);
      stop();
      delay(400);
      Serial.println("queo trai");
      }
    }
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

void distance_mesure(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  time = pulseIn(echoPin,HIGH);
  distance = time/2/19.412;
}

void turnServoToTheLeft(){
  myServo.write(120);
  delay(1000);
}

void turnServoToTheRight(){
  myServo.write(0);
  delay(1000);
}

void resetServo(){
  myServo.write(60);
}