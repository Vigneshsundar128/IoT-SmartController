#define BLYNK_TEMPLATE_ID           "xxxxxxxxxx"
#define BLYNK_DEVICE_NAME           "xxxxxxxxxxxxx"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int button = 16;
int buttonstate = 0;
const int led= 12;
String buttonstatus;

int relay1 = 15;
int switchstate = 0;
int oldswitchstate = 0;
int relay1on =0;
String relay1status;
String relay1statusdisp;

int relay2 = 0;
int switchstate1 = 0;
int oldswitchstate1 = 0;
int relay2on =0;
String relay2status;
String relay2statusdisp;

void setup()
{
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  pinMode(button,INPUT);
  pinMode(relay1,INPUT);
  pinMode(relay2,INPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  Serial.println(F("SSD1306 allocation failed"));
  for(;;);
  }
  delay(100);
 buttonstatus = "Not Pressed";
 BlynkEdgent.begin();
}

void loop() {
  buttonstatus = "Not Pressed";
  buttonstate=digitalRead(button);
  switchstate=digitalRead(relay1);
  switchstate1=digitalRead(relay2);

  if(buttonstate == 1)
  {
    buttonstatus = "Pressed";
    String server = "http://maker.ifttt.com/trigger/emergency/with/key/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    WiFiClient client;
    HTTPClient http;
    http.begin(client, server.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
 http.end();
 digitalWrite(led,HIGH);
 delay(3000);
 digitalWrite(led,LOW);
  }

 if (switchstate != oldswitchstate)
 {
 oldswitchstate = switchstate;
 if (switchstate == HIGH)
 {
 relay1on = !relay1on;
 }
 }
 if(relay1on)
   {
    Serial.println("relay1 on");
    relay1status = "1";
    relay1statusdisp = "On";
}  
   else if("relay1 off")
   {
    Serial.println("relay1 off");
    relay1status = "0";
    relay1statusdisp = "Off";
   }

if (switchstate1 != oldswitchstate1)
 {
 oldswitchstate1 = switchstate1;
 if (switchstate1 == HIGH)
 {
 relay2on = !relay2on;
 }
 }
   if(relay2on)
   {
    Serial.println("relay2 on");
    relay2status = "1";
    
    relay2statusdisp = "On";
   }  
   else if("relay2 off")
  {
    Serial.println("relay2 off");
    relay2status = "0";
    relay2statusdisp = "Off";
  }
String Add = relay1status+relay2status;
//int Final = Add.toInt()
Blynk.virtualWrite(V0,Add);

display.clearDisplay();  
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 10);
display.println("Dash: "+String(buttonstatus));
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 20);
display.println("Relay 1: "+String(relay1statusdisp));
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 30);
display.println("Relay 2: "+String(relay2statusdisp));
display.display();
  BlynkEdgent.run();
  delay(1000);
}
