#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define led 2

// void taskBlink(void *pvParameters);
// void taskAnalogReadA3(void *pvParameters);

void taskPrint1(void *pvParameters);
void taskPrint2(void *pvParameters);

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;

int counter = 0;

void setup() {
  Serial.begin(115200);
  // xTaskCreatePinnedToCore(taskBlink,"taskBlink",1024,NULL,2,NULL,0);
  // xTaskCreatePinnedToCore(taskAnalogReadA3,"taskAnalogReadA3",1024,NULL,1,NULL,1);
  xTaskCreate(taskPrint1,"taskPrint1",1024,NULL,3, &TaskHandle_1);
  xTaskCreate(taskPrint2,"taskPrint2",1024,NULL,3, &TaskHandle_2);
}

// void taskBlink(void *pvParameters) {
//   (void) pvParameters;
//   pinMode(led, OUTPUT);
//   for(;;){
//     Serial.print("task: ");
//     Serial.println(xPortGetCoreID());
//     digitalWrite(led, HIGH);
//     vTaskDelay(250); //1 tisk la 15ms
//     digitalWrite(led,LOW);
//     vTaskDelay(250);
//   }
//   vTaskDelete(NULL);
// }

// void taskAnalogReadA3(void *pvParameters){
//   (void)pvParameters;
//   for(;;){
//     int sensorValueA3 = analogRead(A3);
//     Serial.println(sensorValueA3);
//     Serial.print("Task 2: ");
//     Serial.println(xPortGetCoreID());
//     vTaskDelay(500);
//   }
//   vTaskDelete(NULL);
// }

void loop() {
  // put your main code here, to run repeatedly:
}

void taskPrint1(void *pvParameters){
  (void) pvParameters;
  // TickType_t getTick;
  // getTick = xTaskGetTickCount();
  while(1){
    Serial.println("TASK 1");
    // counter++;
    // if(counter == 15){
    //   vTaskResume(TaskHandle_2);
    // }
    // vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);
    vTaskDelay(1000);
  }
}

void taskPrint2(void *pvParameters){
  (void) pvParameters;
  while(1){
    Serial.println("TASK 2");
    // counter++;
    // if(counter == 10){
    //   vTaskSuspend(TaskHandle_2);
    // }
    // vTaskDelay(1000/portTICK_PERIOD_MS);
    vTaskDelay(1000);
  }
}