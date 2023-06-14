#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>
#define I2C_SDA 14 // SDA Connected to GPIO 14
#define I2C_SCL 15 // SCL Connected to GPIO 15
TwoWire I2CSensors = TwoWire(0);
#define RELAY_PIN 2
/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.
*/
BH1750 lightMeter(0x23);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,LOW);
  I2CSensors.begin(I2C_SDA, I2C_SCL, 100000);
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE,0x23,&I2CSensors)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
  delay(5000);
}
float lux;

void loop() {
  if (lightMeter.measurementReady()) {
    lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  }
  if(lux < 100){
    digitalWrite(RELAY_PIN, HIGH);
  }
  else digitalWrite(RELAY_PIN, LOW);
}