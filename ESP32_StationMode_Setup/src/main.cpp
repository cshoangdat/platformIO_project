#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "HOANG DAT";
const char* password = "24072002";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print(" ");
  Serial.print("Connecting to WIFI...");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
}