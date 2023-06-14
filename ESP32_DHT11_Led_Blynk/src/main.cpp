#define BLYNK_TEMPLATE_ID "TMPLzIyFYdKy" //ID template đã tạo
#define BLYNK_DEVICE_NAME "DHT11 and Led Blynk" //tên dự án đã tạo
#define BLYNK_AUTH_TOKEN "7Kqg27Pa8JaKTz5EOAha8enC-I5TRab3" //Auth token
#define BLYNK_PRINT Serial //hiện thị kết nối với Blynk ra Serial

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "Hihi"; //tên wifi kết nối 
char pass[] = "123456890"; //pass wifi

#define ledPin 2 //gán chân led chân 2
#define DHTPIN 4 //gán chân data của DHT là chân 4
#define DHTTYPE DHT11 //loại DHT là DHT11

DHT dht(DHTPIN, DHTTYPE);  //cấu hình cho dht
BlynkTimer timer; //gửi dữ liệu theo từng khoảng thời gian

float temp =0;        
float humid = 0;       

void sendSensor(){
  temp = dht.readTemperature(); //đọc giá trị nhiệt độ từ dht
  humid = dht.readHumidity(); // đọc giá trị độ ẩm từ dht
  Blynk.virtualWrite(V0, temp); //gửi dữ liệu nhiệt độ đến V0
  Blynk.virtualWrite(V1, humid); // gửi dữ liệu độ ẩm đến V1
}

void setup() {
  Serial.begin(115200); 
  Blynk.begin(auth, ssid, pass); //khai báo để bắt đầu blynk với (auth,ssid,pass)
  dht.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(DHTPIN,INPUT);
  digitalWrite(ledPin, 0); //ban đầu cho led tắt
  timer.setInterval(500L, sendSensor); // cứ 500ms gửi giá trị nhiệt độ và độ ẩm lên Blynk 1 lần
}

void loop() {
  Blynk.run(); //khởi chạy Blynk
  timer.run(); //khởi chạy bộ hẹn giờ
  Serial.println(temp); // xuất giá trị nhiệt độ ra Serial
  delay(500);
  Serial.println(humid); // xuất giá trị độ ẩm ra Serial 
  delay(500);
}

BLYNK_WRITE(V2){ //nhận giá trị cập nhật từ button
  int p = param.asInt(); //nhận giá trị dưới dạng số nguyên 
  digitalWrite(ledPin, p); //bật tắt led theo p
}