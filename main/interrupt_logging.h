#include "sd_card_logging.h"
#include "timerInterrupts.h"
#include "rtc.h"

volatile bool interruptLoggingFlag = false;


void executeSDCardLogging() {
  extern sensorData smoothData;
  String time = rtc::getTimeString(true);
  logSensordata(smoothData, time);
}

void runInterruptLogging() {
  if (interruptLoggingFlag) {
    interruptLoggingFlag = false; 
    executeSDCardLogging();       
  }
}

void vTimerLoggingISR() {
  interruptLoggingFlag = true;
}

void setupInterruptLogging() {
  Timer3.attachInterrupt(vTimerLoggingISR);
  Timer3.start(30000000);
}