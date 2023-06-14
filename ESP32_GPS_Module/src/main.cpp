#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <BlynkSimpleEsp32.h>
#include "SoftwareSerial.h"

// #define BLYNK_PRINT Serial
#define rxPin 5
#define txPin 18

boolean newData = false;
char ssid[] = "HOANG DAT";
char pass[] = "24072002";

SoftwareSerial neogps(rxPin, txPin);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
 neogps.begin(9600);
}

void loop() {
  while(neogps.available()){
    gps.encode(neogps.read());
    if(gps.location.isValid()){
    Serial.print("SAT: ");
    Serial.println(gps.satellites.value());
    Serial.print("LAT: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("LONG: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("SPEED: ");
    Serial.println(gps.speed.kmph());
    Serial.print("ALT: ");
    Serial.println(gps.altitude.value());
      }
    }
  }
