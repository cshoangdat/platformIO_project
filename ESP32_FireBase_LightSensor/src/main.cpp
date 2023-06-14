#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

#define lightSensor 34
int t1 = 0;

#define ssid "HOANG DAT"
#define pass "24072002"

#define fireBase_Host "esp32-light-sensor-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define fireBase_Auth "Zb31IrylUI81HaVhoqPia8B9t2Zl74K1OTfABwfp"

FirebaseData fireBase_Data;

float get_light_sensor(){
  int light_value;
  light_value = analogRead(lightSensor);
  float percent_value = (float)((light_value * 100) / 4096);
  return percent_value;
}

//WifiClient
String path = "/";
FirebaseJson json;

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Firebase.begin(fireBase_Host, fireBase_Auth);
  Firebase.reconnectWiFi(true);
  if(!Firebase.beginStream(fireBase_Data,path)){
    Serial.print("Reason: " + fireBase_Data.errorReason());
    Serial.println();
  }
  Serial.print("WiFi Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop() {
  float lightPercent = get_light_sensor();
  if(millis() - t1 >500){
    Firebase.setFloat(fireBase_Data, path + "/LDR/Percentage", lightPercent);
    Serial.println(lightPercent);
    t1 = millis();
  }
}