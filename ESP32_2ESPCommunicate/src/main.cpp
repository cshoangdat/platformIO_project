#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);
}

void loop() {
  while(Serial2.available()){
    Serial.write(char(Serial2.read()));
  }
  Serial2.println("ChaoESP32");
  delay(1000);
  // put your main code here, to run repeatedly:
}