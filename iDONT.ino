#include "mainsm.h"
#include "ledsm.h"
#include "nvram.h"
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi 
#include <ESP8266WebServer.h>


WiFiServer* pWifiServer;
unsigned char mainState;
unsigned char ledState;

void setup() {  
  // put your setup code here, to run once:
  initialize_hal();
  nvram_init();
  Serial.begin(115200);

  mainState = STATE_MAIN_INIT;
  ledState= STATE_LED_INIT_OFF;
  pinMode(MAIN_LED, OUTPUT);
  
  enable_bell();
  delay(1000);
  disable_bell();
  delay(1000);
  enable_bell();
  delay(1000);
  disable_bell();
  delay(1000);
  nvram_dump();
}

void loop() 
{
  led_state_machine(&ledState);
  main_state_machine(&mainState, &ledState, &pWifiServer);
  yield();
}
