#include "server_handler.h"

#include <Arduino.h>
#include <ESP8266WebServer.h>

#include "utils.h"

void ServerHandler::begin() {
    server.begin();
}
void ServerHandler::handleClient() {
    server.handleClient();
}

void ServerHandler::handleRoot() {
    String output5State = "off";
    String output4State = "off";

    String body = "<!DOCTYPE html><html>";
    body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    body += "<link rel=\"icon\" href=\"data:,\">";
    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences
    body += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    body += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
    body += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    body += ".button2 {background-color: #77878A;}</style></head>";

    // Web Page Heading
    body += "<body><h1>ESP8266 Web Server</h1>";

    // Display current state, and ON/OFF buttons for GPIO 5
    body += "<p>GPIO 5 - State " + output5State + "</p>";
    // If the output5State is off, it displays the ON button
    if (output5State == "off") {
        body += "<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>";
    } else {
        body += "<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>";
    }

    // Display current state, and ON/OFF buttons for GPIO 4
    body += "<p>GPIO 4 - State " + output4State + "</p>";
    // If the output4State is off, it displays the ON button
    if (output4State == "off") {
        body += "<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>";
    } else {
        body += "<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>";
    }
    body += "</body></html>";

    callbackFunc("gw");
    server.send(200, "text/html", body);
}

void ServerHandler::setListener(void (*_callback)(String)) {
    callbackFunc = _callback;
}

ServerHandler::ServerHandler() {
    server.on("/", [&]() {
        digitalWrite(INDICATOR_LED, INDICATOR_LED_ON);
        handleRoot();
        digitalWrite(INDICATOR_LED, INDICATOR_LED_OFF);
    });
}
