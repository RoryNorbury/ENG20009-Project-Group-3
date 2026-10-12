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


#include "command_parser.h"
#include "serial_monitor_input_for_testing.h"
#include "sensors.h"
#include "LCD.h"
#include "threshold_alert.h"

// variable declarations here
int ledAlertPin = 11;

void setup() {
  // setup here
  Serial.begin(9600); //9600 for arduino to PC communication
  Serial1.begin(1200, SERIAL_7E1); //1200 baud for SDI, 7 data bits and 1 error bit
  pinMode(7, OUTPUT);   
  digitalWrite(7, HIGH); //HIGH = READ. LOW = WRITE

  pinMode(ledAlertPin, OUTPUT);
  digitalWrite(ledAlertPin, HIGH);

  if (initSensors())
  {
    Serial.println("Could not initialise sensors, exiting");
    exit(1);
  }

  delay(1000);
  Serial.println("Compliant sensor node");

  //LCD init:
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
}

void loop() {
  // main code here
  while (1) {
    String str = serial_test_input_string();
    Serial.println(str);
    String ret = interpret_command(str);
    Serial.println(ret);

    sensorData data;
    readSensors(&data);
    u_int8_t alerts = check_alert_thresholds(data);
    if (alerts > 0)
    {
      displayAlert(alerts);
      digitalWrite(ledAlertPin, HIGH);
    }
    else
    {
      updateLCD(lastValues);
      digitalWrite(ledAlertPin, LOW);
    }
  }
}