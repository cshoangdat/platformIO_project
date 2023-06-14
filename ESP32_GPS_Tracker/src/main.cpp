#include <Arduino.h>

#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 256

#define TINY_GSM_MODEM_SIM800

#include <TinyGPS++.h>
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "SoftwareSerial.h"

#define SCL 22
#define SDA 21

#define rxPin 5
#define txPin 18

#define RXD2 16
#define TXD2 17

#define buttonSMS 15
#define buttonCall 4

char auth[] = "s9LBWfAWifT0jNf_ICjvv0B2WLBxI4SD";

char apn[] = "m-i090";
char user[] = "mms";
char pass[] = "mms";

const String PHONE_NUMBER = "+84903847805";
const String MY_PHONE_NUMBER = "+84363604119";

SoftwareSerial neogps(rxPin, txPin);
TinyGPSPlus gps;

HardwareSerial sim800(2);
TinyGsm modem(sim800);

WidgetMap myMap(V0);
BlynkTimer timer;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // ESP32 Thing, HW I2C with pin remapping

int pointIndex = 1;
float lattitude;
float longitude;
float speed;
int satellites;
int buttonSMS_State;
int buttonCall_State;
int network;
int battery;
int p = 0;
unsigned long t = 0;
unsigned long oledTime = 0;
unsigned long currentTime = 0;

int get_network()
{
  String buff;
  //buff = "+CSQ: 16,0";
  unsigned int result, index1, index2, timeout=0;

  sim800.println("AT+CSQ");  
  for (unsigned long start = millis(); millis() - start < 2000;){
    while(sim800.available())
    {
      buff = sim800.readString();
      timeout = 1;
      break;
    }
  }

  if(timeout == 0)
    { return 0; }
    
  Serial.println(buff);
  
  //String network_status;
  //_____________________________________________________
  //Remove sent "AT Command" from the response string.
  index1 = buff.indexOf("\r");
  buff.remove(0, index1+2);
  buff.trim();
  //_____________________________________________________
  
  //_____________________________________________________
  
    index1 = buff.indexOf(":");
    index2 = buff.indexOf(",");
    buff = buff.substring(index1+1, index2);
    buff.trim();
    result = buff.toInt();

    if(result == 99)
    {
      //not known or not detectable
      return 0;
    }
    else if(result >= 2 && result <= 9)
    {
      //Signal Quality = Marginal
      return 20;
    }
    else if(result >= 10 && result <= 14)
    {
      //Signal Quality = OK
      return 40;
    }
    else if(result >= 15 && result <= 19)
    {
      //Signal Quality = Good
      return 60;
    }
    else if(result >= 20 && result <= 31)
    {
      //Signal Quality = Excellent
      return 99;
    }

}

int get_battery()
{
  String buff;
  //buff = "+CBC: 0,99,4190";
  unsigned int index1, index2, timeout=0;
    
  sim800.println("AT+CBC");  
  for (unsigned long start = millis(); millis() - start < 2000;){
    while(sim800.available())
    {
      buff = sim800.readString();
      timeout = 1;
      break;
    }
  }

  if(timeout == 0)
    { return 0; }
    
  Serial.println(buff);
  
  //String network_status;
  //_____________________________________________________
  //Remove sent "AT Command" from the response string.
  index1 = buff.indexOf("\r");
  buff.remove(0, index1+2);
  buff.trim();
  //_____________________________________________________
  
  //_____________________________________________________
  
    //search for first comma (,) = index1
    //search for 2nd comma (,) = index2
    index1 = buff.indexOf(",");
    index2 = buff.indexOf(",", index1+1);
    String result = buff.substring(index1+1, index2);
    result.trim();
    
    Serial.println(result);
    //buff.remove(0, index+2);
  return result.toInt();
}

void emergencyCall(String phoneNumber){
  sim800.println("ATD+ "+phoneNumber+";");
  delay(20000);
  sim800.println("ATH");
  delay(1000);
}

void sendSMS_Location(String text,String phoneNumber){
  sim800.println("AT+CMGF=1");
  delay(1000);
  sim800.println("AT+CMGS=\""+phoneNumber+"\"");
  delay(100);
  sim800.println(text);
  sim800.print("Latitude: ");
  sim800.println(lattitude,6);
  sim800.print("longitude: ");
  sim800.println(longitude,6);
  delay(10);
  sim800.write(26);
  delay(1000);
  Serial.println("SMS Sent Successfully");
  currentTime = 0;
}

void sendSMS(){
  sim800.println("AT+CMGF=1");
  delay(1000);
  sim800.println("AT+CMGS=\""+MY_PHONE_NUMBER+"\"");
  delay(100);
  sim800.print("http://maps.google.com/maps?q=loc:");
  sim800.print(gps.location.lat(), 6);
  sim800.print(",");
  sim800.print(gps.location.lng(), 6);
  delay(10);
  sim800.write(26);
  delay(1000);
  Serial.println("SMS Sent Successfully");
  currentTime = 0;
}

// void IRAM_ATTR ISR1(){
//   sendSMS_Location("Emergency Situation SMS!!! I'm at this location:", MY_PHONE_NUMBER);
// }

// void IRAM_ATTR ISR2(){
//   emergencyCall(MY_PHONE_NUMBER);
//   sendSMS("This is an emergency situation, please scan this QR to access to BLYNK App to track my location, QRCode: bit.ly/3VP03Pu",MY_PHONE_NUMBER);
// }

void sendToBlynk()
{
    myMap.location(pointIndex,lattitude,longitude,"GPS location");
    Blynk.virtualWrite(V1, String(lattitude,6));
    Blynk.virtualWrite(V2, String(longitude,6));
    Blynk.virtualWrite(V3, speed);
    Blynk.virtualWrite(V4, satellites);
    Blynk.virtualWrite(V5, network);
    Blynk.virtualWrite(V6, battery);
    if(buttonSMS_State == 1){
      delay(300);
      if(buttonSMS_State == 1){
        Blynk.notify("THAT IS AN EMERGENCY SITUATION");
        currentTime = 0;
      }
    }
    // if(battery < 20){
    //   Blynk.notify("Your device is almost out of battery, plug it in");
    // }
    // if(network <= 20){
    //   Blynk.notify("check your network connection level");
    // }
    if(buttonCall_State == 1){
      delay(300);
      if(buttonCall_State == 1){
        Blynk.notify("THAT IS AN EMERGENCY SITUATION");
      }
    }
    if(speed >=80){
    //sendSMS("WARNNING!!! YOU JUST HAVE OVER SPEED",MY_PHONE_NUMBER);
      Blynk.notify("WARNNING!!! YOU JUST HAVE OVER SPEED");
    }
}

void updateSerial(){
  delay(500);
  while(Serial.available()){
    sim800.write(Serial.read());
  }
  while(sim800.available()){
    Serial.write(sim800.read());
  }
}

void oledDisplay(){
  u8g2.clearBuffer();
  u8g2.setCursor(2, 30);
  u8g2.print("Lat: " + String(lattitude,6));
  u8g2.setCursor(2, 40);
  u8g2.print("long: " + String (longitude,6));
  u8g2.setCursor(2, 50);
  u8g2.print("Speed: " + String(speed));
  u8g2.setCursor(2, 60);
  u8g2.print("Satellies: " + String(satellites));
  u8g2.sendBuffer();
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonSMS, INPUT);
  pinMode(buttonCall, INPUT);
  Serial.println("esp32 serial initialize");
  delay(10);
  sim800.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("SIM800L serial initialize");
  delay(10);
  neogps.begin(9600);
  Serial.println("neogps serial initialize");
  delay(3000); 
  Serial.println("Initializing modem...");
  modem.restart();
  Blynk.begin(auth, modem, apn, user, pass, "blynk.iot-cm.com", 8080);
  u8g2.begin();
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.enableUTF8Print();
  u8g2.setFontMode(1);
  u8g2.clearBuffer();
  u8g2.setCursor(2, 20);
  u8g2.print("GPS Tracker");
  u8g2.sendBuffer();
  // xTaskCreatePinnedToCore(taskGPS,"taskGPS",1024,NULL,2,NULL,0);
  network = get_network();
  battery = get_battery();
  timer.setInterval(200L, sendToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
  buttonSMS_State = digitalRead(buttonSMS);
  buttonCall_State = digitalRead(buttonCall);
 while(neogps.available()){
    if(gps.encode(neogps.read())){
      if(gps.location.isValid()){
         lattitude = gps.location.lat();
         longitude = gps.location.lng();
         speed = gps.speed.kmph();
         satellites = gps.satellites.value();
             Serial.print("Latitude:  ");
    Serial.println(lattitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    Serial.print("Speed: ");
    Serial.println(speed, 6);
         oledDisplay();
      }
      // else{
      //   u8g2.clearBuffer();
      //   u8g2.setCursor(2, 40);
      //   u8g2.print("CAN'T FIND LOCATION");
      //   u8g2.sendBuffer();
      // }
    }
  }

  if (buttonSMS_State == 1)
    {
      delay(300);
      if (buttonSMS_State == 1)
      {
        sendSMS_Location("Emergency Situation SMS!!! I'm at this location:", MY_PHONE_NUMBER);
      }
  }

  if(buttonCall_State == 1){
    delay(300);
    if(buttonCall_State == 1){
      emergencyCall(MY_PHONE_NUMBER);
      sendSMS();
    }
  }

  if((unsigned long) (millis - t) > 20000){
  network = get_network();
  battery = get_battery();
  t = millis();
  }
  //Geo Fencing
  
  updateSerial();
} 





