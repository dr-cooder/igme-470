#include "arduino_secrets.h"
/*
   WifiRITServer

   Example of using an Arduino wifi to trade sensor readings with another Arduino
   Adaapted by W Michelle Harris, April 2022
   reference: https://www.arduino.cc/reference/en/libraries/wifinina/
*/

#include <SPI.h>
#include <WiFiNINA.h>

/*
   please enter your sensitive data in the arduino_secrets.h file
*/
char ssid[] = "RIT-WiFi";       // Name of WPA or WPA2 enterprise network
char user[] = SECRET_NAME;        // WPA2 enterprise username
const char pass[] = SECRET_PASS;  // WPA2 enterprise user password or WPA network password
const unsigned int keyIndex = 0;  // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

int wifiPort = 2390;
WiFiServer server(wifiPort);         // create a server on port wifiPort

const unsigned int ledPin = 13; 
const unsigned int sensorPin = 0;    // analog pin 0

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  // check the status of the wifi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println(F("Communication with WiFi module failed!"));
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println(F("Please upgrade the firmware"));
  }

  // attempt to connect to the wifi network
  while (status != WL_CONNECTED) {
    Serial.print(F("Attempting to connect to Network named: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network.
    // If using a WEP network use WiFi.begin(ssid, keyIndex, key);
    // If using an open network use WiFi.begin(ssid);
    //status = WiFi.begin(ssid, pass);
    status = WiFi.beginEnterprise(ssid, user, pass);

    // wait 10 seconds for connection
    delay(10000);
  }

  Serial.println("Connected to wifi");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID()); // the SSID of the network you're connected to
  IPAddress ip = WiFi.localIP(); // this board's IP address
  Serial.print("My IP Address: ");  // Share this with your networking partner!
  Serial.print(ip);               //double check in browser as http://ip
  Serial.print(":");
  Serial.print(wifiPort);
  // start the server
  Serial.println(F("\nStarting server..."));
  server.begin();

}

void loop() {
  // wait for a new client
  WiFiClient client = server.available();

  if (client) {
    digitalWrite(ledPin, HIGH);
    // if there are incoming bytes available
    // from the client, read and print them:
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    } //you should Do Something with it, tho

     // read the analog sensor value and send to client
    int outVal = analogRead(sensorPin);
    
    Serial.print(" received. Send ");
    Serial.print(outVal);
    Serial.print(" back to ");
    Serial.println(client.remoteIP());
    
    server.println(outVal);
    
    digitalWrite(ledPin, LOW);
    delay(5); // slight delay to keep things stable
  }


}