// AUTHOR: RORY NORBURY | 105914122
// Initialisation and reading functions for attached sensors
#pragma once

// Sensor libraries
#include <Wire.h>
#include <BH1750.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// Light sensor
BH1750 lightSensor;

// Weather sensor declarations
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;

struct sensorData
{
  float temperature;    // in Celsius
  float humidity;       // in percentage
  float pressure;       // in Pascals
  float gas_resistance; // in kilo Ohms
  float light;          // in LUX
  void fillArray(float array[5])
  {
    array[0] = temperature;
    array[1] = humidity;
    array[2] = pressure;
    array[3] = gas_resistance;
    array[4] = light;
  }
};

/// @brief initialise attached sensors
/// @return 0 on success, 1 on error
bool initSensors()
{
  // init IC2
  Wire.begin();

  // light sensor initialisation
  lightSensor.begin();

  // bme initialisation
  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    return 1;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  return 0;
}

/// @brief read data from sensors into data object
/// @param sensorData will be populated with sensor data
/// @return 0 on success, 1 on failure
bool readSensors(sensorData* data)
{
  data->light = lightSensor.readLightLevel();

  if (!bme.performReading())
  {
    Serial.println("bme sensor did not read properly");
    return 1;
  }
  // Temp (C)
  data->temperature= bme.temperature;
  // Humidity (%)
  data->humidity = bme.humidity;
  // Pressure (Pa)
  data->pressure = bme.pressure;
  // Gas (kOhms)
  data->gas_resistance = bme.gas_resistance;
  return 0;
}