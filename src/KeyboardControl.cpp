#include "KeyboardControl.h"

KeyboardControl::KeyboardControl() {
}

void KeyboardControl::begin() {
  USB.usbClass(0);
  USB.usbSubClass(0);
  USB.usbProtocol(0);
  _keyboard.begin();
  USB.begin();
}

void KeyboardControl::pressKey(uint8_t key) {
  _keyboard.press(key);
}

void KeyboardControl::releaseKey(uint8_t key) {
  _keyboard.release(key);
}

void KeyboardControl::releaseAllKeys() {
  _keyboard.releaseAll();
}