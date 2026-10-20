// Author: Joseph Cowell | Student ID: 104888857
// ISR for rolling averages

#pragma once

#include <DueTimer.h>
#include "sensors.h"

#define AVG_SAMPLES 5

volatile bool sampleDue = false;

sensorData latestData;
sensorData sumData;
sensorData avgData;
int sampleCount = 0;
bool avgReady = false;

// boolean to trigger updateRollingAvg()
void rollingAvg() {
  sampleDue = true;
}

void printAvg() { // debugging function
  String out = "Avg: ";
  out += "T=" + String(avgData.temperature, 1) + " C, ";
  out += "H=" + String(avgData.humidity, 1) + " %, ";
  out += "P=" + String(avgData.pressure / 100, 1) + " hPa, ";
  out += "G=" + String(avgData.gas_resistance / 1000, 1) + " kOhm, ";
  out += "L=" + String(avgData.light, 1) + " lux";
  Serial.println(out);
}

// takes one sample every 50 ms
void updateRollingAvg() {
  if (!sampleDue) {
    return;
  }
  sampleDue = false;

  // non-blocking read of BH1750
  if (lightSensor.measurementReady()) {
    float lux = lightSensor.readLightLevel();

    if (lux >= 0) {
        latestData.light = lux;
    }
  }

  // non-blocking read of BME680
  if (bme.remainingReadingMillis() == 0) { // sensor data is ready
    if (bme.endReading()) { // ensure reading was successful
      latestData.temperature = bme.temperature;
      latestData.humidity = bme.humidity;
      latestData.pressure = bme.pressure;
      latestData.gas_resistance = bme.gas_resistance;
    }
  }

  if (bme.remainingReadingMillis() < 0) { //sensor is idle
    bme.beginReading();
  }

  // add the newest values to the running totals
  sumData.temperature += latestData.temperature;
  sumData.humidity += latestData.humidity;
  sumData.pressure += latestData.pressure;
  sumData.gas_resistance += latestData.gas_resistance;
  sumData.light += latestData.light;
  sampleCount++;

  // build block to be 5 samples
  if (sampleCount < AVG_SAMPLES) {
    return;
  }

  // divide the totals to get the averages
  avgData.temperature = sumData.temperature / AVG_SAMPLES;
  avgData.humidity = sumData.humidity / AVG_SAMPLES;
  avgData.pressure = sumData.pressure / AVG_SAMPLES;
  avgData.gas_resistance = sumData.gas_resistance / AVG_SAMPLES;
  avgData.light = sumData.light / AVG_SAMPLES;
  avgReady = true;
  sampleCount = 0;
  memset(&sumData, 0, sizeof(sumData));

  printAvg();
}