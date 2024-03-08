#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "server_handler.h"
#include "utils.h"

const String ssid = "A-S-H-F-I-Y-A";
unsigned char bssid[18] = {0x80, 0xAF, 0xCA, 0x16, 0xF2, 0x88};

ServerHandler server;
std::list<Controller> controllers;

void onChange(Controller* cont) {
    Serial.println((*cont).name + " Changed to " + (*cont).getValue());
}

std::list<Controller>* setupController() {
    EEPROM.begin(sizeof(uint8_t) * 2);

    controllers.push_back(Controller(0, "Pin1", 4, OFF, *onChange));  // GPIO4 D1
    controllers.push_back(Controller(1, "Pin2", 5, OFF, *onChange));  // GPIO5 D2

    return &controllers;
}

void setup() {
    Serial.begin(9600);
    pinMode(INDICATOR_LED, OUTPUT);
    server.useController(setupController());

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Connecting to SSID: " + ssid);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, "", 0, bssid);
    while (WiFi.status() != WL_CONNECTED) blinkIndicator();

    // Print local IP address and start web server
    digitalWrite(INDICATOR_LED, ON);
    Serial.println("Connected to SSID: " + ssid);
    Serial.println("IP address: " + WiFi.localIP().toString());

    server.begin();
    Serial.println("HTTP server started");
    digitalWrite(INDICATOR_LED, OFF);
}

void loop() {
    server.handleClient();
}
