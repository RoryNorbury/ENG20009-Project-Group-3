// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// Periodically log data using a timer interrupt
#pragma once

#include "sd_card_logging.h"
#include "timerInterrupts.h"
#include "rtc.h"
#include <DueTimer.h>

volatile bool interruptLoggingFlag = false;


void executeSDCardLogging() {
  extern sensorData avgData;
  String time = rtc::getTimeString(true);
  logSensordata(avgData, time);
  Serial.println("executeSDCardLogging");
  isLog = true;
  //Read_SD(file);
}

void runInterruptLogging() {
  if (interruptLoggingFlag) {
    interruptLoggingFlag = false; 
    executeSDCardLogging();       
  }
}

void vTimerLoggingISR() {
  interruptLoggingFlag = true;
  // Serial.println("vTimerLoggingISR");
}

void setupInterruptLogging() {
  Timer3.attachInterrupt(vTimerLoggingISR);
  Timer3.start(30000000);
}