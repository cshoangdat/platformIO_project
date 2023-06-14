#include <Arduino.h>
#define button 0
int numPress = 0;
bool isPress = false;
void IRAM_ATTR isr(){
  numPress += 1;
  isPress = true;
}
void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(button,isr,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(isPress){
    Serial.printf("Nut nhan da duoc an %u lan\n",numPress);
    isPress = false;
  }
}