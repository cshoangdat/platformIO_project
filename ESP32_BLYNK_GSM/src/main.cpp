#include <Arduino.h>
#define BLYNK_PRINT Serial

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

char auth[] = "0DPlSeHM2ZeIKIN400Wbm3BJnx0caHuN";

char apn[] = "internet";
char user[] = "Telenor";
char pass[] = "Telenor";

#include "SoftwareSerial.h"
#define rxPin 5
#define txPin 18
SoftwareSerial mySerial(rxPin, txPin);
//#define mySerial Serial2

TinyGsm modem(mySerial);

void setup() {
  Serial.begin(115200);
  delay(10);
  mySerial.begin(9600);
  delay(300);
  Serial.println("Intializing modem...");
  modem.restart();
  Blynk.begin(auth, modem, apn, user, pass, "blynk.en-26.com", 9600);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
}