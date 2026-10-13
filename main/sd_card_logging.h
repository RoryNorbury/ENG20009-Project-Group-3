//sd card libraray
#include "SdFat.h"

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

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), $softSpi)

void start_SDcard(){

  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();
    while (1);
  }

  if (!file.open("message.txt", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.close(); //release file

}

void Write_SD(File& file, String log){
  
}
