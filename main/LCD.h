// Author: Joseph Cowell | Student ID: 104888857
// LCD init and function
#pragma once

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h> // Hardware SPI library
#include "threshold_alert.h"

//pins for LCD
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 
#define TFT_SCLK 13   
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST); //creates object called tft

int lastLCDDraw = 0;
int LCDDrawPeriod = 6000;

void updateLCD(float data[2][4])
{
  if (millis() > lastLCDDraw + LCDDrawPeriod)
  {
    lastLCDDraw = millis();

    tft.setTextColor(ST77XX_WHITE,  ST77XX_BLACK);
    tft.setTextSize(1);
    
    //clear screen, then display: temp, humidity, pressure, and gas resistance on LCD
    tft.setCursor(0, 0);
    tft.print("Temperature: ");
    tft.print(data[0][0], 1); // rounded to 1dp
    tft.println(" C  ");

    tft.print("Humidity:    ");
    tft.print(data[0][1], 1); // rounded to 1 dp
    tft.println(" %  ");
    
    tft.print("Pressure:    ");
    tft.print(round((data[0][2])/100)); // converted to HPa (standard unit)
    tft.println(" HPa  ");
    
    tft.print("Gas Resis:   ");
    tft.print(round((data[0][3])/1000)); // converted to KOhms
    tft.println(" kOhms  ");
    
    tft.print("Light Level: ");
    tft.print(round(data[1][0]));
    tft.println(" Lux  ");

    tft.println("");
    tft.print("Logging to SD: ");
    if(isLog){
      tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
      tft.println("IN PROGRESS");
      isLog = false;
    }
    else{
      tft.println("INACTIVE   ");
    }
  }
}

void displayAlert(uint8_t alert_flag)
{
  // do nothing if no alerts
  if (alert_flag == 0)
  {
    return;
  }

  if (millis() > lastLCDDraw + LCDDrawPeriod)
  {
    lastLCDDraw = millis();

    // clear screen and set text colour to red
    tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);

    tft.println("ALERT");
    if (alert_flag & sensor_alert_flags::LIGHT)
    {
      tft.println("Light is below threshold!");
    }
    if (alert_flag & sensor_alert_flags::HUMIDITY)
    {
      tft.println("Humidity is above threshold!");
    }
    if (alert_flag & sensor_alert_flags::GAS)
    {
      tft.println("Gas is above threshold!");
    }
  }
}