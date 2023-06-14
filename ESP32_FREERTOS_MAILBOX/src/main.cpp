#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

QueueHandle_t xMailBox;
TaskHandle_t xTaskHandle_1;
TaskHandle_t xTaskHandle_2;

void vUpdateMailBox(void *p);
void vReadMailBox(void *p);

void setup() {
  Serial.begin(115200);
  xMailBox = xQueueCreate(1, sizeof(int32_t));
  xTaskCreate(vUpdateMailBox,"vUpdateMailBox",10000,NULL,1,NULL);
  xTaskCreate(vReadMailBox,"vReadMailBox",10000,NULL,1,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void vUpdateMailBox(void *p){
  int32_t ulNewValue = 1;
  while(1){
    xQueueOverwrite(xMailBox,&ulNewValue); //ghi de gia tri vao queue
    Serial.println("ghi gia tri vao mailbox");
    ulNewValue++;
    if(ulNewValue>100) ulNewValue=0;
    vTaskDelay(1000);
  }
}

void vReadMailBox(void *p){
  int32_t value_received;
  while(1){
    xQueuePeek(xMailBox, &value_received, portMAX_DELAY);
    Serial.print("Gia tri nhan duoc tu mail box: ");
    Serial.println(value_received);
    vTaskDelay(500);
  }
}