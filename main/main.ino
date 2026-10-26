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
#include "rtc.h"
#include "timerInterrupts.h"
#include "interrupt_logging.h"
#include "SDI_12_indicator.h"

// variable declarations here
int ledAlertPin = 11;
int DIRO = 8;

void setup() {
  // setup here
  Serial.begin(9600); //9600 for arduino to PC communication
  Serial1.begin(1200, SERIAL_7E1); //1200 baud for SDI, 7 data bits and 1 error bit
  pinMode(DIRO, OUTPUT);   
  digitalWrite(DIRO, HIGH); //HIGH = READ. LOW = WRITE

  pinMode(ledAlertPin, OUTPUT);
  digitalWrite(ledAlertPin, HIGH);
  
  // setup SDI-12 transaction indicator LED
  setupIndicatorPin();
  
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
  
  start_SDcard();
  
  delay(1000);
  Serial.println("Compliant sensor node");

  //LCD init:
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  
  readSensors(&latestData);
  
  //attach ISR for rolling avg
  Timer1.attachInterrupt(rollingAvg);
  Timer1.start(100000); // 100ms = 10Hz
  
  // set up interrupt for logging
  setupInterruptLogging();

  // set up interrupt for SDI-12 transaction indicator
  setupIndicatorInterrupt();
}

void handle_input()
{
  if (Serial1.available() > 0)
  {
    // enable SD-12 indicator blinking while input is being handled
    startIndicatorBlink();
    
    String str = Serial1.readString(); // reads string from Serial Monitor
    str = str.substring(1); //removes weird square/junk character
    str.trim(); //removes trailing whitespace
    Serial.println(str);
    
    String ret = interpret_command(str);

    Serial.println(ret);
    sendToSDA(ret, DIRO);

    // stop SDI-12 indicator
    stopIndicatorBlink();
  }
}

void loop() {
  handle_input();

  updateRollingAvg();
  if (!avgReady)
  {
    return;
  }

  runInterruptLogging();
  
  uint8_t alerts = check_alert_thresholds(avgData);
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
    float avgValues[2][4];
    if (wasAlerted){
      tft.fillScreen(ST77XX_BLACK);
      wasAlerted = false;
    }
    avgData.fillArray(avgValues);
    updateLCD(avgValues);
    digitalWrite(ledAlertPin, LOW);
  }
}