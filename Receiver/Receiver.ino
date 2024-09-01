#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

char ssid[]="xxxxxxxxxxxxx";
char password[]="xxxxxxxxxxxx";
String relay = "http://blynk.cloud/external/api/get?token=xxxxxxxxxxxxxxxxxxxxxxxxxxxxx&dataStreamId=x"; 
const int relay1pin = 15;
const int relay2pin = 0;
int relay1status;
int relay2status;
String payload;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(100);
  Serial.print("connecting to wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);//connect your wifi to hotspot
  while(WiFi.status() !=WL_CONNECTED)
  {
    Serial.print(".");
  }
  delay(500);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  
  pinMode(relay1pin,OUTPUT);
  pinMode(relay2pin,OUTPUT);
}
void loop()
{   WiFiClient client;
HTTPClient http;

    http.begin(client,relay.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
 http.end();

String relay1string = payload.substring(0,1);
Serial.println(relay1string);
String relay2string = payload.substring(1,2);
Serial.println(relay2string);
relay1status = relay1string.toInt();
relay2status = relay2string.toInt();
 
if (relay1status == 1)
{
digitalWrite(relay1pin, HIGH);  
}
else
{
  digitalWrite(relay1pin, LOW); 
}
if (relay2status == 1)
{
digitalWrite(relay2pin, HIGH);  
}
else
{
  digitalWrite(relay2pin, LOW);   
}
delay(2000);
}
