#include <Arduino.h>
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256
 
#include <TinyGsmClient.h> //https://github.com/vshymanskyy/TinyGSM
#include <ArduinoHttpClient.h> //https://github.com/arduino-libraries/ArduinoHttpClient
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include "SoftwareSerial.h"
#include <U8g2lib.h>
#include <Wire.h>
// #include <FirebaseESP32.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/ 21);   // ESP32 Thing, HW I2C with pin remapping
 
#define RXPin 5
#define TXPin 18
#define button 19
int numPress = 0;

SoftwareSerial sim800(RXPin,TXPin);
TinyGsm modem(sim800);
 
#define DHTTYPE DHT11
#define DHTPIN 4
 
DHT dht(DHTPIN, DHTTYPE);
const char FIREBASE_HOST[] = "do-an-1-5cd7a-default-rtdb.firebaseio.com";
const String FIREBASE_AUTH = "rBTR8uO60dwnJa5334nOUHN1Kts402zO0upjCiZk";
const String FIREBASE_PATH = "data";
const int SSL_PORT = 443;
 
char apn[] = "m-wap";
char user[] = "mms";
char pass[] = "mms";

TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);

// FirebaseData fireBaseData;
String MY_PHONE_NUMBER;
String REL_PHONE_NUMBER;
String new_response;
 
unsigned long previousMillis = 0;

int is_login = 0;
int is_menu = 0;
int is_tracking = 0;
int is_fencing = 0;
int is_fencing_install = 0;
int is_fencing_running = 0;

float init_lat = 0;
float init_long = 0;
int max_distance = 0;
float distance;

u8g2_uint_t xx  = 0;
int x_max    = 128;       //OLED display width, in pixels
int y_max    = 62;        //OLED display width, in pixels
int x_center = x_max/2;
int y_center = y_max/2+10;
int arc      = y_max/2;
int angle    = 0;
int needle_pos = 0;

  float a = 10.84386;
  float b = 106.63597;
  int c = 100;
  int d = 2;
  int e = 3;

#define sat_logo_width 20
#define sat_logo_height 19

#define logo_width 64
#define logo_height 64

const unsigned char sat_logo[] = {
0x00, 0x08, 0x00, 0x24, 0x00, 0x42, 0x00, 0x81, 0x00, 0x42, 0x07, 0xA4, 0x07, 0xC8, 0x0F, 0xD0,
0x13, 0xE0, 0x21, 0xF6, 0x50, 0xFC, 0xA1, 0x38, 0x12, 0x35, 0x04, 0x21, 0x08, 0x02, 0x00, 0x05,
0x00, 0x06, 0x00, 0x01, 0x00, 0x04, 0x00, 0x5C, 0x6C, 0x6F, 0x67, 0x6F, 0x2E, 0x74, 0x78, 
};

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

const unsigned char satellite_BMP_64x64 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x01, 0xDC, 0x00, 0x00, 0x00,
0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x87, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE3, 0x80,
0x00, 0x00, 0x00, 0x18, 0x63, 0x80, 0x00, 0x00, 0x00, 0x38, 0x37, 0x00, 0x00, 0x00, 0x00, 0x7E,
0x3E, 0x00, 0x00, 0x00, 0x00, 0xC7, 0x1C, 0x00, 0x00, 0x00, 0x20, 0xC3, 0x38, 0x00, 0x00, 0x02,
0xF0, 0xE1, 0xF0, 0x00, 0x00, 0x07, 0xD8, 0x70, 0xE0, 0x00, 0x00, 0x03, 0x9C, 0x39, 0xC0, 0x00,
0x00, 0x07, 0x8E, 0x7F, 0x80, 0x00, 0x00, 0x06, 0x03, 0xCF, 0x00, 0x00, 0x00, 0x0C, 0x03, 0x86,
0x00, 0x00, 0x00, 0x06, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x60, 0x00, 0x00, 0x00, 0x01,
0x80, 0x70, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0C, 0x00, 0x00,
0x00, 0x1C, 0x70, 0x0C, 0x00, 0x00, 0x00, 0x3E, 0xF8, 0x38, 0x00, 0x00, 0x00, 0x67, 0x8C, 0x70,
0x00, 0x00, 0x00, 0xE3, 0x86, 0x78, 0x00, 0x00, 0x01, 0xE1, 0xC3, 0xDF, 0x00, 0x00, 0x03, 0x30,
0xE1, 0x89, 0x08, 0xC6, 0x07, 0x38, 0xE0, 0x09, 0x1C, 0xC6, 0x0F, 0x1D, 0xC0, 0x0F, 0x18, 0xC6,
0x1B, 0x07, 0x80, 0x00, 0x18, 0x86, 0x39, 0x87, 0x00, 0x00, 0x30, 0x8E, 0x71, 0xEE, 0x00, 0x00,
0x71, 0x8C, 0x38, 0x7C, 0x00, 0x01, 0xE1, 0x8C, 0x18, 0x38, 0x00, 0x07, 0x83, 0x18, 0x0E, 0x70,
0x00, 0x02, 0x07, 0x18, 0x07, 0xE0, 0x00, 0x00, 0x0C, 0x30, 0x03, 0xC0, 0x00, 0x00, 0x3C, 0x30,
0x01, 0x80, 0x00, 0x07, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x07, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00,
0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char satellite_BMP_24x24 [] = {
0x00, 0x00, 0x40, 0x00, 0x00, 0xE0, 0x00, 0x01, 0xF0, 0x00, 0x03, 0xF8, 0x00, 0x06, 0xF8, 0x00,
0x0F, 0x70, 0x01, 0xCD, 0xE0, 0x03, 0xEF, 0xC0, 0x03, 0xBF, 0x80, 0x03, 0x1B, 0x00, 0x01, 0x8C,
0x00, 0x00, 0xC6, 0x00, 0x07, 0xE6, 0x00, 0x0F, 0xBE, 0x00, 0x1D, 0xDF, 0x2B, 0x37, 0xC3, 0x6B,
0x7B, 0x80, 0x6B, 0xFF, 0x01, 0xDA, 0x7E, 0x03, 0xB6, 0x3C, 0x00, 0x64, 0x18, 0x03, 0xCC, 0x00,
0x00, 0x38, 0x00, 0x03, 0xE0, 0x00, 0x03, 0x80, 
};

const unsigned char location_BMP_32x32 [] = {
0x00, 0x01, 0x80, 0x00, 0x00, 0x1E, 0x78, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0xC0, 0x03, 0x00,
0x01, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x80, 0x03, 0x03, 0xC0, 0xC0, 0x02, 0x0C, 0x30, 0x40,
0x06, 0x18, 0x18, 0x60, 0x04, 0x10, 0x08, 0x20, 0x04, 0x10, 0x08, 0x20, 0x04, 0x10, 0x08, 0x20,
0x04, 0x10, 0x08, 0x20, 0x04, 0x18, 0x18, 0x20, 0x02, 0x0C, 0x30, 0x40, 0x02, 0x07, 0xE0, 0x40,
0x02, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x00, 0x80, 0x01, 0x00,
0x00, 0xC0, 0x03, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x20, 0x04, 0x00,
0x00, 0x10, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x0C, 0x30, 0x00, 0x00, 0x04, 0x20, 0x00,
0x00, 0x06, 0x60, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char altitude_BMP_24x24 [] = {
0x00, 0xFF, 0x00, 0x03, 0x00, 0xC0, 0x04, 0x7E, 0x20, 0x19, 0x81, 0x98, 0x16, 0x00, 0x68, 0x2C,
0x3C, 0x34, 0x48, 0x00, 0x12, 0x50, 0x18, 0x0A, 0x90, 0x3C, 0x08, 0xA0, 0x00, 0x05, 0xB0, 0x18,
0x0D, 0xA8, 0x24, 0x15, 0xA8, 0xC3, 0x15, 0xB0, 0x7E, 0x0D, 0xA0, 0x00, 0x05, 0x90, 0x3C, 0x09,
0x50, 0x18, 0x0A, 0x48, 0x00, 0x12, 0x2C, 0x3C, 0x34, 0x16, 0x00, 0x68, 0x19, 0x81, 0x98, 0x04,
0x7E, 0x20, 0x03, 0x00, 0xC0, 0x00, 0xFF, 0x00, 
};

const unsigned char distance_BMP_24x24 [] = {
0x00, 0x00, 0x3C, 0x00, 0x00, 0x42, 0x00, 0x00, 0x81, 0x00, 0x00, 0x99, 0x00, 0x00, 0x99, 0x00,
0x00, 0x81, 0x00, 0x00, 0x42, 0x00, 0x00, 0x66, 0x1F, 0x80, 0x24, 0x30, 0xC0, 0x18, 0x40, 0x20,
0x18, 0x4F, 0x30, 0x18, 0x99, 0x90, 0x00, 0x90, 0x90, 0x10, 0x90, 0x91, 0x60, 0x99, 0x93, 0x00,
0xCF, 0x32, 0x00, 0x40, 0x20, 0x00, 0x60, 0x64, 0x00, 0x20, 0x4C, 0x00, 0x10, 0x88, 0x00, 0x19,
0x80, 0x00, 0x0F, 0x60, 0x00, 0x06, 0x40, 0x00, 
};

void tracker(){
  //tracker
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.clearBuffer();
  u8g2.drawBitmap(5, 1,3,24,satellite_BMP_24x24);
  u8g2.setCursor(30,18);
  u8g2.print("Sat:" + (String)d);
  u8g2.drawBitmap(1,33,4,32,location_BMP_32x32);
  u8g2.setCursor(30,45);
  u8g2.print("Lat:" + String(a,6));
  u8g2.setCursor(30,60);
  u8g2.print("Lng:" + String (b,6));
  u8g2.drawBitmap(75,1,3,24,altitude_BMP_24x24);
  u8g2.setCursor(103,13);
  u8g2.print("Alt:");
  u8g2.setCursor(103,23);
  u8g2.print((String)e);
  u8g2.sendBuffer();
}

void gauge(uint8_t angle){
  u8g2.setFont(u8g_font_chikita);
  u8g2.clearBuffer();
  u8g2.drawCircle(x_center,y_center+5,arc+6, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(x_center,y_center+5,arc+4, U8G2_DRAW_UPPER_RIGHT);
  u8g2.drawCircle(x_center,y_center+5,arc+6, U8G2_DRAW_UPPER_LEFT);
  u8g2.drawCircle(x_center,y_center+5,arc+4, U8G2_DRAW_UPPER_LEFT);
  u8g2.drawStr(20,  42, "0");   
  u8g2.drawStr(18,  29, "25");
  u8g2.drawStr(28,  14, "50");
  u8g2.drawStr(58,  5, "100");
  u8g2.drawStr(91,  14, "150");
  u8g2.drawStr(101, 29, "175");
  u8g2.drawStr(105, 42, "200");

  u8g2.setCursor(54,30);
  u8g2.print("km/h");
  u8g2.setCursor(50,37);
  u8g2.print("SPEED");

  u8g2.drawBitmap(1, 0, 2.5, sat_logo_height,sat_logo);
  u8g2.setCursor(18, 5);
  u8g2.print("12");

  float x1=sin(2*angle*2*3.14/360);           
  float y1=cos(2*angle*2*3.14/360); 
  u8g2.drawLine(x_center, y_center, x_center+arc*x1, y_center-arc*y1);
  u8g2.drawDisc(x_center, y_center, 5, U8G2_DRAW_UPPER_LEFT);
  u8g2.drawDisc(x_center, y_center, 5, U8G2_DRAW_UPPER_RIGHT);

  u8g2.setFont(u8g_font_profont22);
  u8g2.setCursor(54,60);
  if (c<10){                              
    u8g2.print("0");
  }
  if (c>99) {                                  
    u8g2.setCursor(47,60);
  }
  u8g2.print(c);

  u8g2.sendBuffer();
}

void PostToFirebase(const char* method, const String & path , const String & data, HttpClient* http)
{
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

void getFormFirebase_info(const String & path, HttpClient* http){
  String response;
  int statusCode = 0;
  http->connectionKeepAlive(); // Currently, this is needed for HTTPS
 
  String url;
  if (path[0] != '/')
  {
    url = "/";
  }
  url += path + ".json";
  url += "?auth=" + FIREBASE_AUTH;
  // url += target;
  Serial.print("POST:");
  Serial.println(url);
 
  int data_get = http->get(url);

  statusCode = http->responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  response = http->responseBody();
  Serial.print("Response: ");
  Serial.println(response);
  response.remove(0,3);
  // unsigned int index1 = response.indexOf("\"");
  // unsigned int index2 = response.indexOf("\"",index1+1);
  new_response = response.substring(0,12);
  Serial.println(new_response);
 
  if (!http->connected())
  {
    Serial.println();
    http->stop();// Shutdown
    Serial.println("HTTP POST disconnected");
  }
}

void getFormFirebase_state(const String & path, HttpClient* http){
  String response;
  int statusCode = 0;
  http->connectionKeepAlive(); // Currently, this is needed for HTTPS
 
  String url;
  if (path[0] != '/')
  {
    url = "/";
  }
  url += path + ".json";
  url += "?auth=" + FIREBASE_AUTH;
  // url += target;
  Serial.print("POST:");
  Serial.println(url);
 
  int data_get = http->get(url);

  statusCode = http->responseStatusCode();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  response = http->responseBody();
  Serial.print("Response: ");
  Serial.println(response);
  response.remove(0,1);
  response.remove(1,1);
  // unsigned int index1 = response.indexOf("\"");
  // unsigned int index2 = response.indexOf("\"",index1+1);
  new_response = response;
  Serial.println(new_response);
 
  if (!http->connected())
  {
    Serial.println();
    http->stop();// Shutdown
    Serial.println("HTTP POST disconnected");
  }
}

void login_get(){
  getFormFirebase_info("User/Phone_num", &http_client);
  MY_PHONE_NUMBER = new_response;
  Serial.println("My phone number: "+ MY_PHONE_NUMBER);
  getFormFirebase_info("User/Rel_Phone_num", &http_client);
  REL_PHONE_NUMBER = new_response;
  Serial.println("Rel phone number: "+ REL_PHONE_NUMBER);
}
 
void dht_loop()
{
  // String h = String(dht.readHumidity(), 2);
  // String t = String(dht.readTemperature(), 2);
  String h = String(a,6);
  String t = String(b,6);
  String f = String(c);
  String g = String(d);
  String k = String(e);
  delay(100);
 
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println(" %");
  Serial.println(f);
  Serial.println(g);
  Serial.println(k);
 
    String Data = "{";
    Data += "\"lat\":" + h + ",";
    Data += "\"long\":" + t + ",";
    Data += "\"speed\":" + f + ",";
    Data += "\"sat\":" + g + ",";
    Data += "\"alt\":" + k + "";
    Data += "}";
 
  PostToFirebase("PATCH", FIREBASE_PATH, Data, &http_client);
}


bool isPress = false;
void IRAM_ATTR isr(){
  numPress += 1;
  isPress = true;
}

float getDistance(float flat1, float flon1, float flat2, float flon2) {
  // Variables
  float dist_calc=0;
  float dist_calc2=0;
  float diflat=0;
  float diflon=0;

  // Calculations
  diflat  = radians(flat2-flat1);
  flat1 = radians(flat1);
  flat2 = radians(flat2);
  diflon = radians((flon2)-(flon1));

  float a = (sin(diflat/2.0)*sin(diflat/2.0) + cos(flat1)*cos(flat2)*sin(diflon/2.0)*sin(diflon/2.0));
  float c = 2*atan2(sqrt(a),sqrt(1.0-a));
  float d = 6371000.0*c;
  return d;
}

void geo_fencing(int max_distance, float distance){
  u8g2.setFont(u8g2_font_t0_12_mr);
  u8g2.clearBuffer();
  u8g2.drawCircle(20,20,13,U8G2_DRAW_ALL);
  u8g2.drawCircle(20,20,0.5,U8G2_DRAW_ALL);
  u8g2.drawLine(20,20,33,20);
  u8g2.drawLine(20,20,22,25);
  u8g2.drawLine(20,20,22,15);
  u8g2.drawLine(33,20,29,25);
  u8g2.drawLine(33,20,29,15);
  u8g2.setCursor(40,18);
  u8g2.print("Max");
  u8g2.setCursor(40,28);
  u8g2.print("Distance: ");
  u8g2.setCursor(95,28);
  u8g2.print((String)max_distance);
  u8g2.drawBitmap(10,36,3,24,distance_BMP_24x24);
  u8g2.setCursor(40,55);
  u8g2.print("Distance: ");
  u8g2.setCursor(95,55);
  u8g2.print((String)distance);
  u8g2.sendBuffer();
}

void setup()
{
  Serial.begin(115200);
  pinMode(button, INPUT);
  attachInterrupt(button,isr, RISING);
  dht.begin();
  Serial.println("device serial initialize");
 
  sim800.begin(9600);
  Serial.println("SIM800L serial initialize");
 
  Serial.println("Initializing modem...");
  modem.restart();

  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
 
  http_client.setHttpResponseTimeout(10 * 1000); //^0 secs timeout

  Serial.begin(115200);
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
}
 
void loop()
{ 
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
 
  while (true) {
    if (!http_client.connected())
    {
      Serial.println();
      http_client.stop();// Shutdown
      Serial.println("HTTP  not connect");
      break;
    }
    else
    {
      // dht_loop();
      // GET();
      while(is_login == 1){
    getFormFirebase_state("State/Is_Login",&http_client);
    is_login = new_response.toInt();
    Serial.print("Is Login: ");
    Serial.println(is_login);
    if(is_login == 0 || is_menu == 1) break;
      }

    while(is_login == 0) {
      login_get();
      delay(500);
      getFormFirebase_state("State/Is_Menu",&http_client);
      is_menu = new_response.toInt();;
      Serial.print("Is Menu: ");
      Serial.println(is_menu);
      if(is_menu == 1) break;
    } 

    while(is_menu == 1) {
      getFormFirebase_state("State/Is_Tracking",&http_client);
      is_tracking = new_response.toInt();;
      Serial.print("Is Tracking: ");
      Serial.println(is_tracking);
      getFormFirebase_state("State/Is_Fencing",&http_client);
      is_fencing = new_response.toInt();;
      Serial.print("Is fencing: ");
      Serial.println(is_fencing);
      if(is_tracking == 1) {
          a += 0.1;
  b += 0.1;
  c += 1;
  d += 1;
  e += 1;
  delay(200);
        unsigned long display_time = millis();
        dht_loop();
        tracker();
        if((unsigned long)(millis() - display_time > 5000)){
          needle_pos = map(c,0,200,0,90); //SET NEEDLE
        // show needle and dial
          xx = needle_pos;                                    
          if (xx<45)
            {xx=xx+135;}
          else
            {xx=xx-45;} 
          {
            u8g2.firstPage(); 
            do {             
              gauge(xx);
            }
            while( u8g2.nextPage());
          }
            display_time = millis();
        }
      }

      if(is_fencing == 1){
        getFormFirebase_state("State/Is_Fencing_Install",&http_client);
        is_fencing_install = new_response.toInt();;
        Serial.print("Is Fencing Install: ");
        Serial.println(is_fencing_install);
        getFormFirebase_state("State/Is_Fencing_Running",&http_client);
        is_fencing_running = new_response.toInt();;
        Serial.print("Is fencing_Running: ");
        Serial.println(is_fencing_running);

        if(is_fencing_install == 1){
        getFormFirebase_info("Fencing/init_lat",&http_client);
        init_lat = new_response.toFloat();;
        Serial.print("Init lat: ");
        Serial.println(init_lat);
        getFormFirebase_info("Fencing/init_long",&http_client);
        init_long = new_response.toFloat();;
        Serial.println("Init long: ");
        Serial.println(init_long);
        getFormFirebase_info("Fencing/max_dist",&http_client);
        max_distance = new_response.toInt();;
        Serial.println("Max Distance: ");
        Serial.println(max_distance);
        }

        if(is_fencing_running == 1){
                    a += 0.0001;
  b += 0.0001;
  c += 1;
  d += 1;
  e += 1;
  delay(200);
          dht_loop();
              distance = getDistance(a, b, init_lat, init_long);
              String f = String(distance);
              delay(100);
              Serial.println(f);
              String Data = "{";
              Data += "\"dist\":" + f + "";
              Data += "}";
              PostToFirebase("PATCH", "Fencing", Data, &http_client);
              Serial.print("current Distance= "); Serial.println(distance);
              geo_fencing(max_distance,distance);
           }
      }
    }
  }


  if(isPress){
  Serial.println("dang chay task Call");
  sim800.println("ATD+ "+MY_PHONE_NUMBER+";");
  delay(20000);
  sim800.println("ATH");
  delay(1000);
  sim800.println("AT+CMGF=1");
  delay(500);
  sim800.println("AT+CMGS=\""+MY_PHONE_NUMBER+"\"");
  delay(100);
  sim800.print("http://maps.google.com/maps?q=loc:");
  sim800.print("hi");
  sim800.print(",");
  sim800.print("ho");
  delay(10);
  sim800.write(26);
  Serial.println("SMS Sent Successfully");
  isPress = false;
    }
  }
}