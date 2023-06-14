#include <Arduino.h>

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
// #include <TinyGPS++.h>
#include <TinyGsmClient.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "SoftwareSerial.h"
#include <ArduinoHttpClient.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);

#define SCL 22
#define SDA 21

// #define rxPin 5
// #define txPin 18

#define RXD2 16
#define TXD2 17

#define buttonSMS 15
#define buttonCall 19

const char FIREBASE_HOST[] = "test-107cd-default-rtdb.firebaseio.com";
const String FIREBASE_AUTH = "7JlDcUDlAiX8iGktmbLcwPD0gbrrJekgGk5uPLAY";
const String FIREBASE_PATH = "data";
const int SSL_PORT = 443;

char apn[] = "m-wap";
char user[] = "mms";
char pass[] = "mms";

const String PHONE_NUMBER = "+84903847805";
const String MY_PHONE_NUMBER = "+84363604119";

// SoftwareSerial neogps(rxPin, txPin);
// TinyGPSPlus gps;

HardwareSerial sim800(2);
TinyGsm modem(sim800);
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // ESP32 Thing, HW I2C with pin remapping

QueueHandle_t xQueueLat, xQueueLong, xQueueSpeed, xQueueSat, xQueueBattery;

portMUX_TYPE timerMutex = portMUX_INITIALIZER_UNLOCKED;

SemaphoreHandle_t xMutex;
SemaphoreHandle_t xBinarySemaphore;

xTaskHandle xHandle_1;
xTaskHandle xHandle_2;

void taskGPS(void *param);
void taskSMS(void*param);
void taskCall(void *param);
void taskFirebase(void *param);
void DelayMS(uint32_t delay_ms);

float buff_lattitude,buff_longitude,buff_speed;
int buff_satellites,buff_network,buff_battery;

const unsigned char logo[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
0x12, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x01, 0x40, 0x17, 0x00, 0xA0, 0x03, 0x80, 0x32,
0x00, 0x70, 0x07, 0x00, 0x12, 0x00, 0x38, 0x07, 0x00, 0x00, 0x00, 0x18, 0x0E, 0x00, 0x00, 0x00,
0x1C, 0x0C, 0x00, 0x0C, 0x00, 0x0C, 0x1C, 0x00, 0x0C, 0x00, 0x0E, 0x18, 0x00, 0x2D, 0x00, 0x06,
0x18, 0x00, 0x0C, 0x00, 0x06, 0x38, 0x40, 0x4C, 0x80, 0x87, 0x39, 0x00, 0x4C, 0x00, 0x23, 0x30,
0xC0, 0x2D, 0x00, 0xC3, 0x33, 0x34, 0x3F, 0x01, 0x13, 0x30, 0xC2, 0x0C, 0x00, 0x63, 0x32, 0x32,
0x08, 0x13, 0x93, 0x39, 0x89, 0x00, 0x06, 0x63, 0x38, 0x60, 0x00, 0x21, 0x87, 0x19, 0xFE, 0x00,
0x1F, 0xF7, 0x18, 0xFF, 0x80, 0x7F, 0xC6, 0x1C, 0x1F, 0xC0, 0xFE, 0x0E, 0x0C, 0x03, 0xC0, 0xF0,
0x0E, 0x0E, 0x0F, 0xC0, 0xFC, 0x1C, 0x06, 0x0F, 0xC0, 0x7E, 0x1C, 0x07, 0x0F, 0xC0, 0x7E, 0x38,
0x03, 0x9F, 0x80, 0x7E, 0x70, 0x01, 0xDF, 0x80, 0x7E, 0xF0, 0x00, 0xE7, 0x80, 0x7D, 0xE0, 0x00,
0x73, 0x80, 0x73, 0xC0, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x00, 0x1F, 0xC0, 0xFE, 0x00, 0x00, 0x07,
0xFF, 0xF8, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x66, 0x71, 0x9D, 0x9B, 0xEF, 0x64, 0xC9, 0x9D, 0x99, 0x8C, 0x64, 0xC9, 0xDD, 0x99, 0x8C,
0x7C, 0xC1, 0x4D, 0x99, 0x8F, 0x64, 0xC1, 0x6D, 0x99, 0x8C, 0x64, 0xC9, 0x6D, 0x99, 0x8C, 0x64,
0xD9, 0x6C, 0xD1, 0x8F, 0x64, 0x31, 0x0C, 0x60, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x68, 0x61, 0x72, 0x20, 0x48, 0x43, 0x4D, 0x55, 0x54, 0x45, 0x40,
};

void setup() {
  pinMode(DHTPIN, INPUT);
  pinMode(buttonSMS, INPUT);
  pinMode(buttonCall, INPUT);
  dht.begin();
  Serial.begin(115200);
  delay(10);
  sim800.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(10);
  // neogps.begin(9600);
  // delay(3000);
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
  http_client.setHttpResponseTimeout(10*1000);

  u8g2.setBusClock(1000000);
  u8g2.begin();
  u8g2.setFont(u8g2_font_8x13B_mr);
  u8g2.enableUTF8Print();
  u8g2.setFontMode(1);
  u8g2.clearBuffer();
  u8g2.drawFrame(5,5,118,58);
  u8g2.drawBitmap(12, 6, 5.25, 54, logo);
  u8g2.drawStr(75,30,"GPS");
  u8g2.drawStr(60,45,"TRACKER");
  u8g2.sendBuffer();

  xQueueLat = xQueueCreate(1,sizeof(float));
  xQueueLong = xQueueCreate(1,sizeof(float));
  xQueueSpeed = xQueueCreate(1,sizeof(float));
  xQueueSat = xQueueCreate(1,sizeof(int));
  xQueueBattery = xQueueCreate(1,sizeof(int));

  vSemaphoreCreateBinary(xBinarySemaphore);
  xMutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(taskFirebase,"taskFirebase",10000,NULL,3,NULL,1);
  xTaskCreatePinnedToCore(taskGPS,"taskGPS",100000,NULL,3,NULL,1);
}

void delayMS(uint32_t delay_ms){
  uint32_t current_tick_count = xTaskGetTickCount();
  uint32_t delay_in_ticks = (delay_ms*current_tick_count)/1000;
  while(xTaskGetTickCount() < (current_tick_count + delay_in_ticks));
}

void postToFirebase(const char *method, const String &path, const String &data, HttpClient *http){
  String response;
  int statusCode = 0;
  http->connectionKeepAlive();
  String url;
  if(path[0] != '/'){
    url = "/";
  }
  url += path +".json";
  url += "?auth=" + FIREBASE_AUTH;
  Serial.print("POST: ");
  Serial.println(url);
  Serial.print("Data: ");
  Serial.println(data);
  String ContentType = "application/json";
  http->put(url, ContentType, data);
  statusCode = http->responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  response = http->responseBody();
  Serial.print("Response: ");
  Serial.println(response);
  if(!http->connected()){
    Serial.println();
    http->stop();
    Serial.println("HTTP POST disconnected");
  }
}

void dht_loop()
{
  String h = String(dht.readHumidity(), 2);
  String t = String(dht.readTemperature(), 2);
  delay(100);
 
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println(" %");
 
  String Data = "{";
  Data += "\"temp\":" + t + ",";
 
  Data += "\"humid\":" + h + "";
  Data += "}";
 
  postToFirebase("PATCH", FIREBASE_PATH, Data, &http_client);
}

void taskFirebase(void *param){
  for(;;){
    Serial.println("bat dau task 1");
    Serial.print(F("Connecting to "));
    Serial.print(apn);
    if (!modem.gprsConnect(apn, user, pass))
    {
    Serial.println(" fail");
    delay(1000);
    return;
    }
    Serial.println(" OK");
    http_client.connect(FIREBASE_HOST, SSL_PORT);
  }
}

void taskGPS(void *param){
    float lattitude;
    float longitude;
    float speed;
    int satellites;
    vTaskDelay(5000/portTICK_PERIOD_MS);
    for(;;){
    Serial.println("Task GPS");
    if (!http_client.connected())
    {
      Serial.println();
      http_client.stop();// Shutdown
      Serial.println("HTTP  not connect");
      break;
    }
    else
    {
      String h = String(dht.readHumidity(), 2);
      String t = String(dht.readTemperature(), 2);
      delay(100);
    
      Serial.print("Temperature = ");
      Serial.print(t);
      Serial.println(" °C");
      Serial.print("Humidity = ");
      Serial.print(h);
      Serial.println(" %");
    
      String Data = "{";
      Data += "\"temp\":" + t + ",";
    
      Data += "\"humid\":" + h + "";
      Data += "}";
    
      postToFirebase("PATCH", FIREBASE_PATH, Data, &http_client);
    }
    int buttonSMS_State = digitalRead(buttonSMS);
    int buttonCall_State = digitalRead(buttonCall);
    if (buttonSMS_State == 1){
      xTaskCreatePinnedToCore(taskSMS,"taskSMS",10000,NULL,4,&xHandle_1,1);
    }
    if(buttonCall_State == 1){
      xTaskCreatePinnedToCore(taskCall,"taskCall",4096,NULL,4,&xHandle_2,1);
    }
    delayMS(500);
  }
}

void taskSMS(void*param){
  Serial.println("dang chay task SMS");
  float buff_lat;
  float buff_long;
  xQueueReceive(xQueueLat,&buff_lat,portMAX_DELAY);
  xQueueReceive(xQueueLong,&buff_long,portMAX_DELAY);
  sim800.println("AT+CMGF=1");
  delay(500);
  sim800.println("AT+CMGS=\""+MY_PHONE_NUMBER+"\"");
  delay(100);
  sim800.println("Emergency Situation SMS!!! I'm at this location:");
  sim800.print("Latitude: ");
  sim800.println(buff_lat,6);
  sim800.print("longitude: ");
  sim800.println(buff_long,6);
  delay(10);
  sim800.write(26);
  Serial.println("SMS Sent Successfully");
  delayMS(500);
  vTaskDelete(NULL);
}

void taskCall(void *param){
  Serial.println("dang chay task Call");
  float buff_lat;
  float buff_long;
  xQueueReceive(xQueueLat,&buff_lat,portMAX_DELAY);
  xQueueReceive(xQueueLong,&buff_long,portMAX_DELAY);
  sim800.println("ATD+ "+MY_PHONE_NUMBER+";");
  delay(10000);
  sim800.println("ATH");
  delay(1000);
  sim800.println("AT+CMGF=1");
  delay(500);
  sim800.println("AT+CMGS=\""+MY_PHONE_NUMBER+"\"");
  delay(100);
  sim800.print("http://maps.google.com/maps?q=loc:");
  sim800.print(buff_lat, 6);
  sim800.print(",");
  sim800.print(buff_long, 6);
  delay(10);
  sim800.write(26);
  Serial.println("SMS Sent Successfully");
  delayMS(500);
  vTaskDelete(NULL);
}

void loop() {
  vTaskDelete(NULL);
}  
