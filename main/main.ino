// used by other files, so must be declared before
int sdiAddress = 0;
bool dataReady = true;
int NUM_VALUES = 5;
float lastValues[2][4] = {
  {
    0,  // Temperature
    0,  // Humidity
    0,  // Pressure
    0,  // Gas
  },
  {  
    0,   // Lux
    0,
    0,
    0
  }
};
bool wasAlerted = false;
bool isLog = false;

#include "command_parser.h"
#include "serial_monitor_input_for_testing.h"
#include "sensors.h"
#include "LCD.h"
#include "threshold_alert.h"
#include "SDA.h"
#include "periodic_logging.h"
#include "rtc.h"

// variable declarations here
int ledAlertPin = 11;

void setup() {
  // setup here
  Serial.begin(9600); //9600 for arduino to PC communication
  Serial1.begin(1200, SERIAL_7E1); //1200 baud for SDI, 7 data bits and 1 error bit
  pinMode(8, OUTPUT);   
  digitalWrite(8, HIGH); //HIGH = READ. LOW = WRITE

  pinMode(ledAlertPin, OUTPUT);
  digitalWrite(ledAlertPin, HIGH);

  if (initSensors())
  {
    Serial.println("Could not initialise sensors, exiting");
    exit(1);
  }

  if (rtc::initRTC(true))
  {
    Serial.println("Could not initialise rtc, exiting");
    exit(1);
  }

  delay(1000);
  Serial.println("Compliant sensor node");

  //LCD init:
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
}

void handle_input()
{
  if (Serial1.available() > 0)
  {
    String str = Serial1.readString(); // reads string from Serial Monitor
    str = str.substring(1); //removes weird square/junk character
    str.trim(); //removes trailing whitespace
    Serial.println(str);

    String ret = interpret_command(str);

    Serial.println(ret);
    sendToSDA(ret);
  }
}

void loop() {
  handle_input();

  update_periodic_logging();

  sensorData data;
  readSensors(&data);
  uint8_t alerts = check_alert_thresholds(data);
  if (alerts > 0)
  {
    if (!wasAlerted){
      tft.fillScreen(ST77XX_BLACK);
      wasAlerted = true;
    }
    displayAlert(alerts);
    digitalWrite(ledAlertPin, HIGH);
  }
  else
  {
    float values[2][4];
    if (wasAlerted){
      tft.fillScreen(ST77XX_BLACK);
      wasAlerted = false;
    }
    data.fillArray(values);
    updateLCD(values);
    digitalWrite(ledAlertPin, LOW);
  }
}