// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// Periodically log data using a timer interrupt
#pragma once

#include "sd_card_logging.h"
#include "timerInterrupts.h"
#include "rtc.h"

volatile bool interruptLoggingFlag = false;


void executeSDCardLogging() {
  extern sensorData avgData;
  String time = rtc::getTimeString(true);
  logSensordata(avgData, time);
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