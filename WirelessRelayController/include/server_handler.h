#include <ESP8266WebServer.h>

class ServerHandler {
   private:
    ESP8266WebServer server = ESP8266WebServer(80);
    void handleRoot();
    void clientHandler();
    void (*callbackFunc)(String);

   public:
    ServerHandler();
    void begin();
    void handleClient();

    void setListener(void (*callback)(String));
};
