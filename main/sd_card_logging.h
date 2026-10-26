#pragma once

//sd card libraray
#include "SdFat.h"
#include "sensors.h"

// define file system for sd card
SdFs sd;
FsFile file;

// Chip select
const int SD_CS_PIN = A3;

// pin numbers
const int SOFT_MISO_PIN = 12;
const int SOFT_MOSI_PIN = 11;
const int SOFT_SCK_PIN = 13;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)

void Clear_SD(File& file){
  sd.remove("LOG.CSV");
  file.open("LOG.CSV", O_RDWR | O_CREAT);
  file.println("TimeStamp,temperature,humidity,pressure,gasresistance,light");
  file.close();
}

void start_SDcard(){

  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();
    while (1);
  }

  if (!file.open("LOG.CSV", O_RDWR)) {
    Clear_SD(file);
    Serial.println("Open");
  }
}

void Write_SD(File& file, String contents){
  file.open("LOG.CSV", O_RDWR | O_AT_END);
  file.println(contents);
  file.close();
}


void logSensordata(sensorData data, String time){
  String log = "";
  log += time + ",";
  log += String(data.temperature) + ",";
  log += String(data.humidity) +",";
  log += String(data.pressure) + ",";
  log += String(data.gas_resistance) + ",";
  log += String(data.light);
  Write_SD(file, log);
}

void Read_SD(File& file){
  file.open("LOG.CSV", O_RDWR);
  String contents = file.readString();
  Serial.println(contents);
  file.close();
}




