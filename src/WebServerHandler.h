#ifndef WebServerHandler_h
#define WebServerHandler_h

#include <WebServer.h>

class WebServerHandler {
public:
  WebServerHandler();
  void begin();
  void handleClient();
  
private:
  WebServer server;
  
  void handleNotFound();
  void handleRoot();
};

#endif