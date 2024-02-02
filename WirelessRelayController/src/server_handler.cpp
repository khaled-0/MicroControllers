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

    body = "";

    for (auto co : *controllers) {
        body += (co.name);
        body += " ";
        body += (co.getValue());
        body += "<br>";
    }
    server.send(200, "text/html", body);
}

ServerHandler::ServerHandler() {
    server.on("/", [&]() {
        digitalWrite(INDICATOR_LED, INDICATOR_LED_ON);
        handleRoot();
        Serial.println("\nGET /");
        for (auto co : *controllers) {
            Serial.println(co.name);
            Serial.println(co.getValue());
        }
        digitalWrite(INDICATOR_LED, INDICATOR_LED_OFF);
    });
}

void ServerHandler::useController(std::list<Controller> *_controllers) {
    controllers = _controllers;

    for (Controller &controller : *controllers) {
        String getUrlPath = "/" + String(controller.GPIO);
        String toggleUrlPath = "/" + String(controller.GPIO) + "/toggle";
        String setUrlPath = "/" + String(controller.GPIO) + "/set";

        server.on(getUrlPath, [&]() {
            digitalWrite(INDICATOR_LED, INDICATOR_LED_ON);
            server.send(200, "text/plain", String(controller.getValue()));
            digitalWrite(INDICATOR_LED, INDICATOR_LED_OFF);
        });

        server.on(toggleUrlPath, [&]() {
            digitalWrite(INDICATOR_LED, INDICATOR_LED_ON);
            controller.toggleValue();
            handleRoot();
            digitalWrite(INDICATOR_LED, INDICATOR_LED_OFF);
        });

        server.on(setUrlPath, [&]() {
            digitalWrite(INDICATOR_LED, INDICATOR_LED_ON);

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

            handleRoot();
            digitalWrite(INDICATOR_LED, INDICATOR_LED_OFF);
        });
    }
}
