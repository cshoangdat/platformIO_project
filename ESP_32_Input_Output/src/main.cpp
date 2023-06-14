#include <Arduino.h>
#define LED 21
#define BUTTON_PIN 19
bool led_state;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(921600);
}

void loop() {
  byte button_value = digitalRead(BUTTON_PIN);
  if(button_value == 1){
    led_state = !led_state;
    digitalWrite(LED, led_state);
    delay(500);
      ;
  }
}