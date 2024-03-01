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

void ServerHandler::redirect(String url /*="/"*/) {
    server.sendHeader("Refresh", "0; url=" + url);
    server.send(200);
}

void ServerHandler::handleRoot() {
    String output5State = "off";
    String output4State = "off";

    String body = "<!DOCTYPE html><html>";
    body += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    body += "<link rel=\"icon\" href=\"data:,\">";
    body += "<title>ESP8266 Web Server</title>";

    body += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    body += "button { border: none; color: white; padding: 16px 40px;";
    body += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>";

    body += "<body><h1>ESP8266 Web Server</h1>";

    // Display current state, and ON/OFF buttons for GPIO Pins
    for (Controller &controller : *controllers) {
        String toggleUrlPath = "/" + String(controller.GPIO) + "/toggle";
        bool isON = controller.getValueRaw() == (uint8_t)ON;
        String buttonLabel = isON ? "TURN OFF" : "TURN ON";

        body += "<p>" + controller.name + " " + controller.getValue() + "</p>";
        body += "<p><a href='" + toggleUrlPath + "'>";
        body += String("<button style='background-color:") + (isON ? "green" : "grey") + "'>" + buttonLabel + "</button></a></p>";
    }

    body += "</body></html>";
    server.send(200, "text/html", body);
}

ServerHandler::ServerHandler() {
    server.on("/", [&]() {
        digitalWrite(INDICATOR_LED, ON);
        handleRoot();
        digitalWrite(INDICATOR_LED, OFF);
    });
}

void ServerHandler::useController(std::list<Controller> *_controllers) {
    controllers = _controllers;

    for (Controller &controller : *controllers) {
        String getUrlPath = "/" + String(controller.GPIO);
        String toggleUrlPath = "/" + String(controller.GPIO) + "/toggle";
        String setUrlPath = "/" + String(controller.GPIO) + "/set";

        server.on(getUrlPath, [&]() {
            digitalWrite(INDICATOR_LED, ON);
            server.send(200, "text/plain", String(controller.getValue()));
            digitalWrite(INDICATOR_LED, OFF);
        });

        server.on(toggleUrlPath, [&]() {
            digitalWrite(INDICATOR_LED, ON);
            controller.toggleValue();
            redirect();
            digitalWrite(INDICATOR_LED, OFF);
        });

        server.on(setUrlPath, [&]() {
            digitalWrite(INDICATOR_LED, ON);

            for (int i = 0; i < server.args(); ++i) {
                if (server.argName(i) == "value") {
                    String argValue = server.arg(i);

                    uint8_t value = controller.getValueRaw();
                    if (argValue == "1" || argValue == "true" || argValue == "on")
                        value = ON;
                    else if (argValue == "0" || argValue == "false" || argValue == "off")
                        value = OFF;

                    controller.setValue(value);
                }
            }

            redirect();
            digitalWrite(INDICATOR_LED, OFF);
        });
    }
}
