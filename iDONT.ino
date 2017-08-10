#include "mainsm.h"
#include "ledsm.h"
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi 
#include <ESP8266WebServer.h>

//WiFiServer server(80);
unsigned char mainState;
unsigned char ledState;
void setup() {
  // put your setup code here, to run once:
 
  mainState = STATE_MAIN_INIT;
  ledState= STATE_LED_INIT_OFF;
  pinMode(ESP8266_LED, OUTPUT);
}

void loop() 
{
  led_state_machine(&ledState);
  main_state_machine(&mainState, &ledState);
  yield();
}
