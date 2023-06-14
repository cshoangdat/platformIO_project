#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#define ssid "Hihi"
#define pass "123456890"

#define CHANNEL_ID 1934168
#define CHANNEL_API_KEY "1UVANCK8E5MBYTBQ"
WiFiClient client;

#define MQ2_PIN 34 //Gán chân A0 của MQ2 với chân 34 của ESP
int t1 = 0;

void setup() {
  Serial.begin(115200); //Gán tốc độ baud để gia tiếp serial với máy tính là 115200 
  WiFi.begin(ssid, pass); //bắn đầu WiFi với ssid và pass
  pinMode(MQ2_PIN, INPUT); //Gán chân MQ2_PIN là INPUT
  if(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  } //Kiểm tra WiFi đã được kết nối chưa
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP()); //hiện thị localIP
  ThingSpeak.begin(client); //bắt đầu thingSpeak
}

void loop() {
  int MQ2_Percent = map(analogRead(MQ2_PIN),0,4095,0,100); //đọc giá trị của cảm biến và chuyển sang phần trăm
  if(millis() - t1 >500){
    ThingSpeak.setField(1, MQ2_Percent); //Chọn Field để xuất ra giá trị MQ2_Percent
    ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY); //Ghi giá trị vào field với CHANNEL_ID và CHANNEL_API_KEY lấy ở trên
    Serial.println(MQ2_Percent);
    t1 = millis(); // gán lại giá trị t1
  }
}