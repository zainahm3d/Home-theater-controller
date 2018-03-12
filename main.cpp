/*
Code for an ESP8266 to control a home tv system.
all infrared hex codes are sent through an IR LED
on pin 4.

Hex codes found by pointing remote at an arduino
and pressing a button. With lots of hope.

Each letter sent from the web app corresponds
to a function on the ESP8266 end

Devices:
Vizio tv
Sony surround sound
*/

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

IRsend irsend(4);

const char *ssid = "ssid";
const char *password = "pwd";

// fire up a wifi server on port 80
WiFiServer server(80);

void setup() {

  irsend.begin();

  delay(500);
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Full System Power
  if (request.indexOf("/a") != -1) {
    Serial.println("System Power Sent");
    irsend.sendSony(0x540A, 15, 3);
    irsend.sendNEC(0x20DF10EF);
  }
  // Blu Ray Power
  if (request.indexOf("/b") != -1) {
    Serial.println("Blu Ray Power");
    irsend.sendSony(0x540A, 15, 3);
  }
  // TV Power
  if (request.indexOf("/c") != -1) {
    Serial.println("TV Power");
    irsend.sendNEC(0x20DF10EF);
  }
  // Blu Ray Commands
  if (request.indexOf("/d") != -1) {
    Serial.println("BR Vol+");
    irsend.sendSony(0x240A, 15, 1);
  }
  if (request.indexOf("/e") != -1) {
    Serial.println("BR Vol-");
    irsend.sendSony(0x640A, 15, 1);
  }
  if (request.indexOf("/f") != -1) {
    Serial.println("BR Bluetooth");
    irsend.sendSony(0x8E10C, 20, 1);
  }
  if (request.indexOf("/g") != -1) {
    Serial.println("BR Play");
    irsend.sendSony(0x200B, 15, 1);
  }
  if (request.indexOf("/h") != -1) {
    Serial.println("BR Pause");
    irsend.sendSony(0x400b, 15, 1);
  }
  if (request.indexOf("/i") != -1) {
    Serial.println("BR forward");
    irsend.sendSony(0x2C108, 15, 1);
  }
  if (request.indexOf("/j") != -1) {
    Serial.println("BR backward");
    irsend.sendSony(0xCC108, 15, 1);
  }
  if (request.indexOf("/n") != -1) {
    Serial.println("BR Function");
    irsend.sendSony(0x4B0B, 15, 1);
  }
  // TV Commands

  if (request.indexOf("/k") != -1) {
    Serial.println("TV HDMI");
    irsend.sendNEC(0x20DF639C);
  }
  if (request.indexOf("/l") != -1) {
    Serial.println("TV Input");
    irsend.sendNEC(0x20DFF40B);
  }
  if (request.indexOf("/m") != -1) {
    Serial.println("TV OK");
    irsend.sendNEC(0x20DF22DD);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
}
