#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "EEPROM.h"
#include "Servo.h"

//////////////////////
#define STEERING_PIN 4 // D2
//#define STEERING_PIN 15 // D8

////////////////
#define PWM_L 14 // D5
#define PWM_R 12 // D6


//////////////////////
#define EEPROM_SIZE 64

//////////////////////
#define LED_PIN     16 // In Board
//#define LED_PIN     2 // In Module

/////////////////
#define PORT 8000

//////////////////////////////////
const char* sta_ssid = "HOANG DAT";
const char* sta_password = "dat24072002";

//////////////////////////////////
IPAddress STA_IP(192, 168, 1, 71);
IPAddress STA_Gateway(192, 168, 1, 1);
IPAddress STA_Subnet(255, 255, 0, 0);
IPAddress STA_primaryDNS(8, 8, 8, 8);
IPAddress STA_secondaryDNS(8, 8, 4, 4);

////////////////////////
WiFiServer server(PORT);
WiFiClient client;

/////////////////////
Servo steering_servo;

///////////////////////
bool led_toggle = true,
     client_connected = false;

String server_rx;

uint8_t server_rx_counter,
        server_rx_throttle,
        server_rx_steering;

uint8_t throttle_control;
uint8_t steering_control;

uint8_t throttle_trim;
uint8_t steering_trim;

uint32_t last_server_ping,
         server_ping;

////////////////////////
void Throttle_Init(void) {
  pinMode(5, OUTPUT); // D1
  pinMode(2, OUTPUT); // D4
  ///////////////////////
  digitalWrite(5, HIGH);
  digitalWrite(2, HIGH);

  //////////////////////
  analogWrite(PWM_L, 0);
}

////////////////////////////////////
void Throttle_Control(uint8_t Speed) {
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  //////////////////////////
  analogWrite(PWM_L, Speed);
}

////////////////////////
void Steering_Init(void) {
  steering_servo.attach(STEERING_PIN);
}

////////////////////////////////////
void Steering_Control(uint8_t Angle) {
  steering_servo.write(Angle);
}

/////////////////////////
void Client_Process(void) {
  if (server_rx.indexOf("C(") >= 0 || server_rx.indexOf("S(") >= 0) {
    server_rx_counter = server_rx.substring(server_rx.indexOf('p') + 1, server_rx.indexOf('t')).toInt();
    server_rx_throttle = server_rx.substring(server_rx.indexOf('t') + 1, server_rx.indexOf('s')).toInt();
    server_rx_steering = server_rx.substring(server_rx.indexOf('s') + 1, server_rx.indexOf(')')).toInt();
    if (server_rx.indexOf("C(") >= 0) {
      steering_control = server_rx_steering;
      throttle_control = server_rx_throttle;

      Serial.print("Steering Control: ");
      Serial.print(steering_control);
      Serial.print(" , ");
      Serial.print("Throttle Control: ");
      Serial.print(throttle_control);
      Serial.print(" , ");

      Steering_Control(steering_control);
      Throttle_Control(throttle_control);
    }
    if (server_rx.indexOf("S(") >= 0) {
      throttle_trim = server_rx_throttle;
      steering_trim = server_rx_steering;
      EEPROM.write(0, throttle_trim);
      EEPROM.write(1, steering_trim);
      EEPROM.commit();
      Serial.print("Steering Trim: ");
      Serial.print(steering_trim);
      Serial.print(" , ");
      Serial.print("Throttle Trim: ");
      Serial.print(throttle_trim);
      Serial.print(" , ");
    }
    led_toggle = !led_toggle;
    digitalWrite(LED_PIN, led_toggle);
    server_ping = millis() - last_server_ping;
    last_server_ping = millis();
    Serial.print("Ping:");
    Serial.println(server_ping);
    server_rx = "";
  }
}

////////////////
void setup(void) {

  /////////////////////
  Serial.begin(115200);

  /////////////////////////
  pinMode(LED_PIN, OUTPUT);
  led_toggle = true;
  digitalWrite(LED_PIN, led_toggle);

  //////////////////////////
  EEPROM.begin(EEPROM_SIZE);
  delay(1000);
  throttle_trim = (uint8_t)EEPROM.read(0);
  steering_trim = (uint8_t)EEPROM.read(1);
  Serial.print("Steering Trim Load: ");
  Serial.print(steering_trim);
  Serial.print(" , ");
  Serial.print("Throttle Trim Load: ");
  Serial.println(throttle_trim);

  ////////////////
  Throttle_Init();
  Throttle_Control(throttle_trim);

  ////////////////
  Steering_Init();
  Steering_Control(steering_trim);

  ////////////////////
  WiFi.mode(WIFI_STA);

  //////////////////////
  WiFi.disconnect(true);

  ///////////////////////////////////
  WiFi.begin(sta_ssid, sta_password);

  /////////////////////////////////////
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WIFI IP Address: ");
  Serial.println(WiFi.localIP());

  ///////////////
  server.begin();
}

///////////////
void loop(void) {

  client = server.available();
  if (client) {
    client_connected = true;
    while (client.connected()) {
      if (client.available() > 0) {
        char inChar = (char)client.read();
        if (inChar == '\n') {
          Client_Process();
          client.write('\n');
        }
        server_rx += inChar;
      }
    }
  }
  else {
    client_connected = false;
    led_toggle = true;
    digitalWrite(LED_PIN, led_toggle);
    throttle_control = throttle_trim;
    steering_control = steering_trim;
    last_server_ping = millis();
    server_ping = 0;
  }

  //////////
  delay(10);
}