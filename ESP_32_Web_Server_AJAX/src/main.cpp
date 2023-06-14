#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
WebServer webServer(80);

#define D1 5
#define D2 18

char* ssid = "HOANG DAT";
char* password = "24072002";

//bien chua ma HTML website
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Digital Control</title>
    <style>
        body{
            text-align: center;
        }
        h1{
            color: #fc068d;
        }
        a{
            text-decoration: none;
            color: #b3e464;
        }
        .bt_on{
            height: 50px;
            width: 100px;
            margin: 15px 0;
            background-color: #df4d4d;
            border-radius: 20px;
        }
        .bt_off{
            height: 50px;
            width: 100px;
            margin: 15px 0;
            background-color: #20db7e;
            border-radius: 20px;
        }
    </style>
</head>
<body>
    <h1>ESP32 Web Server - AJAX</h1>
    <div>D1Status: <b><pan id = "trangthaiD1"><pan></b></div>
    <div>
        <button class = "bt_on" onclick = "getdata('onD1')">ON</button>
        <button class = "bt_off" onclick = "getdata('offD1')">OFF</button>
    </div>
    <div>D2Status: <b><pan id = "trangthaiD2"><pan></b></div>
    <div>
        <button class = "bt_on" onclick = "getdata('onD2')">ON</button>
        <button class = "bt_off" onclick = "getdata('offD2')">OFF</button>
    </div>
    <div id ="reponsetext"></div>
    <script>
        //tao doi tuong request
        function create_obj(){
            td = navigator.appName;
            if(td == "Microsoft Internet Explorer"){
                obj = new ActiveXObject("Microsoft.XMLHTTP");
            }
            else{
                obj = new XMLHttpRequest();
            }
            return obj;
        }
        var xhttp = create_obj();
        // thiet lap gui du lieu va request
        function getdata(url){
            xhttp.open("GET","/"+ url, true);
            xhttp.onreadystatechange = process;//nhan response
            xhttp.send();
        }
        //kiem tra response
        function process(){
            if(xhttp.readyState == 4 && xhttp.status == 200){
                //Update data su dung javascript
                ketqua = xhttp.responseText;
                document.getElementById("reponsetext").innerHTML = ketqua;
            }
        }
        //kiem tra trang thai chan D1 D2
        function getstatusD1D2(){
            xhttp.open("GET","getstatusD1D2", true);
            xhttp.onreadystatechange = process_json;
            xhttp.send();
        }
        //kiem tra response
        function process_json(){
            if(xhttp.readyState == 4 && xhttp.status == 200){
                //update data
                var trangthaiD1D2_JSON = xhttp.responseText;
                var Obj = JSON.parse(trangthaiD1D2_JSON);
                document.getElementById("trangthaiD1").innerHTML = Obj.D1;
                document.getElementById("trangthaiD2").innerHTML = Obj.D2;
            }
        }
        //update du lieu tu dong
        setInterval(function(){
            getstatusD1D2();
        },1000);
    </script>
</body>
</html>
)=====";

void mainpage();
void on_D1();
void off_D1();
void on_D2();
void off_D2();
void get_statusD1D2();

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.println(WiFi.localIP());

  webServer.on("/", mainpage);
  webServer.on("/onD1", on_D1);
  webServer.on("/offD1", off_D1);
  webServer.on("/onD2", on_D2);
  webServer.on("/offD2", off_D2);
  webServer.on("/getstatusD1D2", get_statusD1D2);
  webServer.begin();
}

void loop() {
  webServer.handleClient();
}

void mainpage(){
  String s = FPSTR(MainPage);
  webServer.send(200, "text/html", s);
}

void on_D1(){
  digitalWrite(D1, LOW);
  webServer.send(200, "text/html", "Chân D1 ON");
}

void off_D1(){
  digitalWrite(D1, HIGH);
  webServer.send(200, "text/html", "Chân D1 OFF");
}

void on_D2(){
  digitalWrite(D2, LOW);
  webServer.send(200, "text/html", "Chân D2 ON");
}

void off_D2(){
  digitalWrite(D2, HIGH);
  webServer.send(200, "text/html", "Chân D2 OFF");
}

void get_statusD1D2(){
  String d1, d2;
  if(digitalRead(D1) == 1) 
  d1 = "OFF";
  else
  d1 = "ON";
  if(digitalRead(D2) == 1)
  d2 = "OFF";
  else
  d2 = "ON";
    String s = "{\"D1\": \"" + d1 + "\",\"D2\": \"" + d2 + "\"}";
    webServer.send(200, "application/json", s);
}