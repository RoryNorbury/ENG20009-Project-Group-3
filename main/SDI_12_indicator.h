// Author: Rory Norbury || Student ID: 105914122
// Interrupt that blinks an LED when SDI-12 transactions are in progress
#pragma once

#include <DueTimer.h>

#define BLINK_INTERVAL 100000 // microseconds

int indicatorPin = 6;

volatile bool indicatorBlink = false;

void indicatorISR()
{
    indicatorBlink = !indicatorBlink;
    digitalWrite(indicatorPin, indicatorBlink ? HIGH : LOW);
}

void startIndicatorBlink()
{
    Timer5.start();
}

void stopIndicatorBlink()
{
    Timer5.stop();
    indicatorBlink = false;
    digitalWrite(indicatorPin, LOW);
}

void setupIndicatorPin()
{
    pinMode(indicatorPin, OUTPUT);
    digitalWrite(indicatorPin, LOW);
}

void setupIndicatorInterrupt()
{
    Timer5.attachInterrupt(indicatorISR);
    Timer5.setFrequency(BLINK_INTERVAL);
}