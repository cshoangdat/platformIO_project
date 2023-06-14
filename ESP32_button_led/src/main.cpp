#include <Arduino.h> //Khai bao thu vien arduino.h

#define btn 4 // gan btn la chan 4 cua esp
#define led 2 // gan led la chan 2 cua esp
int p = 0; //cho bien p = 0
int count = 0;

void setup() {
  Serial.begin(115200); //set toc do baud la 115200
  pinMode(btn, INPUT); //khai bao btn INPUT
  pinMode(led, OUTPUT); //khai bao led OUTPUT
  digitalWrite(led, LOW); //ban dau cho led tat
}

void loop() {
  int buttonState = digitalRead(btn); //gan trang bien de doc trang thai nut bam
    if(buttonState == 1){ // neu trang thai nut nhan la 1 
      delay(300);
      if(buttonState == 1){
        count++;
        delay(100);
        // while(digitalRead(btn)==1)
          ;
      }
    }
        if(count == 1){
          digitalWrite(led, HIGH);
          for (int i = 0; i < 100;i++){
            delay(10);
            if(digitalRead(btn)==1)
              break;
          }
          digitalWrite(led, LOW);
           for (int i = 0; i < 100;i++){
            delay(10);
            if(digitalRead(btn)==1)
              break;
          }
        }
         if(count >1){
          digitalWrite(led, HIGH);
          delay(200);
          digitalWrite(led, LOW);
          delay(200);
            ;
        }
  }