#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define SSID "HOANG DAT"
#define PASS "24072002"
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER "hivemq.webclient.1686017745826"
#define MQTT_PASS "R57H>qCjb<a2u.3BGe#W"
#define MQTT_LDP_TOPIC "DatHoang"
#define MQTT_CLIENT_ID "Dat_Hoang123"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setUp_Wifi(){
  Serial.print("Connecting to...");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("Wifi connected to ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connect_to_broker(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    if(client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS)){
      Serial.println("Connected");
      client.subscribe(MQTT_LDP_TOPIC);
    }
    else{
      Serial.print("failed, rc =");
      Serial.print(client.state());
      Serial.println("try again in 2 sec");
      delay(2000);
    }
  }
}

void callBack(char*topic, byte *payload, unsigned int length){
  Serial.println("new message");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: ");
  Serial.write(payload, length);
  Serial.println(); 
}

void send_data(){
  client.publish(MQTT_LDP_TOPIC, "123");
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  setUp_Wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callBack);
  connect_to_broker();
}

void loop() {
  client.loop();
  if(!client.connected()){
    connect_to_broker();
  }
  send_data();
}
