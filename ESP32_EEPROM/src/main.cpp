#include <Arduino.h>
#include <EEPROM.h>

#define FLASH_MEMORY_SIZE 1 // 1 O NHO

const int ledPin = 16;
const int buttonPin = 4;

int ledState = HIGH;

const int debounceDelay = 20;

void setup() {
  EEPROM.begin(FLASH_MEMORY_SIZE);
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  ledState = EEPROM.read(0);
  digitalWrite(ledPin, ledState);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    delay(debounceDelay);
    EEPROM.write(0, ledState);
    EEPROM.commit(); // xac thuc su thay doi
    while(buttonState == HIGH);
  }
}