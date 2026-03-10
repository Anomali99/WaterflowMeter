#include "WebSocketsManager.h"

#include "WaterFlowSensor.h"
#include "WebPage.h"
extern WaterFlowSensor mySensor;

WebSocketsManager::WebSocketsManager() {
  server = new AsyncWebServer(80);
  ws = new AsyncWebSocket("/ws");
}

WebSocketsManager::~WebSocketsManager() {
  delete ws;
  delete server;
}

void WebSocketsManager::onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client #%u terhubung\n", client->id());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client #%u terputus\n", client->id());
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->opcode == WS_TEXT) {
      data[len] = 0;
      String msg = (char*)data;

      if (msg.indexOf("reset_volume") >= 0) {
        mySensor.resetTotal();
        Serial.println("Berhasil: Total Volume direset dari Web!");
      }
    }
  }
}

void WebSocketsManager::begin(const char* ssid, const char* password) {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("\nWiFi Access Point (AP) Aktif!");
  Serial.print("Nama WiFi yang dipancarkan: ");
  Serial.println(ssid);
  Serial.print("Buka browser dan akses IP (Gateway): ");
  Serial.println(WiFi.softAPIP());

  server->on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  ws->onEvent(onEvent);
  server->addHandler(ws);

  server->begin();
}

void WebSocketsManager::broadcastData(float flowRate, float totalVolume) {
  if (ws->count() == 0) {
    return;
  }

  JsonDocument doc;
  doc["flowRate"] = flowRate;
  doc["totalVolume"] = totalVolume;

  String jsonString;
  serializeJson(doc, jsonString);

  ws->textAll(jsonString);
}

void WebSocketsManager::cleanup() {
  ws->cleanupClients();
}