#include <Arduino.h>

#include "Config.h"
#include "WaterFlowSensor.h"
#include "WebSocketsManager.h"

WaterFlowSensor mySensor(SENSOR_PIN, CALIBRATION_FACTOR);

WebSocketsManager webManager;

void setup() {
  Serial.begin(115200);

  mySensor.begin();

  webManager.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Sistem Monitoring Aliran Air (OOP) Dimulai...");
}

void loop() {
  webManager.cleanup();
  mySensor.update();
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();

    float debitAir = mySensor.getFlowRate();
    float volumeTotal = mySensor.getTotalVolume();

    webManager.broadcastData(debitAir, volumeTotal);

    Serial.print("Debit: ");
    Serial.print(debitAir);
    Serial.print(" L/min | Total: ");
    Serial.print(volumeTotal);
    Serial.println(" Liter");
  }
}