#include <Arduino.h>
#include <Wire.h>

const byte DS1307 = 0x68;
const byte NumberofFields = 7;

int second,minute,hour,day,wday,month,year;

int bcd2dec(byte num){
return((num/16*10)+ (num%16));
}

int dec2bcd(byte num){
return((num/10*16) + (num%10));
}

void readDS1307(){
Wire.beginTransmission(DS1307);
Wire.write((byte)0x00);
Wire.endTransmission();
Wire.requestFrom(DS1307,NumberofFields);
second = bcd2dec(Wire.read() & 0x7f);
minute = bcd2dec(Wire.read() & 0x7f);
hour = bcd2dec(Wire.read() & 0x3f);
wday = bcd2dec(Wire.read());
day = bcd2dec(Wire.read());
month = bcd2dec(Wire.read());
year = bcd2dec(Wire.read());
year += 2000;
}

void printDigits(int digits){
Serial.print(":");
if(digits<10)
Serial.print('0');
Serial.print(digits);
}

void digitalClockDisplay(){
Serial.print(hour);
printDigits(minute);
printDigits(second);
Serial.print(" ");
Serial.print(day);
Serial.print(" ");
Serial.print(month);
Serial.print(" ");
Serial.print(year);
Serial.println();
}

void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr){
Wire.beginTransmission(DS1307);
Wire.write(byte(0x00));
Wire.write(dec2bcd(sec));
Wire.write(dec2bcd(min));
Wire.write(dec2bcd(hr));
Wire.write(dec2bcd(wd));
Wire.write(dec2bcd(d));
Wire.write(dec2bcd(mth));
Wire.write(dec2bcd(yr));
Wire.endTransmission();
}

void setup(){
Wire.begin();
Serial.begin(9600);
setTime(12,30,45,1,8,2,15);
}

void loop(){
readDS1307();
digitalClockDisplay();
delay(1000);
}

