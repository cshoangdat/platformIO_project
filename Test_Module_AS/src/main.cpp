#include <Arduino.h>

#include <Wire.h>
#include <BH1750.h>
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter bo_loc(2, 2, 0.01);

BH1750 lightMeter;


// //Giá trị điện áp 
// int sensorPin = A0;    // Chỉ định chân A0 làm đầu vào cho cảm biến
// int sensorValue = 0;   // Biến để lưu giá trị cảm biến
// float voltage = 0.0;   // Biến để lưu giá trị điện áp

void setup() {
  Serial.begin(9600);  // Khởi động giao tiếp Serial với tốc độ 9600 baud
  // pinMode(A0,INPUT);
}

void loop() {
  // sensorValue = analogRead(sensorPin);  // Đọc giá trị cảm biến từ chân A0
  // // voltage = (sensorValue * 5.0) / 1024.0;
  // // float lux = voltage*237;  // Tính giá trị điện áp từ giá trị cảm biến
  // float lux = 0;
  // if(lux <=400) lux =map(sensorValue,0,1023,1,950);
  // else lux = map(sensorValue,0,1023,1,900);
  // Serial.println(lux);  // Gửi giá trị điện áp đến Serial Monitor
  // delay(100);  // Chờ 100ms để đọc giá trị cảm biến tiếp theo
  int value = analogRead(3);   //đọc giá trị điện áp ở chân A0 
                                //(value luôn nằm trong khoảng 0-1023)
  Serial.println(value);        //xuất ra giá trị vừa đọc
  
  int voltage;
  voltage = map(value,0,1023,0,220000);   //chuyển thang đo của value 
                                        //từ 0-1023 sang 0-5000 (mV)
  Serial.println(voltage);              //xuất ra điện áp (đơn vị là mV)
  
  Serial.println();     //xuống hàng
  delay(200);
  analogWrite(3,255);
}