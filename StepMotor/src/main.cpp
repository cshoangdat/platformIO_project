#include <Arduino.h>
#include <SimpleKalmanFilter.h>

/*
  SimpleKalmanFilter(e_mea,e_est,q);
  e_mea: mức độ không chắc chắn của phép đo (mong đợi vào phép đo)
  e_est: ước tính không chắc chắn (set up giống e_mea thư viện sẽ tự hiệu chỉnh)
  q: phương sai - thường là lấy từ 0.001 đến 1 (thường là 0.001) tùy
*/

SimpleKalmanFilter pressureKalmanFilter(2, 2, 0.01);

int i,n;
int var_res = A0;
int currentAngle = 0, angle = 0, stepAngle = 90;
#define enable 3
#define step 4
#define direction 5
void setup() {
  // put your setup code here, to run once:
  pinMode(enable, OUTPUT);
  pinMode(step,OUTPUT);
  pinMode(direction,OUTPUT);
  digitalWrite(enable,LOW);
  pinMode(var_res,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // int var_res_value = analogRead(var_res);
  // int var_res_value_correction = pressureKalmanFilter.updateEstimate(var_res_value);
  // int speed = map(var_res_value_correction,0,1023,200,4000);
  // int currentAngle = map(var_res_value_correction,0,1023,0,200);
  // Serial.println(currentAngle);
  // digitalWrite(direction, HIGH);
  // for(i = 0; i <= currentAngle; i++){
  //   digitalWrite(step,HIGH);
  //   delayMicroseconds(speed);
  //   digitalWrite(step, LOW);
  //   delayMicroseconds(speed);
  // }
  // angle = currentAngle;
    digitalWrite(direction, HIGH);
    for(i = 0; i < 200; i++){
      digitalWrite(step,HIGH);
      delayMicroseconds(200);
      digitalWrite(step, LOW);
      delayMicroseconds(200);
    } 
}