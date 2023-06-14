#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

//Semaphore couting Example

// int nTasks = 4;
// SemaphoreHandle_t barrierSemaphore = xSemaphoreCreateCounting(nTasks,0);

// void genericTask(void *param){
//   String taskMessage = "task number:";
//   taskMessage += (int)param;
//   Serial.println(taskMessage);
//   xSemaphoreGive(barrierSemaphore);
//   vTaskDelete(NULL);
// }

// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("Starting to launch tasks...");
//   int i;
//   for(i = 0;i<nTasks;i++) {
//     xTaskCreatePinnedToCore(genericTask,"genericTask",10000,(void *)i,0,NULL,1);
//   }
//   for(i = 0;i<nTasks;i++) {
//     xSemaphoreTake(barrierSemaphore,portMAX_DELAY);
//   }
//   Serial.println("Tasks launched and semaphore passed...");
// }

// void loop() {
//   vTaskSuspend(NULL);
// }

SemaphoreHandle_t xMutex;

void lowPriorityTask (void*p);
void highPriorityTask (void*p);

void setup()
{
 Serial.begin(115200);
 xMutex = xSemaphoreCreateMutex(); 
 xTaskCreate(lowPriorityTask, "lowPriorityTask",1000,NULL,1,NULL);
 delay(500);
 xTaskCreate(highPriorityTask,"HighPriorityTask",1000,NULL,4,NULL);
 
}

void loop(){

}

void lowPriorityTask(void *p){
  Serial.println((char*)p);
  for(;;){
    Serial.println("lowPriorityTask gains key");
    xSemaphoreTake(xMutex,portMAX_DELAY);
    delay(2000);
    Serial.println("lowPriorityTask releases key");
    xSemaphoreGive(xMutex);
  }
  vTaskDelete(NULL);
}

void highPriorityTask(void *p){
  Serial.println((char*)p);
  for(;;){
    Serial.println("highPriorityTask gains key");
    xSemaphoreTake(xMutex,portMAX_DELAY);
    Serial.println("highPriorityTask is running");
    Serial.println("highPriorityTask releases key");
    xSemaphoreGive(xMutex);
    delay(1000);
  }
  vTaskDelete(NULL);
}
