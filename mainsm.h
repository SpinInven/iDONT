#include "ledsm.h"
#include <WiFiManager.h>

#define STATE_MAIN_INIT                  (0)
#define STATE_MAIN_START_CAPTIVE_PORTAL  (1)
#define STATE_MAIN_WAIT  (2)


const int Relay1=16;
const int Relay2=17;

void main_state_machine(unsigned char* state, unsigned char* ledState, WiFiManager* wifiManager)
{
  switch(*state)
  {
    case STATE_MAIN_INIT:
      *state = STATE_MAIN_START_CAPTIVE_PORTAL;
    break;
    case STATE_MAIN_START_CAPTIVE_PORTAL:
      wifiManager->startConfigPortal("Welcome to iDONT");
      *state = STATE_MAIN_WAIT;
    break;
    case STATE_MAIN_WAIT:
    break;
  }
  
}

