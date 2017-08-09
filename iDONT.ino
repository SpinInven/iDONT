#include "mainsm.h"
#include "ledsm.h"
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi 


WiFiManager wifiManager;
unsigned char mainState;
unsigned char ledState;
void setup() {
  // put your setup code here, to run once:
 
  mainState = STATE_MAIN_INIT;
  ledState= STATE_LED_BLINKING_ON;
  led_sm_setup();
  pinMode(ESP8266_LED, OUTPUT);
}

void loop() 
{
  //main_state_machine(&mainState, &wifiManager);
  led_state_machine(&ledState);
  yield();
}
