#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>

#define ssid "Hihi"
#define pass "123456890"

#define fireBaseHost "esp32-mq2-7724e-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define fireBaseAuth "fqh4JhSX0DReSScBh2ZdXS4Z56Nb4YEUi1xvIS7j"

FirebaseData fireBaseData;
String path = "/";
FirebaseJson json;

#define MQ2_PIN 34
int t1 = 0;

void setup() {
  Serial.begin(115200); //set tốc độ baud để giao tiếp serial voi máy tính là 115200
  WiFi.begin(ssid, pass); //bắt đầu sử dụng wifi với (ssid, pass)
  pinMode(MQ2_PIN, INPUT); //chân MQ2_PIN gắn là INPUT
  if(WiFi.status() != WL_CONNECTED){ 
    delay(500);
    Serial.print(".");
  } //Kiem tra WIFi đã kết nối được chưa 
  Firebase.begin(fireBaseHost, fireBaseAuth); //bắt đầu firebase
  Firebase.reconnectWiFi(true);
  if(!Firebase.beginStream(fireBaseData,path)){
    Serial.print("Reason:" + fireBaseData.errorReason());
    Serial.println();
  } //Kiểm tra xem Firebase đã cập nhật được các data chưa
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP()); //hiện thị localIP
}

void loop() {
  int MQ2_Percent = map(analogRead(MQ2_PIN),0,4095,0,100); //Đọc giá trị từ cảm biến và chuyển sang phần trăm 
  if(millis() - t1 >500){
    Firebase.setFloat(fireBaseData, path + "/MQ2/Percentage", MQ2_Percent); //Cứ 500ms sẽ cập nhật giá trị MQ2 lên fireBase 1 lần
    Serial.println(MQ2_Percent);
    t1 = millis(); //cập nhật lại giá trị t1
  }
}