#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <SPIFFS.h>
#include <FS.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

#include "DHT.h"

#define PIR_PIN 13
#define DHTPIN 14 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

float lastTemperature;
float lastHumidity;

const char* ssid = "HOANG DAT";
const char* password = "24072002";

#define API_KEY "AIzaSyBSE_jDV8mBtvLDcInc9M4yJzHJ8DgFKrc"
#define USER_EMAIL "csdathoang88@gmail.com"
#define USER_PASSWORD "dat123456"

#define STORAGE_BUCKET_ID "securitycam-3de3b.appspot.com"
#define FILE_PHOTO "/data/photo.jpg"

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

boolean takeNewPhoto = true;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

boolean taskCompleted = false;

bool checkPhoto(fs::FS &fs){
  File f_pic = fs.open(FILE_PHOTO);
  unsigned int pic_sz = f_pic.size();
  return (pic_sz>100);
}

void capturePhotoSaveSpiffs(void){
  camera_fb_t *fb = NULL;
  bool ok = 0;
  do{
    Serial.println("Taking a photo...");
    fb = esp_camera_fb_get();
    if(!fb){
      Serial.println("Camera capture failed");
    }
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
    if(!file)
    {
      Serial.println("failed to open file in writing mode");
    }
    else{
      file.write(fb->buf, fb->len);
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    file.close();
    esp_camera_fb_return(fb);
    ok = checkPhoto(SPIFFS);
  }while(!ok);
}

void initWiFi(){
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.print(WiFi.localIP());
}

void initSPIFFS(){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else{
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }
}

void initCamera(){
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  } 
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  initWiFi();
  initSPIFFS();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG,0);
  initCamera();
  configF.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback;

  Firebase.begin(&configF,&auth);
  Firebase.reconnectWiFi(true);
}

unsigned long previousMillis = 0;

void loop() {
  bool iscompleted = false;
  int PIR_STATE = digitalRead(PIR_PIN);
  unsigned long currentMillis = millis();
  int interval = 2000;
  float temp = dht.readTemperature(); //đọc giá trị nhiệt độ từ dht
  float humid = dht.readHumidity(); // đọc giá trị độ ẩm từ dht
  if(isnan(temp) || isnan(humid)){
    temp = lastTemperature;
    humid = lastHumidity;
  }
  else{
    lastTemperature = temp;
    lastHumidity = humid;
  }
  Serial.println();
  Serial.print("Nhiet do: ");
  Serial.println(temp);
  Serial.print("Do am: ");
  Serial.println(humid);

  // previousMillis = currentMillis;
  if(PIR_STATE == 1){
    capturePhotoSaveSpiffs();
    Serial.println("Co nguoi");
    if(Firebase.ready()){
      Serial.print("Uploading picture... ");
      while(iscompleted == false){
        if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, FILE_PHOTO /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, FILE_PHOTO /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */)){
          Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
          iscompleted = true;
          delay(500);
        }
        else{
          Serial.println(fbdo.errorReason());
        }
      }
    }
  }  
  else{
    Serial.println("khong co nguoi");
  }
  // if((unsigned long)(currentMillis - previousMillis) >= interval){

  // }
}
  
  // if(takeNewPhoto){
  //   capturePhotoSaveSpiffs();
  //   takeNewPhoto = false;
  // }
  // delay(1);
  // if(Firebase.ready() && !taskCompleted){
  //   taskCompleted = true;
  //   Serial.print("Uploading picture... ");
  //   if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, FILE_PHOTO /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, FILE_PHOTO /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */)){
  //     Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
  //   }
  //   else{
  //     Serial.println(fbdo.errorReason());
  //   }
  // }
// }