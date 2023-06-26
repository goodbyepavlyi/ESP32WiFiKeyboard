#include "WiFiConnection.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>

void WiFiConnection::setup() {
  connect();
  setupMDNS();
}

void WiFiConnection::connect() {
  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;

  if (!wifiManager.autoConnect("WiFiHID")) {
    Serial.println("[Error] WiFi: Unable to establish WiFi connection");
    delay(1000);
    ESP.restart();
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("[Info] WiFi: Connecting to WiFi...");
  }

  Serial.printf("[Info] WiFi: MAC: %s, IPv4: %s, IPv6: %s, Gateway: %s, DNS: %s, SSID: %s\n",
    WiFi.macAddress().c_str(), WiFi.localIP().toString().c_str(),
    WiFi.localIPv6().toString().c_str(), WiFi.gatewayIP().toString().c_str(),
    WiFi.dnsIP().toString().c_str(), WiFi.SSID().c_str());
}

void WiFiConnection::setupMDNS() {
  if (MDNS.begin("wifihid")) {
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
    Serial.println("[Info] MDNS: Discovery started");
  } else {
    Serial.println("[Error] MDNS: Discovery failed to start");
  }
}