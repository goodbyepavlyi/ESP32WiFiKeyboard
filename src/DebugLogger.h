#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <Arduino.h>

class DebugLogger
{
public:
  void begin(unsigned long interval);
  void logUptime();

private:
  unsigned long bootTime = 0;
  unsigned long previousTime = 0;
  unsigned long interval = 1000; // Default interval of 1 second
};

#endif
