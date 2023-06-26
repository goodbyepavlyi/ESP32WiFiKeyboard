#include "DebugLogger.h"

void DebugLogger::begin(unsigned long interval) {
  this->interval = interval;
  bootTime = millis();
  previousTime = millis();

  Serial.begin(115200);
}

void DebugLogger::logUptime() {
  if ((millis() - previousTime) <= interval)
    return;
  
  Serial.printf("[Info] Main: Uptime %d seconds\n", ((millis() - bootTime) / 1000));
  previousTime = millis();
}
