#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void task1(void *p);
void task2(void *p);

TaskHandle_t TaskHandle_1 = NULL;
TaskHandle_t TaskHandle_2 = NULL;
QueueHandle_t Queue;

void task1(void *p){
  char myTxBuff[30];
  Queue = xQueueCreate(7,sizeof(myTxBuff));

  sprintf(myTxBuff,"Hello");
  xQueueSend(Queue,(void*)myTxBuff,(TickType_t) 0);

  sprintf(myTxBuff,"Dat");
  xQueueSend(Queue,(void*)myTxBuff,(TickType_t) 0);

  sprintf(myTxBuff,"Hoang");
  xQueueSend(Queue,(void*)myTxBuff,(TickType_t) 0);

  sprintf(myTxBuff,"ne");
  xQueueSend(Queue,(void*)myTxBuff,(TickType_t) 0);

  Serial.print("data waiting to be read");
  Serial.println(uxQueueMessagesWaiting(Queue));
  Serial.print("available space: ");
  Serial.println(uxQueueSpacesAvailable(Queue));
  while(1){}
}

void task2(void *p){
  char myRxBuff[30];

  while(true){
    if(Queue != 0){
      if(xQueueReceive(Queue, (void *)myRxBuff, (TickType_t) 5)){
        Serial.print("dara recieved: ");
        Serial.println(myRxBuff);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(task1,"task1",1024,NULL,1,&TaskHandle_1,1);
  xTaskCreatePinnedToCore(task2,"task2",1024,NULL,1,&TaskHandle_2,1);
}

void loop() {
  // put your main code here, to run repeatedly:
}