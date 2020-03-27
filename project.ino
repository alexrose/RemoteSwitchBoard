#include <ESP8266WiFi.h>

const int ON = LOW;
const int OFF = HIGH;
const int SwitchOne = D1;
const int SwitchTwo = D2;

const char* ssid = "";
const char* password = "";

// Set server port
WiFiServer server(80);

void setup() {
  // initialize serial communications at 115200 bps
  Serial.begin(115200);
  delay(10);

  // connect to wifi
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  
  }
  Serial.println("");
  Serial.println("WiFi connected successfully.");

  // Start the server then show connect info
  server.begin();
  Serial.println("Server started.");
  Serial.print("Server URL is: http://");
  Serial.print(WiFi.localIP());
  Serial.println("");
  
  // configure first switch
  pinMode(SwitchOne, OUTPUT);
  digitalWrite(SwitchOne, OFF);

  // configure second switch
  pinMode(SwitchTwo, OUTPUT);
  digitalWrite(SwitchTwo, OFF);
}

void loop() {
  // Search for connected clients
  WiFiClient client = server.available();
  
  if (!client) {
    return;  
  }

  // Wait for clients
  Serial.println("New client detected.");
  while(!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();

  // Match the request
  if (request.indexOf("/switchOneOn") > 0)  {
    digitalWrite(SwitchOne, ON);
    Serial.println("SwitchOne is on.");
  } else if (request.indexOf("/switchOneOff") > 0)  {
    digitalWrite(SwitchOne, OFF);
    Serial.println("SwitchOne is off.");
  } else if (request.indexOf("/switchTwoOn") > 0)  {
    digitalWrite(SwitchTwo, ON);
    Serial.println("SwitchTwo is on.");
  } else if (request.indexOf("/switchTwoOff") > 0)  {
    digitalWrite(SwitchTwo, OFF);
    Serial.println("SwitchTwo is off.");
  } else if (request.indexOf("/switchStatus") > 0)  {
    Serial.println("Get switch status.");
  } else {
    client.stop();
    return;
  }

  // Return data for http server
  client.flush();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("");
  
  client.println("{");
  client.print("'SwitchOne' : ");
  if (digitalRead(SwitchOne)) {
    client.println("'off',");
  } else {
    client.println("'on',");
  }
  
  client.print("'SwitchTwo' : ");
  if (digitalRead(SwitchTwo)) {
    client.println("'off'");
  } else {
    client.println("'on'");
  }

  client.println("}");
  client.println("");
  delay(1);
}
