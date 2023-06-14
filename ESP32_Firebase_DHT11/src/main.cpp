#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define ssid "IoT Lab"
#define pass "IoT@123456"

#define fireBaseHost "dht11-7cae8-default-rtdb.firebaseio.com/"
#define fireBaseAuth "7dRfnYJd39esRdGAy8JrPJ34HXfAvgix5AW0Xc4G"

FirebaseData fireBaseData;
String path = "/";
FirebaseJson json;

#define DHTPIN 4 //gán chân data của DHT là chân 4
#define DHTTYPE DHT11 //loại DHT là DHT11

DHT dht(DHTPIN, DHTTYPE);
int t1 = 0;

void setup() {
  Serial.begin(115200); //set tốc độ baud để giao tiếp serial voi máy tính là 115200
  WiFi.begin(ssid, pass); //bắt đầu sử dụng wifi với (ssid, pass)
  pinMode(DHTPIN, INPUT); //chân MQ2_PIN gắn là INPUT
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
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
   //Đọc giá trị từ cảm biến và chuyển sang phần trăm 
  if(millis() - t1 >500){
    Firebase.setFloat(fireBaseData, path + "/data/temp", temp);
    Serial.print("Nhiet do: "); //Cứ 500ms sẽ cập nhật giá trị MQ2 lên fireBase 1 lần
    Serial.println(temp);
    Firebase.setFloat(fireBaseData, path + "/data/humid", humid); 
    Serial.print("Do Am: "); //Cứ 500ms sẽ cập nhật giá trị MQ2 lên fireBase 1 lần
    //Cứ 500ms sẽ cập nhật giá trị MQ2 lên fireBase 1 lần
    Serial.println(humid);
    t1 = millis(); //cập nhật lại giá trị t1
  }
}