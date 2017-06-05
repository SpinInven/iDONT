//uses this website as a start: https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
#include <WiFi.h>

const char* ssid ="YourSSID";
const char* password = "YourPassword";

WiFiServer server(80);
const int Relay1=16;
const int Relay2=17;
// Client variables
char linebuf[80];
int charcount=0;

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
Serial.println("Connected to the WiFi network");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop() {
// listen for incoming clients
WiFiClient client = server.available();
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
          digitalWrite(Relay1,HIGH);
        }
        else if (strstr(linebuf,"GET /off1") > 0){
          Serial.println("Doorbell OFF");
          DoorbellOn= false;
          digitalWrite(Relay1, LOW);
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
