#include "mainsm.h"
#include "ledsm.h"
#include "nvram.h"
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager WiFi 
#include <ESP8266WebServer.h>


WiFiServer* pWifiServer;
unsigned char mainState;
unsigned char led1State;
unsigned char led2State;
unsigned char doorbellEnabled;


void setup() {  
  // put your setup code here, to run once:
  initialize_hal();
  nvram_init();
  Serial.begin(115200);

  mainState = STATE_MAIN_INIT;
  led1State= STATE_LED_INIT_BLINKING;
  led2State= STATE_LED_INIT_BLINKING;
  doorbellEnabled = STATE_ENABLED;
  
  enable_bell();
  delay(200);
  disable_bell();
  delay(100);
  enable_bell();
  delay(200);
  disable_bell();
  delay(2000);
  nvram_dump();
}

void loop() 
{
  if (doorbellEnabled) enable_bell(); else disable_bell();
  led_state_machine(&led1State, MAIN_LED);
  led_state_machine(&led2State, SECOND_LED);
  main_state_machine(&mainState, &led1State, &led2State, &doorbellEnabled, &pWifiServer);
  yield();
}
