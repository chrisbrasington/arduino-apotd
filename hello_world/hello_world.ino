#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.11.50";
String dataLocation = "/test.txt HTTP/1.1";

EthernetClient client;
String currentLine = "";            // string for incoming serial data
String currRates = "";
boolean readingRates = false;       // is reading?
const int requestInterval = 900000; // ms delay between requests (15 min)

boolean requested;         // whether you've made a request since connecting
long lastAttemptTime = 0;  // last time you connected to the server, in ms

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
 
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore
  }
 
  // Connect to server 
  connectToServer();
}

void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(server, 80)) {
    Serial.println("making HTTP request...");
     
    // make HTTP GET request to dataLocation:
    client.println("GET " + dataLocation);
    client.println("Host: 192.168.11.50");
    client.println();
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}

void loop() {
  // Checking for new data
  if (client.connected()) {
    if (client.available()) {
      // read incoming bytes:
      char inChar = client.read();
    
      // add incoming byte to end of line:
      currentLine += inChar;
 
      // if you get a newline, clear the line:
      if (inChar == '\n') {
        currentLine = "";
      }
      else {
        Serial.println(currentLine);
      }

  }
 
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and requestInterval has passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
}
}



