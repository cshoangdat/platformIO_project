#include <Arduino.h>
// khai báo thư viện 
  #include <SoftwareSerial.h> // khai báo thư viện cho bluetooth
  #include <Servo.h>  // khai báo thư viện cho servo
  #include <SPI.h>    
  #include <RFID.h>  // khai báo thư viện cho rfid

// khai báo các chân 

  SoftwareSerial blue(9, 10);   // khai báo chân cho RX, TX
  #define button1 6             // KHAI BÁO CHÂN CHO NÚT NHẤN 1
  #define button2 5             // KHAI BÁO CHÂN CHO NÚT NHẤN 2
  #define button3 7             // KHAI BÁO CHÂN CHO NÚT NHẤN 3
  #define led1 4                // KHAI BÁO CHÂN CHO LED 1
  #define led2 2                // KHAI BÁO CHÂN CHO LED 2
  #define led3 3                // KHAI BÁO CHÂN CHO LED 3
  #define led 8                 // KHAI BÁO CHÂN CHO LED TRƯỚC CỬA
  #define  echoPin  11          // KHAI BÁO CHÂN ECHOPIN CHO CẢM BIẾN SIÊU ÂM                     
  #define  initPin  12          // KHAI BÁO CHÂN INT CHO CẢM BIẾN SIÊU ÂM 
  #define servoPin1 25          // KHAI BÁO CHÂN CHO SERVO 1
  #define rainSensor 13         //  KHAI BÁO CHÂN CHO CẢM BIẾN MƯA 
  #define servoPin2 44          // KHAI BÁO CHÂN CHO SERVO 2
  #define SS_PIN 53             // KHAI BÁO CHÂN SS CHO RFID      
  #define RST_PIN 46            // KHAI BÁO CHÂN RST CHO RFID
  #define relay 28              // KHAI BÁO CHÂN CHO RELAY
  #define lm35 A0               // KHAI BÁO CHÂN CHO CẢM BIẾN NHIỆT ĐỘ LM35

// khai báo biến 
unsigned long my_time = 0;
 //khai báo biến cho led trong phòng
  uint8_t dem1 = 0;
  uint8_t dem2 = 0;
  uint8_t dem3 = 0;
  byte state;
 // khai báo biến cho led trước cửa
  unsigned long pulseTime = 0;         
  unsigned long distance = 0;  
 //khai báo biến cho cửa sổ 
  int servoPos=0;
 // khai báo biến cho cửa chính
  uint8_t goc = 0;
  unsigned char reading_card[5];
  unsigned char master[5] = { 208,147,37,27}; // mã để mở cửa 
  unsigned char slave[5] = { 162,119,95,26 }; //  mã để đóng cửa 
  unsigned char i, j; 
 // khai báo biến cho quạt
  int bandau = 30;
  byte count = 0;
Servo myServo1;

Servo myservo2;
RFID rfid(SS_PIN, RST_PIN);


void setup(){ 
  
  // led phòng
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  //led trước cửa

  pinMode(initPin, OUTPUT);          

  pinMode(echoPin, INPUT);  

  // cửa sổ

  pinMode(rainSensor,INPUT);
  myServo1.attach(servoPin1);

  // cửa chính

  myservo2.attach(servoPin2);
    SPI.begin();
    rfid.init();

  // quạt 

   pinMode(relay, OUTPUT);
   
 Serial.begin(9600);
 blue.begin(9600);
   
}

// led phòng

void led_handle(){
   digitalWrite(led1, dem1);
   digitalWrite(led2, dem2);
   digitalWrite(led3, dem3);
     
 }
void readbutton(unsigned char pin, uint8_t *temp){
    if(!digitalRead(pin)){
      delay(10);
        if(!digitalRead(pin))
        while(!digitalRead(pin)){
          void read_blue();
          }
          *temp = 1 - (*temp);       
      }
}

  // hc06
  
void read_blue(){
    if(blue.available())
     state = blue.read();
    else 
      state = 0;
     switch (state)
  {
    case 1:
         dem1 = 1;
            break;
    case 2:
         dem1 = 0;
            break;
    case 3:
         dem2 = 1;
            break;
    case'4':
         dem2 = 0;
             break;
    case'5':
         dem3 = 1;
             break;
    case'6':
         dem3 = 0;
             break;
    case 7:
         servoPos = 180; 
             break;
    case 8:
         servoPos = 0;
             break;
    case 9:
         goc = 180;
             break;
    case 10:
         goc = 0; 
              break;
    case 11:
         digitalWrite(relay,HIGH);
              break;
    case 12:
         digitalWrite(relay, LOW);
              break;
     default:
     break;
  }  
}

// cửa chính 

 void read_rfid() {
  if (rfid.isCard()) {
 
        if (rfid.readCardSerial())
        {
 
            for (i = 0; i < 5; i++) {
 
                reading_card[i] = rfid.serNum[i]; 
            }
            for (i = 0; i < 5; i++) {
              
                if (reading_card[i] != master[i]) 
                {
                    break;
                }
            }
            for (j = 0; j < 5; j++) {
                if (reading_card[i] != slave[i]) {
                    break;
                }
            }
            if (i == 4) 
            {
            goc = 180;
            }
            if (j == 4) {
            goc = 0;
            }
        }
  }
  
        rfid.halt();
}
   
  void loop(){
    
    // hc06
      void read_blue();
      
    // led phòng
      readbutton(button2, &dem1);
      readbutton(button1, &dem2);
      readbutton(button3, &dem3);
      led_handle();

    // led trước cửa
 digitalWrite(initPin, HIGH);            

  delayMicroseconds(10);                

  digitalWrite(initPin, LOW);             

  pulseTime = pulseIn(echoPin, HIGH); 

  distance = pulseTime/29.1/2;              
  delay(20);
  if(distance<20){
    my_time = millis();
    digitalWrite(led,1);
  }
      else
if(millis() - my_time >= 3000 and digitalRead(led) == 1)
    digitalWrite(led,0);
    
    // cửa sổ
 int value = digitalRead(rainSensor);
 
 if(value==HIGH){
  if(servoPos<=180)
  {
  servoPos = 0;
  }
 }
 myServo1.write(servoPos);
 
    // cửa chính
      void read_rfid();
      myservo2.write(goc);
      
     //quạt
  int reading = analogRead(lm35);
  float t = (float)reading*500/1024;
  delay(20);
  if(t >= bandau){
  digitalWrite(relay,HIGH);
  count = 0;
  }
  else{
    count++;
    if(count >= 4){
    digitalWrite(relay,LOW);
    }
  }
}