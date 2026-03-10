#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

class WebSocketsManager {
 private:
  AsyncWebServer* server;
  AsyncWebSocket* ws;
  static void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);

 public:
  WebSocketsManager();
  ~WebSocketsManager();
  void begin(const char* ssid, const char* password);
  void broadcastData(float flowRate, float totalVolume);
  void cleanup();
};
