#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "DatHoang";
const char *password = "dat123456";

// gan cho ESP32 IP tinh

IPAddress local_ip(192, 68, 4, 1);
IPAddress gateway(192, 68, 4, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

char LED1Pin = 19;
bool LED1Status = 0;
char LED2Pin = 18;
bool LED2Status = 0;

void handle_onConnect();
void handle_led1On();
void handle_led1Off();
void handle_led2On();
void handle_led2Off();
void handle_notFound();
String SendHTML(uint8_t led1Stat, uint8_t led2Stat);

void setup() {
  Serial.begin(115200);
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_onConnect);
  server.on("/led1On", handle_led1On);
  server.on("/led1Off", handle_led1Off);
  server.on("/led2On", handle_led2On);
  server.on("/led2Off", handle_led2Off);
  server.onNotFound(handle_notFound);
  server.begin();
  Serial.println("HTTP server started");
  // put your setup code here, to run once:
}

void loop() {
  server.handleClient();
  if(LED1Status == HIGH)
    digitalWrite(LED1Pin, HIGH);
  else
    digitalWrite(LED1Pin, LOW);
  if(LED2Status == HIGH)
    digitalWrite(LED2Pin, HIGH);
  else
    digitalWrite(LED2Pin, LOW);
  // put your main code here, to run repeatedly:
}

void handle_onConnect(){
  LED1Status = LOW;
  LED2Status = LOW;
  Serial.println("GPIO18 Status: OFF | GPIO19 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1Status, LED2Status));
}

void handle_led1On(){
  LED1Status = HIGH;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2Status));
}

void handle_led1Off(){
  LED1Status = LOW;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(false,LED2Status));
}

void handle_led2On(){
  LED2Status = HIGH;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(LED1Status,true));
}

void handle_led2Off(){
  LED2Status = LOW;
  Serial.println("GPIO19 Status: ON");
  server.send(200, "text/html", SendHTML(LED1Status, false));
}

void handle_notFound(){
  server.send(200, "text/html", "Page Not Found");
}

String SendHTML(uint8_t led1Stat, uint8_t led2Stat){
  String ptr = "<!DOCTYPE html> \n";
  ptr += "<html lang=\"en\"> \n";
  ptr += "<head>\n";
  ptr += "<meta charset=\"UTF-8\"/>\n";
  ptr += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
  ptr += "<title>control LED</title>\n";
  ptr += "<style>\n";
  ptr += ".title, .content {text-align: center;font-size: large;}\n";
  ptr += ".title{ text-transform: uppercase; line-height: 2rem; font-size: x-large;}\n";
  ptr += ".sub - content{line-height: 5rem; padding-top: 20px; color: #aaa;}\n";
  ptr += ".button{cursor: pointer; padding: 1.3rem; border-radius: 5px;}\n";
  ptr += ".button__on{background-color: #66FF00; color: #FFFFFF; padding: 22px 52px;}\n";
  ptr += ".button__off{background-color: #FF0000;color: #FFFFFF;padding: 21px 50px;}\n";
  ptr += "*{box-sizing: inherit;}\n";
  ptr += "html{font-size: 62.5%; line-height: 1.6rem; font-family: 'Roboto', sans-serif; box-sizing: border-box;}\n";
  ptr += "li{list-style: none;}\n";
  ptr += "a{text-decoration: none;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1 class=\"title\">Control LED Device</h1>\n";
  ptr += "<h3 class=\"title\">AP mode - Access Point</h3>\n";
  ptr += "<div class=\"content\">\n";

  if(led1Stat){
  ptr += "<p class=\"sub-content\">LED 1 status: ON</p>\n";
  ptr += "<a href=\"/led1Off\" class=\"button button__off\">OFF</a>\n";
  }
  else{
  ptr += "<p class=\"sub-content\">LED 1 status: OFF</p>\n";
  ptr += "<a href=\"/led1On\" class=\"button button__on\">ON</a>\n";
  }
  if(led2Stat){
  ptr += "<p class=\"sub-content\">LED 2 status: ON</p>\n";
  ptr += "<a href=\"/led2Off\" class=\"button button__off\">OFF</a>\n";
  }
  else{
  ptr += "<p class=\"sub-content\">LED 2 status: ON</p>\n";
  ptr += "<a href=\"/led2On\" class=\"button button__on\">ON</a>\n";
  }

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}