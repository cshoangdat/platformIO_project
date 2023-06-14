#include <Arduino.h>
#define RPWM 10
#define LPWM 9
byte var_res_pin = A0;
byte PWM;

void setup() {
  // put your setup code here, to run once:
  pinMode(RPWM,OUTPUT);
  pinMode(LPWM,OUTPUT);
  Serial.begin(9600);
}

int var_res(){
  int var_res_value = analogRead(var_res_pin);
  PWM = map(var_res_value,0,1023,0,255);
  return PWM;
}

void H_Br_Forward(){
  analogWrite(RPWM,PWM);
  analogWrite(LPWM,0);
}
void H_Br_Backward(){
  analogWrite(LPWM,PWM);
  analogWrite(RPWM,0);
}
void H_Br_Stop(){
  analogWrite(RPWM,0);
  analogWrite(LPWM,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  var_res();
  Serial.println(var_res());
  if(Serial.available()) {
    int blue = Serial.read();
    switch(blue){
      case 'F':
      H_Br_Forward();
      break;
      case 'B':
      H_Br_Backward();
      break;
      case 'S':
      H_Br_Stop();
      break;
      default:
      H_Br_Stop();
      break;
    }
  }
}