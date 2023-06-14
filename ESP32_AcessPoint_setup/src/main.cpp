#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "DatHoang";
const char *password = "dat123456";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
}

void loop() {
  // put your main code here, to run repeatedly:
}