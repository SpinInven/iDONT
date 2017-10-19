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
  char linebuf[80];
  unsigned int charcount=0;

  if (client) {
  Serial.println("New client");
  memset(linebuf,0,sizeof(linebuf));
  charcount=0;
  // an http request ends with a blank line
  boolean currentLineIsBlank=true;
  boolean DoorbellOn=true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      //read char by char HTTP request
      linebuf[charcount]=c;
      if (charcount<sizeof(linebuf)-1) charcount++;
      //if you have gotten to the end of the line (received a
      //newline character) & the line is blank, the http request
      //has ended, so you can send a reply
      if (c == '\n' && currentLineIsBlank) {
        //send a standard http response header
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close"); //the connection 
        //will be closed after completion of the response
        client.println();
        client.println("<!DOCTYPE HTML><html><head>");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
        client.println("<h1>iDONT - internet Doorbell ON/off Trigger</h1>");
        client.println("<p>Doorbell <a href=\"on1\"><button>ON</button></a>&nbsp;<a href=\"off1\"><button>OFF</button></a></p>");
        if (DoorbellOn ==true) {client.println("<h2>Doorbell is currently on.</h2>");}
        else {client.println("<h2>Doorbell is currently off.</h2>");}
        //client.println("<p>LED #2 <a href=\"on2\"><button>ON</button></a>&nbsp;<a href=\"off2\"><button>OFF</button></a></p>");
        client.println("</html>");
        break;    
      }
      if (c == '\n') {
        //you are starting a new line
        currentLineIsBlank = true;
        if (strstr(linebuf,"GET /on1") > 0){
          Serial.println("Doorbell ON");
          DoorbellOn= true;
          ////digitalWrite(Relay1,HIGH);
        }
        else if (strstr(linebuf,"GET /off1") > 0){
          Serial.println("Doorbell OFF");
          DoorbellOn= false;
          ////digitalWrite(Relay1, LOW);
        }
//        else if (strstr(linebuf,"GET /on2") > 0){
//          Serial.println("LED 2 ON");
//          digitalWrite(Relay2, HIGH);
//        }
//        else if (strstr(linebuf,"GET /off2") > 0){
//          Serial.println("LED 2 OFF");
//          digitalWrite(Relay2, LOW);
//        }
        // you're starting a new line
        currentLineIsBlank = true;
        memset(linebuf,0,sizeof(linebuf));
        charcount=0;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
        currentLineIsBlank = false;
    
        }
    }
  }
  //give the web browser time to receiver the data
  delay(1);

  //close the connection
  client.stop();
  Serial.println("client disconnected");
  
  }
}

#define pServer (*ppServer)
void main_state_machine(unsigned char* state, unsigned char* ledState, unsigned char* led2State, WiFiServer** ppServer)
{  
  switch(*state)
  {
    case STATE_MAIN_INIT:
    
      if (WiFi.SSID()=="")
      {
        *ledState = STATE_LED_INIT_OFF;
        *led2State = STATE_LED_INIT_BLINKING;
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

