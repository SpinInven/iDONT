#include "mainsm.h"
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi 

#define ESP8266_LED (5)

WiFiManager wifiManager;
unsigned char mainState;
void setup() {
  // put your setup code here, to run once:
 
  mainState = STATE_MAIN_INIT;
  pinMode(ESP8266_LED, OUTPUT);
}

void loop() 
{
  //main_state_machine(&mainState, &wifiManager);

  digitalWrite(ESP8266_LED, HIGH);
  delay(500);
  digitalWrite(ESP8266_LED, LOW);
  delay(500);
}
