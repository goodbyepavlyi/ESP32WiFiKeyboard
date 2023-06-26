#include "WebSocketHandler.h"

WebSocketHandler::WebSocketHandler(KeyboardControl& keyboardControl) : _keyboardControl(keyboardControl), webSocketServer(81) {}

// Keymap
std::unordered_map<std::string, uint8_t> keyMap = {
  {"KEY_LEFT_CTRL", KEY_LEFT_CTRL},
  {"KEY_LEFT_SHIFT", KEY_LEFT_SHIFT},
  {"KEY_LEFT_ALT", KEY_LEFT_ALT},
  {"KEY_LEFT_GUI", KEY_LEFT_GUI},
  {"KEY_RIGHT_CTRL", KEY_RIGHT_CTRL},
  {"KEY_RIGHT_SHIFT", KEY_RIGHT_SHIFT},
  {"KEY_RIGHT_ALT", KEY_RIGHT_ALT},
  {"KEY_RIGHT_GUI", KEY_RIGHT_GUI},
  {"KEY_UP_ARROW", KEY_UP_ARROW},
  {"KEY_DOWN_ARROW", KEY_DOWN_ARROW},
  {"KEY_LEFT_ARROW", KEY_LEFT_ARROW},
  {"KEY_RIGHT_ARROW", KEY_RIGHT_ARROW},
  {"KEY_BACKSPACE", KEY_BACKSPACE},
  {"KEY_TAB", KEY_TAB},
  {"KEY_RETURN", KEY_RETURN},
  {"KEY_ESC", KEY_ESC},
  {"KEY_INSERT", KEY_INSERT},
  {"KEY_DELETE", KEY_DELETE},
  {"KEY_PAGE_UP", KEY_PAGE_UP},
  {"KEY_PAGE_DOWN", KEY_PAGE_DOWN},
  {"KEY_HOME", KEY_HOME},
  {"KEY_END", KEY_END},
  {"KEY_CAPS_LOCK", KEY_CAPS_LOCK},
  {"KEY_F1", KEY_F1},
  {"KEY_F2", KEY_F2},
  {"KEY_F3", KEY_F3},
  {"KEY_F4", KEY_F4},
  {"KEY_F5", KEY_F5},
  {"KEY_F6", KEY_F6},
  {"KEY_F7", KEY_F7},
  {"KEY_F8", KEY_F8},
  {"KEY_F9", KEY_F9},
  {"KEY_F10", KEY_F10},
  {"KEY_F11", KEY_F11},
  {"KEY_F12", KEY_F12},
  {"KEY_F13", KEY_F13},
  {"KEY_F14", KEY_F14},
  {"KEY_F15", KEY_F15},
  {"KEY_F16", KEY_F16},
  {"KEY_F17", KEY_F17},
  {"KEY_F18", KEY_F18},
  {"KEY_F19", KEY_F19},
  {"KEY_F20", KEY_F20},
  {"KEY_F21", KEY_F21},
  {"KEY_F22", KEY_F22},
  {"KEY_F23", KEY_F23},
  {"KEY_F24", KEY_F24}
};

void WebSocketHandler::begin() {
  webSocketServer.begin();
  webSocketServer.onEvent([this](uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    this->handleWebSocketEvent(num, type, payload, length);
  });

  Serial.println("[Info] WebSocket: Listening on port 81");
}

void WebSocketHandler::handleClient() {
  webSocketServer.loop();
}

void WebSocketHandler::handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[Info] WebSocket: [Client %u] Disconnected!\n", num);
      break;
    
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketServer.remoteIP(num);
        Serial.printf("[Info] WebSocket: [Client %u] Connected from %d.%d.%d.%d, URL: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
      }
      break;
    
    case WStype_TEXT:
      Serial.printf("[Info] WebSocket: [Client %u] Received message: %s\n", num, payload);

      StaticJsonDocument<100> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.printf("[Error] WebSocket: Failed to deserialize JSON: %s\n", error.f_str());
        return;
      }

      const char* event = doc["event"];
      const char* key = doc["key"];

      if (strcmp(event, "KeyPress") == 0) {
        // Look up the key in the mapping
        auto it = keyMap.find(key);

        if (it != keyMap.end()) {
          _keyboardControl.pressKey(it->second);
        } else {
          // Treat the key as a regular character
          char character = *key;
          _keyboardControl.pressKey(character);
        }
      } else if (strcmp(event, "KeyRelease") == 0) {
        // Look up the key in the mapping
          auto it = keyMap.find(key);

          if (it != keyMap.end()) {
            _keyboardControl.releaseKey(it->second);
          } else {
            // Treat the key as a regular character
            char character = *key;
            _keyboardControl.releaseKey(character);
          }
      }

      break;
  }
}