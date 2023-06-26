#ifndef WebSocketHandler_h
#define WebSocketHandler_h

#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <unordered_map>
#include "KeyboardControl.h"

class WebSocketHandler {
public:
  WebSocketHandler(KeyboardControl& keyboardControl);
  void begin();
  void handleClient();

private:
  WebSocketsServer webSocketServer;
  KeyboardControl& _keyboardControl;

  void handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};

#endif
