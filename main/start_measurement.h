// Author: Rory Norbury|| Student ID: 105914122
// START MEASUREMENT FUNCTION

// Sensor libraries
#include <Wire.h>
#include <BH1750.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

void start_measurement(int sensorAddress)
{
  // prepare and print response message
  String out = String(sensorAddress);
  
  // Allow 5 seconds for the information to be retreived
  out += "005";
  // adds NUM_VALUES to integer value of '0' ASCII character
  out += NUM_VALUES + '0';

  // <CR><LF>
  // possibly translates to <CR><CR><LF> on Windows, need to check
  out += "\r\n";
  Serial.print(out);

  // retrieve data
  // Light (lux)
  double light = lightSensor.readLightLevel();
  lastValues[0] = light;

  if (!bme.performReading()) {
    // reading failed
    for (int i = 1; i < 5; i++) {
      lastValues[i] = 0;
    }
    return;
  }
  else {
    // Temp (C)
    double temp = bme.temperature;
    lastValues[1] = temp;
    // Pressure (Pa)
    double pressure = bme.pressure;
    lastValues[2] = pressure;
    // Humidity (%) 
    double humidity = bme.humidity;
    lastValues[3] = humidity;
    // Gas (kOhms)
    double gas = bme.gas_resistance;
    lastValues[4] = gas;
  }
}