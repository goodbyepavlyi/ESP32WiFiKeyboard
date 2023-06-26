#include "DebugLogger.h"
#include "WiFiConnection.h"
#include "WebServerHandler.h"
#include "WebSocketHandler.h"
#include "KeyboardControl.h"

DebugLogger debugLogger;
WiFiConnection wifiConnection;
WebServerHandler webServer;
KeyboardControl keyboardControl;
WebSocketHandler webSocketHandler(keyboardControl);

void setup() {
  debugLogger.begin(1000);

  keyboardControl.begin();
  wifiConnection.setup();
  webServer.begin();
  webSocketHandler.begin();
}

void loop() {
  debugLogger.logUptime();
  webServer.handleClient();
  webSocketHandler.handleClient();
}