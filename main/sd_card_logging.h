// Author: NOAH MUNRO | 105912100
// File for sd card reading, writing and logging

#pragma once

//sd card libraray
#include "SdFat.h"
#include "sensors.h"

// define file system for sd card
SdFs sd;
FsFile file;

// number of free bytes in sd card
uint64_t freeBytes = 0;
uint32_t logCount = 0;

// Chip select
const int SD_CS_PIN = A3;

// pin numbers
const int SOFT_MISO_PIN = 12;
const int SOFT_MOSI_PIN = 11;
const int SOFT_SCK_PIN = 13;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)

void updateFreeBytes()
{
  // get number of free clusters
  uint32_t freeClusters = sd.vol()->freeClusterCount();
  // get number of blocks per cluster
  uint32_t blocksPerCluster = sd.vol()->sectorsPerCluster();
  // calculate free space in bytes (1 block = 512 bytes)
  uint64_t free = (uint64_t)freeClusters * blocksPerCluster * 512;
  if (free < 16000000000)
  {
    freeBytes = free;
  }
}

void Clear_SD(File& file){
  sd.remove("LOG.CSV");
  file.open("LOG.CSV", O_RDWR | O_CREAT);
  file.println("TimeStamp,temperature,humidity,pressure,gasresistance,light");
  file.close();
  updateFreeBytes();
}

void start_SDcard(){

  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();
    while (1);
  }

  Clear_SD(file);
}

void Write_SD(File& file, String contents){
  file.open("LOG.CSV", O_RDWR | O_AT_END);
  file.println(contents);
  file.close();
  updateFreeBytes();
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
  logCount++
}

void Read_SD(File& file){
  file.open("LOG.CSV", O_RDWR);
  String contents = file.readString();
  Serial.println(contents);
  file.close();
}

uint64_t getFreeBytes()
{
  return freeBytes;
}

int getLineCount()
{
  return logCount;
}