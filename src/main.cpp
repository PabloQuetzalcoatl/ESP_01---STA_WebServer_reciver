#include <Arduino.h>


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "WIFI"
#define STAPSK  "pass"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);



void handleRoot() {

if( server.hasArg("State") )
  {
       Serial.print("State: ");
       Serial.println(server.arg("State"));
  }
if( server.hasArg("X") )
  {
       Serial.print("X: ");
       Serial.println(server.arg("X"));
  }
if( server.hasArg("Y") )
  {
       Serial.print("Y: ");
       Serial.println(server.arg("Y"));
  }
  
  server.send ( 200, "text/html", "" );
}
 


void handleNotFound() {
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}



void setup(void) {
 
  //digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}