// used by other files, so must be declared before
int sdiAddress = '0';
bool dataReady = false;
int NUM_VALUES = 5;
double lastValues[5] = {
  0,  // Temperature
  0,  // Humidity
  0,  // Pressure
  0,  // Gas
  0   // Lux
};

// variable declarations here

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

#include "command_parser.h"
#include "serial_monitor_input_for_testing.h"

void setup() {
  // setup here
  Serial.begin(9600);

  // init IC2
  Wire.begin();

  // light sensor initialisation
  lightSensor.begin();

  // bme initialisation
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    exit(1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms
}

void loop() {
  // main code here
  while (1) {
    String str = serial_test_input_string();
    String ret = interpret_command(str);
    Serial.println(ret);
  }
}