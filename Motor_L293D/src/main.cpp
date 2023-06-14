#include <MotorDriver.h>
#include <Arduino.h>

MotorDriver m;


void setup()
{

}


void loop()
{
  m.motor(2,FORWARD,255);
  delay(1000);
  m.motor(2,BACKWARD,255);
  delay(500);  
  m.motor(2,RELEASE,0);
}