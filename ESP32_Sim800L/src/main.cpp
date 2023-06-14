#include <Arduino.h>
#include <Wire.h>

#define RX2 16
#define TX2 17

HardwareSerial mySerial(2);

void updateSerial(){
  delay(500);
  while(Serial.available()){
    mySerial.write(Serial.read());
  }
  while(mySerial.available()){
    Serial.write(mySerial.read());
  }
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RX2, TX2);
  mySerial.println("AT"); //Kiem tra module co hoat dong khong
  updateSerial();
//kiem tra thong tin lien quan toi sim
  mySerial.println("AT+CSQ"); //kiem tra chat luong song tra ve tu 0 toi 31 tuong ung voi cuong do song, cuong do song cang cao cang tot, <18 coi lai
  updateSerial();
  mySerial.println("AT+CCID"); //lay so the sim
  updateSerial();
  mySerial.println("AT+CREG?"); // kiem tra ma the sim
  updateSerial();
  mySerial.println("AT+CBC"); //kiem tra pin, so thu 2 la %pin, so thu 3 la dien ap thuc te mV
  updateSerial();
  mySerial.println("AT+COPS?"); // kiem tra nha mang
  updateSerial();
  mySerial.println("AT+CUSD=1"); //kiem tra tai khoan
  updateSerial();
  mySerial.println("ATD*101#");
  updateSerial();
//Gui tin nhan SMS
  // mySerial.println("AT+CMGF=1"); //chon dinh dang SMS la van ban
  // updateSerial();
  // mySerial.println("AT+CMGS=\"+84363604119\""); //
  // updateSerial();
  // mySerial.print("Hihi so nhap vao la 9 so bo so 0 dau nhe");
  // updateSerial();
  // mySerial.write(26);
// Doc tin nhan SMS
  // mySerial.println("AT+CMGF=1");
  // updateSerial();
  // mySerial.println("AT+CNMI=1,2,0,0,0");// Decides how newly arrived SMS messages should be handled
  // updateSerial();
// thuc hien cuoc goi
  mySerial.println("ATD+ +84363604119;"); //goi vao so
  updateSerial();
  delay(50000);
  mySerial.println("ATH"); //ket thuc cuoc goi
  updateSerial();
//nhan cuoc goi
  //ATA -> chap nhan cuoc goi
}

void loop() {
   updateSerial();
}

