// used by other files, so must be declared before
int sdiAddress = '0';
bool dataReady = false;
int NUM_VALUES = 5;
float lastValues[5] = {
  0,  // Temperature
  0,  // Humidity
  0,  // Pressure
  0,  // Gas
  0   // Lux
};

// variable declarations here

#include "command_parser.h"
#include "serial_monitor_input_for_testing.h"
#include "sensors.h"

void setup() {
  // setup here
  Serial.begin(9600);
  initSensors();
}

void loop() {
  // main code here
  while (1) {
    String str = serial_test_input_string();
    String ret = interpret_command(str);
    Serial.println(ret);
  }
}