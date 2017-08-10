#include "ledsm.h"
#include "utils.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library
#include <ESP8266WebServer.h>

#define STATE_MAIN_INIT                  (0)
#define STATE_MAIN_START_CAPTIVE_PORTAL  (1)
#define STATE_MAIN_INIT_HTTP             (2)
#define STATE_MAIN_SERVE_HTTP            (3)
#define STATE_MAIN_RESET                 (50)


const int Relay1=16;
const int Relay2=17;

void config_wifi()
{
  WiFiManager wifiManager;
  wifiManager.startConfigPortal("Welcome to iDONT");
  delay(4000);
}


// We can't do this global-statically or it will conflict with the WiFiManager's instantiation
// We can't do this local-statically because we need the object to survive longer than a single function call on the stack
void instantiate_wifi_server(WiFiServer** ppServer)
{
  (*ppServer) = new WiFiServer(80);
}

void serve_wifi_client(WiFiServer* pServer)
{
  WiFiClient client = pServer->available();
  if (client) {
    Serial.println("New client");
  }
}

#define pServer (*ppServer)
void main_state_machine(unsigned char* state, unsigned char* ledState, WiFiServer** ppServer)
{  
  switch(*state)
  {
    case STATE_MAIN_INIT:
    
      if (WiFi.SSID()=="")
      {
        *ledState = STATE_LED_INIT_OFF;
        *state = STATE_MAIN_START_CAPTIVE_PORTAL;
      }
      else
      {
        Serial.print("SSID:       ");
        Serial.println(WiFi.SSID());
        WiFi.waitForConnectResult();
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        if (WiFi.status()!=WL_CONNECTED){Serial.println("Not connected.");}
        *ledState = STATE_LED_INIT_BLINKING;
        *state = STATE_MAIN_INIT_HTTP;
      }
    break;
    case STATE_MAIN_START_CAPTIVE_PORTAL:
      config_wifi();
      *state = STATE_MAIN_RESET;
    break;
    case STATE_MAIN_RESET:
      board_reset();
    break;
    case STATE_MAIN_INIT_HTTP:
      instantiate_wifi_server(ppServer);
      (*ppServer)->begin();
      *state = STATE_MAIN_SERVE_HTTP;
    break;
    case STATE_MAIN_SERVE_HTTP:
      serve_wifi_client((*ppServer));
    break;
  }

}

