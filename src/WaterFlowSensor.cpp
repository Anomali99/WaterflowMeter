#include "WaterFlowSensor.h"

WaterFlowSensor::WaterFlowSensor(uint8_t sensorPin, float calFactor)
    : pin(sensorPin), pulseCount(0), calibrationFactor(calFactor), flowRate(0.0), totalVolume(0.0), previousMillis(0) {}

void IRAM_ATTR WaterFlowSensor::isrWrapper(void* arg) {
  WaterFlowSensor* sensor = static_cast<WaterFlowSensor*>(arg);
  sensor->handleInterrupt();
}

void IRAM_ATTR WaterFlowSensor::handleInterrupt() {
  pulseCount++;
}

void WaterFlowSensor::begin() {
  pinMode(pin, INPUT_PULLUP);

  attachInterruptArg(digitalPinToInterrupt(pin), isrWrapper, this, FALLING);
  previousMillis = millis();
}

void WaterFlowSensor::update() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    detachInterrupt(digitalPinToInterrupt(pin));

    float timeFraction = (currentMillis - previousMillis) / 1000.0;
    float currentPulse = pulseCount;

    flowRate = (currentPulse / timeFraction) / calibrationFactor;
    float volumeThisSecond = (flowRate / 60.0) * timeFraction;
    totalVolume += volumeThisSecond;
    pulseCount = 0;
    previousMillis = currentMillis;
    attachInterruptArg(digitalPinToInterrupt(pin), isrWrapper, this, FALLING);
  }
}

float WaterFlowSensor::getFlowRate() const {
  return flowRate;
}

float WaterFlowSensor::getTotalVolume() const {
  return totalVolume;
}

void WaterFlowSensor::resetTotal() {
  totalVolume = 0.0;
}