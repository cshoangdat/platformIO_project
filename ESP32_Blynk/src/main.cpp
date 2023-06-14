#define BLYNK_TEMPLATE_ID "TMPLjVtGqs49"
#define BLYNK_DEVICE_NAME "TestLedBlink"
#define BLYNK_AUTH_TOKEN "nrD9wFdMjv6jhpAcYawNX7n0k2IZSLei"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "HOANG DAT";
char pass[] = "24072002";

#define ledPin  2

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V0){
  int p = param.asInt();
  digitalWrite(ledPin, p);
}