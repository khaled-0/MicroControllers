#include <ESP8266WebServer.h>

#include <list>

#include "controller.h"

class ServerHandler {
   private:
    ESP8266WebServer server = ESP8266WebServer(80);
    std::list<Controller> *controllers;
    void handleRoot();
    void redirect(String url = "/");

   public:
    ServerHandler();
    void useController(std::list<Controller> *);
    void begin();
    void handleClient();
};
