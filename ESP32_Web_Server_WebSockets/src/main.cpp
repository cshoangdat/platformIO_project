#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

 

// Thay thế bằng thông tin wifi của bạn

const char* ssid = "HOANG DAT";

const char* password = "24072002";

 

// Tạo đối tượng AsyncWebServer trên cổng 80

AsyncWebServer server(80);

// Tạo một đối tượng WebSocket

 

AsyncWebSocket ws("/ws");

// Đặt LED GPIO

const int ledPin1 = 12;

const int ledPin2 = 13;

const int ledPin3 = 14;

 

String message = "";

String sliderValue1 = "0";

String sliderValue2 = "0";

String sliderValue3 = "0";

 

int dutyCycle1;

int dutyCycle2;

int dutyCycle3;

 

// thiết lập thuộc tính PWM

const int freq = 5000;

const int ledChannel1 = 0;

const int ledChannel2 = 1;

const int ledChannel3 = 2;

 

const int resolution = 8;

 

//Biến Json để giữ các giá trị thanh trượt

JSONVar sliderValues;

 

//Get Slider Values

String getSliderValues(){

  sliderValues["sliderValue1"] = String(sliderValue1);

  sliderValues["sliderValue2"] = String(sliderValue2);

  sliderValues["sliderValue3"] = String(sliderValue3);

 

  String jsonString = JSON.stringify(sliderValues);

  return jsonString;

}

 

// Khởi tạo SPIFFS

void initFS() {

  if (!SPIFFS.begin()) {

    Serial.println("An error has occurred while mounting SPIFFS");

  }

  else{

   Serial.println("SPIFFS mounted successfully");

  }

}

 

// Khởi tạo WiFi

void initWiFi() {

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print('.');

    delay(1000);

  }

  Serial.println(WiFi.localIP());

}

/*chức năng thông báo cho tất cả các máy khách với các giá trị thanh trượt hiện tại. Gọi hàm này là chức năng cho phép chúng tôi thông báo các thay đổi trong tất cả các máy khách bất cứ khi nào bạn đặt vị trí mới cho thanh trượt.*/

void notifyClients(String sliderValues) {

  ws.textAll(sliderValues);

}

 

//xử lý những gì xảy ra khi máy chủ nhận được thông báo từ máy khách //thông qua giao thức WebSocket. 

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {

  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

    data[len] = 0;

    message = (char*)data;

    if (message.indexOf("1s") >= 0) {

      sliderValue1 = message.substring(2);

      dutyCycle1 = map(sliderValue1.toInt(), 0, 100, 0, 255);

      Serial.println(dutyCycle1);

      Serial.print(getSliderValues());

      notifyClients(getSliderValues());

    }

    if (message.indexOf("2s") >= 0) {

      sliderValue2 = message.substring(2);

      dutyCycle2 = map(sliderValue2.toInt(), 0, 100, 0, 255);

      Serial.println(dutyCycle2);

      Serial.print(getSliderValues());

      notifyClients(getSliderValues());

    }    

    if (message.indexOf("3s") >= 0) {

      sliderValue3 = message.substring(2);

      dutyCycle3 = map(sliderValue3.toInt(), 0, 100, 0, 255);

      Serial.println(dutyCycle3);

      Serial.print(getSliderValues());

      notifyClients(getSliderValues());

    }

    if (strcmp((char*)data, "getValues") == 0) {

      notifyClients(getSliderValues());

    }

  }

}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  switch (type) {

    case WS_EVT_CONNECT:

      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());

      break;

    case WS_EVT_DISCONNECT:

      Serial.printf("WebSocket client #%u disconnected\n", client->id());

      break;

    case WS_EVT_DATA:

      handleWebSocketMessage(arg, data, len);

      break;

    case WS_EVT_PONG:

    case WS_EVT_ERROR:

      break;

  }

}

 

void initWebSocket() {

  ws.onEvent(onEvent);

  server.addHandler(&ws);

}




void setup() {

  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);

  pinMode(ledPin2, OUTPUT);

  pinMode(ledPin3, OUTPUT);

  initFS();

  initWiFi();

 

  // cấu hình chức năng LED PWM

  ledcSetup(ledChannel1, freq, resolution);

  ledcSetup(ledChannel2, freq, resolution);

  ledcSetup(ledChannel3, freq, resolution);

 

  // gắn kênh vào GPIO cần điều khiển

  ledcAttachPin(ledPin1, ledChannel1); 

  ledcAttachPin(ledPin2, ledChannel2);

  ledcAttachPin(ledPin3, ledChannel3);

  initWebSocket();

 

  // URL gốc máy chủ web

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){

    request->send(SPIFFS, "/index.html", "text/html");

  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server

  server.begin();

}

void loop() {

  ledcWrite(ledChannel1, dutyCycle1);

  ledcWrite(ledChannel2, dutyCycle2);

  ledcWrite(ledChannel3, dutyCycle3);

  ws.cleanupClients();

}