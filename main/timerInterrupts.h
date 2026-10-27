// Author: Joseph Cowell | Student ID: 104888857
// ISR for rolling averages

#pragma once

#include <DueTimer.h>

#define SAMPLE_INT     500000 // MICROseconds. Decrease with CAUTION! comment out printAvg(); [L97] below 100ms
#define AVG_SAMPLES      5
#define NUM_VARS         5

#define ROW_TEMP     0
#define ROW_HUMIDITY 1
#define ROW_PRESSURE 2
#define ROW_GAS      3
#define ROW_LIGHT    4


float dataMatrix[NUM_VARS][AVG_SAMPLES];
int sampleCount = 0;
sensorData latestData;
sensorData avgData;
volatile bool avgReady = false;

void printAvg() { // debugging function
String out = "Avg: ";
  out += "T=" + String(avgData.temperature, 1) + " C, ";
  out += "H=" + String(avgData.humidity, 1) + " %, ";
  out += "P=" + String(avgData.pressure / 100, 1) + " hPa, ";
  out += "G=" + String(avgData.gas_resistance / 1000, 1) + " kOhm, ";
  out += "L=" + String(avgData.light, 1) + " lux";
  Serial.println(out);
}

float rowAverage(int row) { // returns avg of row in matrix
  float sum = 0;
  for (int col = 0; col < AVG_SAMPLES; col++) {
    sum += dataMatrix[row][col];
  }
  return sum / AVG_SAMPLES;
}

void rollingAvg() {
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

  // newest values
  float newest[NUM_VARS];
  newest[ROW_TEMP] = latestData.temperature;
  newest[ROW_HUMIDITY] = latestData.humidity;
  newest[ROW_PRESSURE] = latestData.pressure;
  newest[ROW_GAS] = latestData.gas_resistance;
  newest[ROW_LIGHT] = latestData.light;

  // shift every row left by one, newest value goes in the rightmost column
  for (int row = 0; row < NUM_VARS; row++) {
    for (int col = 0; col < AVG_SAMPLES - 1; col++) {
      dataMatrix[row][col] = dataMatrix[row][col + 1];
    }
    dataMatrix[row][AVG_SAMPLES - 1] = newest[row];
  }

  // wait until the matrix is full before averaging
  if (sampleCount < AVG_SAMPLES) {
    sampleCount++;
    return;
  }

  // average each row
  avgData.temperature = rowAverage(ROW_TEMP);
  avgData.humidity = rowAverage(ROW_HUMIDITY);
  avgData.pressure = rowAverage(ROW_PRESSURE);
  avgData.gas_resistance = rowAverage(ROW_GAS);
  avgData.light = rowAverage(ROW_LIGHT);
  avgReady = true;

  avgData.fillArray(lastValues);

  printAvg();
}

void setupRollingAvgInterrupt() {
  Timer1.attachInterrupt(rollingAvg);
  Timer1.start(SAMPLE_INT);
}