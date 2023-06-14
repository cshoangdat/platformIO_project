#include <Arduino.h>
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256

#include <stdio.h>
#include <TinyGPS++.h>
#include <TinyGsmClient.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "SoftwareSerial.h"
#include <ArduinoHttpClient.h>

#define SCL 22
#define SDA 21

#define rxPin 5
#define txPin 18

#define RXD2 16
#define TXD2 17

#define buttonSMS 15
#define buttonCall 4

const char FIREBASE_HOST[] = "test-107cd-default-rtdb.firebaseio.com";
const String FIREBASE_AUTH = "7JlDcUDlAiX8iGktmbLcwPD0gbrrJekgGk5uPLAY";
const String FIREBASE_PATH = "data";
const int SSL_PORT = 443;

char apn[] = "m-wap";
char user[] = "mms";
char pass[] = "mms";

const String PHONE = "+84903847805";
const String MY_PHONE_NUMBER = "+84363604119";

SoftwareSerial neogps(rxPin, txPin);
TinyGPSPlus gps;
HardwareSerial sim800(2);
TinyGsm modem(sim800);
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // ESP32 Thing, HW I2C with pin remapping

float latitude = 0,longitude = 0,speed;
int satellites;
//--------------------------------------------------------------

#define BUZZER 4

// Alarm
int buzzer_timer = 0;
// boolean alarm = false;
boolean send_alert_once = true;
//--------------------------------------------------------------
// Size of the geo fence (in meters)
const float maxDistance = 100;

//--------------------------------------------------------------
float initialLatitude = 0;
float initialLongitude = 0;

//--------------------------------------------------------------


void getGps(float& latitude, float& longitude);


/*****************************************************************************************
 * setup() function
 *****************************************************************************************/
void setup()
{
  //--------------------------------------------------------------
  //Serial.println("Arduino serial initialize");
  Serial.begin(115200);
  //--------------------------------------------------------------
  //Serial.println("SIM800L serial initialize");
  sim800.begin(9600);
  //--------------------------------------------------------------
  //Serial.println("NEO6M serial initialize");
  neogps.begin(9600);
  //--------------------------------------------------------------
  pinMode(BUZZER, OUTPUT);
  //--------------------------------------------------------------
  // sim800.println("AT"); //Check GSM Module
  // delay(1000);
  // sim800.println("ATE1"); //Echo ON
  // delay(1000);
  // sim800.println("AT+CPIN?"); //Check SIM ready
  // delay(1000);
  // sim800.println("AT+CMGF=1"); //SMS text mode
  // delay(1000);
  // sim800.println("AT+CNMI=1,1,0,0,0"); /// Decides how newly arrived SMS should be handled
  // delay(1000);
  //AT +CNMI = 2,1,0,0,0 - AT +CNMI = 2,2,0,0,0 (both are same)
  //--------------------------------------------------------------
  // delay(20000);
  buzzer_timer = millis();
}





/*****************************************************************************************
 * loop() function
 *****************************************************************************************/
float getDistance(float flat1, float flon1, float flat2, float flon2);
void loop()
{
  //--------------------------------------------------------------
  // gps_task();
  latitude +=0.0001;
  longitude +=0.0001;
  delay(1000); 
  //--------------------------------------------------------------
  float distance = getDistance(latitude, longitude, initialLatitude, initialLongitude);
  //--------------------------------------------------------------
  Serial.print("Latitude= "); Serial.println(latitude, 6);
  Serial.print("Lngitude= "); Serial.println(longitude, 6);
  Serial.print("initialLatitude= "); Serial.println(initialLatitude, 6);
  Serial.print("initialLngitude= "); Serial.println(initialLongitude, 6);
  Serial.print("current Distance= "); Serial.println(distance);
  //--------------------------------------------------------------
  // Set alarm on?
  if(distance > maxDistance) {
    //------------------------------------------
    // if(send_alert_once == true){
    //   digitalWrite(BUZZER, HIGH);
    //   // sendAlert();
    //   alarm = true;
    //   send_alert_once = false;
    //   buzzer_timer = millis();
    // }
    //------------------------------------------
    Serial.println("qua gioi han");
  }
  // else{
  //   send_alert_once = true;
  // }
  // //--------------------------------------------------------------

  // // Handle alarm
  // if (alarm == true) {
  //   if (millis() - buzzer_timer > 5000) {
  //     digitalWrite(BUZZER, LOW);
  //     alarm = false;
  //     buzzer_timer = 0;
  //   }
  // }
  //--------------------------------------------------------------  
  while(sim800.available()){
    Serial.println(sim800.readString());
  }
  //--------------------------------------------------------------
  while(Serial.available())  {
    sim800.println(Serial.readString());
  }
  //--------------------------------------------------------------


}



/*****************************************************************************************
* getDistance() function
*****************************************************************************************/

// Calculate distance between two points using
//using haversinr formula
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
  
  // dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
  // dist_calc2 = cos(flat1);
  // dist_calc2*=cos(flat2);
  // dist_calc2*=sin(diflon/2.0);
  // dist_calc2*=sin(diflon/2.0);
  // dist_calc +=dist_calc2;

  // dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));
  
  // dist_calc*=6371000.0; //Converting to meters

  // return dist_calc;
}


/*****************************************************************************************
 * getGps() Function
*****************************************************************************************/
void gps_task()
{
   while(neogps.available()){
    if(gps.encode(neogps.read())){
      if(gps.location.isValid()){
         latitude = gps.location.lat();
         longitude = gps.location.lng();
         speed = gps.speed.kmph();
         satellites = gps.satellites.value();
      }
      else{
        u8g2.clearBuffer();
        u8g2.setCursor(2, 40);
        u8g2.print("CAN'T FIND LOCATION");
        u8g2.sendBuffer();
      }
    }
   }
}




/*****************************************************************************************
* sendAlert() function
*****************************************************************************************/
// void sendAlert()
// {
//   //return;
//   String sms_data;
//   sms_data = "Alert! The object is outside the fense.\r";
//   sms_data += "http://maps.google.com/maps?q=loc:";
//   sms_data += String(latitude) + "," + String(longitude);

//   //return;
//   sim800.print("AT+CMGF=1\r");
//   delay(1000);
//   sim800.print("AT+CMGS=\""+PHONE+"\"\r");
//   delay(1000);
//   sim800.print(sms_data);
//   delay(100);
//   sim800.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
//   delay(1000);
//   Serial.println("SMS Sent Successfully.");
  
// }