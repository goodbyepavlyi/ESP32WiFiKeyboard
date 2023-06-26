#ifndef KEYBOARD_CONTROL_H
#define KEYBOARD_CONTROL_H

#include <USB.h>
#include <USBHIDKeyboard.h>

class KeyboardControl {
public:
  KeyboardControl();
  void begin();
  void pressKey(uint8_t key);
  void releaseKey(uint8_t key);
  void releaseAllKeys();

private:
  USBHIDKeyboard _keyboard;
};

#endif