#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_TEMPLATE_ID "TMPLzLVFNJ1q"
#define BLYNK_DEVICE_NAME "TestLedBlink"
#define BLYNK_AUTH_TOKEN "vMCfGB1-xuH_WINssBtIcTxSs5I619_C"
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HOANG DAT";
char pass[] = "24072002";

void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop(){}