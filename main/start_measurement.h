// Author: Rory Norbury|| Student ID: 105914122
// START MEASUREMENT FUNCTION
#pragma once
#include "sensors.h"

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

  sensorData data;
  if (!readSensors(&data))
  {
    data.fillArray(lastValues);
  }
}