#include <Arduino.h>
#define touch T4
byte touch_value;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(921600);
}

void loop() {
  touch_value = touchRead(touch);
  Serial.println(touch_value);
  delay(500);
}