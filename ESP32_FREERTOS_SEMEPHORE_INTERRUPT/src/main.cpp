#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

SemaphoreHandle_t interruptSemaphore;

void taskLedOn(void *p);
void taskLedOff(void *p);
void interruptHandler();

void setup() {
  Serial.begin(115200);
  xTaskCreate(taskLedOn,"taskLedOn",1024,NULL,0,NULL);
  xTaskCreate(taskLedOff,"taskLedOff",1024,NULL,0,NULL);
  interruptSemaphore = xSemaphoreCreateBinary();
  if(interruptSemaphore != NULL) attachInterrupt(2,interruptHandler,HIGH);
}

void interruptHandler(){
  Serial.println("Semaphore is given");
  BaseType_t xHigherPriorityTaskWoken pdFALSE;
  xSemaphoreGiveFromISR(interruptSemaphore, &xHigherPriorityTaskWoken);
}

void taskLedOn(void *p){
  (void) p;
  while(1){
    if(xSemaphoreTake(interruptSemaphore, portMAX_DELAY) == pdPASS){
      Serial.println("taskLedOn received Semaphore");
      delay(100);
    }
  }
}

void taskLedOff(void *p){
  (void) p;
  while(1){
    if(xSemaphoreTake(interruptSemaphore, portMAX_DELAY) == pdPASS){
      Serial.println("taskLedOn received Semaphore");
      delay(100);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}