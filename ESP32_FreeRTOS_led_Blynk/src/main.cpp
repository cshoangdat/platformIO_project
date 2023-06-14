#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#define LED1 2 //gan chan LED1 chan 2
#define LED2 4 // gan chan LED2 chan 4
void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void setup() {
  Serial.begin(115200); //gan toc do baud = 115200
  //set up 2 task chay doc lap voi nhau
  xTaskCreatePinnedToCore(TaskBlink1,"TaskBlink1",1024,NULL,2,NULL,0);
  //tao task 1 voi (task can chay, ten task, stack size, priority, core su dung)
  xTaskCreatePinnedToCore(TaskBlink2,"TaskBlink2",1024,NULL,2,NULL,1);
  //tao task 2 voi (task can chay, ten task, stack size, priority, core su dung)
}
void loop()
{
}
void TaskBlink1(void *pvParameters)  //task1
{
  (void) pvParameters;
  pinMode(LED1, OUTPUT);// khai bao led1 output

  for (;;) //lap lai vo tan
  {
    Serial.print("Task1: ");
    Serial.println(xPortGetCoreID());
    digitalWrite(LED1, HIGH);  
    vTaskDelay(100);  // bat led sang trong 15ms
    digitalWrite(LED1, LOW);    
    vTaskDelay(100);  // led tat trong 15ms
  }
}

void TaskBlink2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
   pinMode(LED2, OUTPUT); //khai bao led2 output
  for (;;) //khai bao led2 output
  {
    Serial.print("Task2: ");
    Serial.println(xPortGetCoreID());
    digitalWrite(LED2, HIGH);   
    vTaskDelay(200);  //bat led sang trong 30ms
    digitalWrite(LED2, LOW);  
    vTaskDelay(200); //led tat trong 30ms
  }
}