#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "motordriver.h"
#include"picture.h"
#include <Wire.h> 
//OLED define
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#define Logo_width 128
#define Logo_height 64
SSD1306Wire  display(0x3c, 21, 22);

int hungryData=100;
int moodData=100;
int timer=0;
int sensorValue;
int pressValue;
int lightInit=0;
int eatpress_state=0;
bool eatpress_over=true;
bool timer_state=false;
unsigned long starttime;
unsigned long stoptime=0;
unsigned long looptime;

//WIFI define
const char* ssid = "iPhone";
const char* password = "maggie9907";

char webSite[3000];
WebServer server(80);
void handleRoot() {
  snprintf(webSite,3000,"<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Remote Control LED</title> <script src=\"https://code.jquery.com/jquery-3.3.1.js\"></script> <script>setInterval(requestData, 500); function requestData(){$.get(\"/handleSensorData\").done(function(data){if(data){$(\"#starv_data\").text(data.vr);$(\"#mood_data\").text(data.moodvr);}else{$(\"#starv_data\").text(\"?\");$(\"#mood_data\").text(\"?\");}}).fail(function(){console.log(\"fail\");});}</script> <style>.mood, .hungry{display: flex; justify-content: center;}html{background-color: #bce6ff; font-family: Arial, Helvetica, sans-serif;}.title{text-align: center; color: #d33d3d; margin-bottom: 50px;}a{position: relative; text-decoration: none; background-color: #FFFFFF; border-radius: 4px; height: 100px; width: 100px; text-align: center; margin: 0; top: 50%; font-size: 2em; /*outline: solid red 2px;*/}.btn{color: #5e5e5e;}.toprow{display: flex; justify-content: center; /* outline: solid red 2px;*/}.secrow{justify-content: center; display: flex; /* outline: solid red 2px;*/}.speedchoose{/* outline: solid red 2px;*/ display: flex; flex-direction: column; text-align: center;}.submit{/*outline: solid red 2px;*/ text-align: center; width: 100px; margin-bottom: 30px;}</style></head><body> <h1 class=\"title\">Block controler</h1> <div class=\"petstate\"> <h2 class=\"hungry\">Starvation Condition=<span id=\"starv_data\"></span>%</h2> <h2 class=\"mood\">Mood Condition=<span id=\"mood_data\"></span>%</h2> </div><div class=\"toprow\"> <a class=\"btn\" href=\"/T\"> <p>top</p></a> </div><div class=\"secrow\"> <a class=\"btn\" href=\"/L\"> <p>left</p></a> <a class=\"btn\" href=\"/D\"> <p>down</p></a> <a class=\"btn\" href=\"/R\"> <p>right</p></a> </div><div> <a class=\"btn\" href=\"/S\"> <p>stop</p></a> </div></body></html>");
  server.send(200, "text/html",webSite);

}
void handleSensorData(){
  int sensor=analogRead(34);
  String json="{\"vr\": ";
  json+= hungryData;
  json+=", \"moodvr\": ";
  json+= moodData;
  json+="}";
  Serial.println(sensor);
  server.send(200,"appliction/json",json);
  
  }

/*void handleHungerData(){
  //hungryData-=timer;
  
  String json="{\"vr\": ";
  json+=hungryData;
  json+="}";
  Serial.println(hungryData);
  server.send(200,"appliction/json",json);
  
  }*/
  




void handleNotFound() {
 
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

//motor control

void setup(void) {
  pinMode(Motor_A_A_Pin, OUTPUT);
  pinMode(Motor_A_B_Pin, OUTPUT);
  pinMode(Motor_B_A_Pin, OUTPUT);
  pinMode(Motor_B_B_Pin, OUTPUT);
  
  ledcSetup(0,5000,8);
  ledcAttachPin(23,0);
 
 
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  /*
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }*/

  
  server.on("/", handleRoot);
  server.on("/handleSensorData",handleSensorData);
  server.on("/T", top);
  server.on("/L", left);
  server.on("/D", down);
  server.on("/R", right);
  server.on("/S", motor_stop);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  //OLED define
   display.init();
   display.setContrast(255); //數值介於0~255，調整對比
   display.clear();//清除螢幕和緩衝區(clear)
   
   
}




void Wakeup(){
   server.handleClient();
   server.send(200, "text/html",webSite);
    display.drawXbm(0,0, 128, 64, face[14]);
    display.display();
    delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[15]);
    display.display();
    delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[16]);
    display.display();
     delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[15]);
    display.display();
     delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[14]);
    display.display();
     delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[7]);
    display.display();
    delay(200);
} //開機醒來


void Happy1(){
    for (int i=0; i<=3; i++) {
       server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[17]);
        display.display();
        delay(300);
         server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[18]);
        display.display();
        delay(300);
         server.send(200, "text/html",webSite);
    }
}//新增的笑臉

void Angry(){
    for (int i=0; i<=3; i++) {
       server.handleClient();
       server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[0]);
        display.display();
        delay(300);
        server.handleClient();
        server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[1]);
        display.display();
        delay(300);
        server.handleClient();
        server.send(200, "text/html",webSite);
    }
}//生氣表情

void Blink(){
   server.handleClient();
   server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[7]);
    display.display();
    delay(300);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[2]);
    display.display();
    delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[3]);
    display.display();
    delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[2]);
    display.display();
    delay(200);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[7]);
    display.display();
}//待機札眨眨眼

void Happy(){
   
    for (int i=0; i<=3; i++) {
     
      server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[4]);
        display.display();
        delay(300);
       
        server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[5]);
        display.display();
        delay(300);
       
        server.send(200, "text/html",webSite);
    }
}//餵食後開心的表情＆撫摸後開心

void Unhappy(){
  
   server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[9]);
    display.display();
    delay(300);
    
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[10]);
    display.display();
    delay(300);
   
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[11]);
    display.display();
    delay(300);
   
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[10]);
    display.display();
    delay(300);
   
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[9]);
    display.display();
     //壓力感測
       sensorValue = analogRead(39);
       pressValue = analogRead(34);
        
        if(pressValue>1000){
            moodData+=5;
             Happy1();
               if(moodData>=90){
                eatpress_over=true;
                timer=0;
                 
                }
              
            }
         if(sensorValue>500){
           
             Angry();   
           
             
            }
            
     
}//不開心（效果：不爽眼神從左到右來回1次）


 
void Normal(){
   server.handleClient();
   server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[7]);
    display.display();
}//正常情況表情

void Left(){
    server.handleClient();
    server.send(200, "text/html",webSite);
    server.send(200, "text/html",webSite);
    Normal();
    delay(500);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[8]);
    display.display();
    delay(500);
    server.handleClient();
    server.send(200, "text/html",webSite);
}//左轉

void Right(){
    server.handleClient();
    server.send(200, "text/html",webSite);
    Normal();
    delay(500);
    server.handleClient();
    server.send(200, "text/html",webSite);
    display.clear();
    display.drawXbm(0,0, 128, 64, face[6]);
    display.display();
    delay(500);
    server.handleClient();
    server.send(200, "text/html",webSite);
}//右轉

void Hungry(){
 
   server.send(200, "text/html",webSite);
    for (int i=0; i<=5; i++) {
        display.clear();
        display.drawXbm(0,0, 128, 64, face[12]);
        display.display();
        delay(300);
       
        server.send(200, "text/html",webSite);
        display.clear();
        display.drawXbm(0,0, 128, 64, face[13]);
        display.display();
        delay(300);
        
        server.send(200, "text/html",webSite);

        //感測光敏電阻
        sensorValue = analogRead(39);
        pressValue = analogRead(34);
        if(sensorValue>500){
              
              
              hungryData+=5;
              server.send(200, "text/html",webSite);
              Happy();
             
              if(hungryData>=90){
                eatpress_over=true;
                timer=0;
                 break;
                }
             
            }
        if(pressValue>1000){
           Angry();   
            }
      
        
    }
 } //肚子餓不開心


void loop(void) {
  

 display.clear();//清除螢幕和緩衝區(clear) 
 server.handleClient();
 server.send(200, "text/html",webSite);

  //start
 
  for(int i=0;i<=1;i++){
    Wakeup();
      }
     
 //read mode
    while(1){
        server.handleClient();
        server.send(200, "text/html",webSite);
        Blink();
        if(state_move==true&&eatpress_over==true){
              server.handleClient();
              server.send(200, "text/html",webSite);
              if(way==right_state){
                server.handleClient();
                server.send(200, "text/html",webSite);
                Right();
                }
               else if(way==left_state){
                server.handleClient();
                server.send(200, "text/html",webSite);
                 Left();
                }
                else if(way==top_state||way==down_state||way==motor_stop_state){
                  server.handleClient();
                  server.send(200, "text/html",webSite);
                  Blink();
                }
          server.handleClient();
          server.send(200, "text/html",webSite);     
         delay(1000);
         hungryData-=2;
         moodData-=2;
          server.handleClient();
          server.send(200, "text/html",webSite);
         timer++;
               
         //Serial.print(timer);
         //Serial.print(" \n"); 
       }
      if(timer>10){
       // Serial.print("stop");
        state_move=false;
        timer_state=true;
        //motor A
        digitalWrite(Motor_A_A_Pin, HIGH);
        digitalWrite(Motor_A_B_Pin, HIGH);
        //motor B
        digitalWrite(Motor_B_A_Pin, HIGH);
        digitalWrite(Motor_B_B_Pin, HIGH);
          eatpress_state= 2/*random(1, 3)*/;
         
          eatpress_over=false;   
          while(eatpress_over==false){
           switch(eatpress_state){
            case 1 :
                //eat+animation
                Hungry();
            break;
            
            case 2 :
                //press+animation
               Unhappy();
            break;         
          }
        }
      }
       
    }
}
  
