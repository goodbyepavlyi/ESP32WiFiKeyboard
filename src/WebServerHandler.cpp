#include "WebServerHandler.h"
#include "WebPage.h"

WebServerHandler::WebServerHandler() : server(80) {}

void WebServerHandler::begin() {
  server.onNotFound([this]() { this->handleNotFound(); });
  server.on("/", [this]() { this->handleRoot(); });

  server.begin();
  Serial.println("[Info] Web Server: Listening on port 80");
}

void WebServerHandler::handleClient() {
  server.handleClient();
}

void WebServerHandler::handleNotFound() {
  server.send(200, "text/html", NOT_FOUND);
}

void WebServerHandler::handleRoot() {
  server.send(200, "text/html", WEBPAGE);
}
