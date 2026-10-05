// used by other files, so must be declared before
int sdiAddress = 0;
bool dataReady = true;
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
  Serial.begin(9600); //9600 for arduino to PC communication
  Serial1.begin(1200, SERIAL_7E1); //1200 baud for SDI, 7 data bits and 1 error bit
  pinMode(7, OUTPUT);   
  digitalWrite(7, HIGH); //HIGH = READ. LOW = WRITE

  if (initSensors())
  {
    Serial.println("Could not initialise sensors, exiting");
    exit(1);
  }

  delay(1000);
  Serial.println("Compliant sensor node");
}

void loop() {
  // main code here
  while (1) {
    String str = serial_test_input_string();
    Serial.println(str);
    String ret = interpret_command(str);
    Serial.println(ret);
  }
}