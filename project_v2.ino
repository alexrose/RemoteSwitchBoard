#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

const int ON = LOW;
const int OFF = HIGH;
const int SwitchOne = D1;
const int SwitchTwo = D2;

const char* ssid = "WeMosAP";
const char* password = "WeMosPass";

// Set server port
ESP8266WebServer server(80);

void setup() {
  // initialize serial communications at 115200 bps
  Serial.begin(115200);

  // connect to wifi
  WiFiManager wifiManager;
  wifiManager.autoConnect();
 
  // configure first switch
  pinMode(SwitchOne, OUTPUT);
  digitalWrite(SwitchOne, OFF);

  // configure second switch
  pinMode(SwitchTwo, OUTPUT);
  digitalWrite(SwitchTwo, OFF);

  // Server
  server.on("/", handleRoot);
  server.on("/switchStatus", handleRoot);
  server.on("/switchOne", handleSwitchOne);
  server.on("/switchTwo", handleSwitchTwo);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  getSwitchStatus();
 
}

void handleSwitchOne() {
   for (uint8_t i=0; i < server.args(); i++){
    if (server.argName(i) == "status" && server.arg(i) == "on") {
       digitalWrite(SwitchOne, ON);
       break;
    } else if (server.argName(i) == "status" && server.arg(i) == "off") {
       digitalWrite(SwitchOne, OFF);
       break;
    }
  }
  getSwitchStatus();
}

void handleSwitchTwo() {
   for (uint8_t i=0; i < server.args(); i++){
    if (server.argName(i) == "status" && server.arg(i) == "on") {
       digitalWrite(SwitchTwo, ON);
       break;
    } else if (server.argName(i) == "status" && server.arg(i) == "off") {
       digitalWrite(SwitchTwo, OFF);
       break;
    }
  }
  getSwitchStatus();
}


void handleNotFound() {
  String message = "{";
  message += "'error' : 'true',";
  message += "'message' : 'Invalid request.'";
  message += "}";
  
  server.send(200, "application/json", message);
}

void getSwitchStatus() {
  String message = "{";
  message += "'error' : 'false',";
  
  if (digitalRead(SwitchOne)) {
    message += "'SwitchOne' : 'off',";
  } else {
    message += "'SwitchOne' : 'on',";
  }

  if (digitalRead(SwitchTwo)) {
    message += "'SwitchTwo' : 'off'";
  } else {
    message += "'SwitchTwo' : 'on'";
  }

  message += "}";

  server.send(200, "application/json", message);
}

void loop() {
  server.handleClient();
}
