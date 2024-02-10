#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "server_handler.h"
#include "utils.h"

// Device will also be accessible via http://<hostname>.local
const String hostName = "esp8266";
const String ssid = "A-S-H-F-I-Y-A";

ServerHandler server;
std::list<Controller> controllers;

void onChange(Controller* cont) {
    Serial.println((*cont).name + " Changed to " + (*cont).getValue());
}

std::list<Controller>* setupController() {
    controllers.push_back(Controller("Pin1", 4, OFF, *onChange));  // GPIO4 D1
    controllers.push_back(Controller("Pin2", 5, OFF, *onChange));  // GPIO5 D2
    return &controllers;
}

void setup() {
    Serial.begin(9600);
    pinMode(INDICATOR_LED, OUTPUT);

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Connecting to SSID: " + ssid);
    WiFi.hostname(hostName);
    WiFi.begin(ssid);

    while (WiFi.status() != WL_CONNECTED) blinkIndicator();

    // Print local IP address and start web server
    digitalWrite(INDICATOR_LED, ON);
    Serial.println("Connected to SSID: " + ssid);
    Serial.println("IP address: " + WiFi.localIP().toString());
    if (MDNS.begin(hostName)) Serial.println("MDNS responder started");

    server.useController(setupController());
    server.begin();
    Serial.println("HTTP server started at http://" + hostName + ".local");
    digitalWrite(INDICATOR_LED, OFF);
}

void loop() {
    MDNS.update();
    server.handleClient();
}
