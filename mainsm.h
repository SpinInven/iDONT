#include "ledsm.h"
#include "utils.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library

#define STATE_MAIN_INIT                  (0)
#define STATE_MAIN_START_CAPTIVE_PORTAL  (1)
#define STATE_MAIN_WAIT                  (2)
#define STATE_MAIN_RESET                 (3)


const int Relay1=16;
const int Relay2=17;

void config()
{
  
}

void main_state_machine(unsigned char* state, unsigned char* ledState, WiFiManager* wifiManager)
{
  switch(*state)
  {
    case STATE_MAIN_INIT:
      if (WiFi.SSID()=="")
      {
        *state = STATE_MAIN_START_CAPTIVE_PORTAL;
      }
      else
      {
        *state = STATE_MAIN_WAIT;
      }
    break;
    case STATE_MAIN_START_CAPTIVE_PORTAL:
      wifiManager->startConfigPortal("Welcome to iDONT");
      *state = STATE_MAIN_RESET;
    break;
    case STATE_MAIN_RESET:
      board_reset();
    break;
    case STATE_MAIN_WAIT:
      *ledState = STATE_LED_INIT_BLINKING;
    break;
  }
  
}

