#include <Arduino.h>
byte LPWM = 9;
byte RPWM = 10;
int var_res = A0;
void setup() {
  // put your setup code here, to run once:
  pinMode(RPWM,OUTPUT);
  pinMode(LPWM,OUTPUT);
  pinMode(var_res,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int var_res_value = analogRead(var_res);
  byte PWM = map(var_res_value,0,1023,0,255);
  analogWrite(LPWM,PWM);
  analogWrite(RPWM,0);
  Serial.println(PWM);
}