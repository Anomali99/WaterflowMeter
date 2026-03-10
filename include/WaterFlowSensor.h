#pragma once
#include <Arduino.h>

class WaterFlowSensor {
 private:
  uint8_t pin;
  volatile uint32_t pulseCount;
  float calibrationFactor;
  float flowRate;
  float totalVolume;
  unsigned long previousMillis;
  static void IRAM_ATTR isrWrapper(void* arg);
  void handleInterrupt();

 public:
  WaterFlowSensor(uint8_t sensorPin, float calFactor = 4.5);
  void begin();
  void update();
  float getFlowRate() const;
  float getTotalVolume() const;
  void resetTotal();
};
