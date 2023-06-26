#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

class WiFiConnection {
public:
  void setup();

private:
  void connect();
  void setupMDNS();
};

#endif
