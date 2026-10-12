// Author: Joseph Cowell | Student ID: 104888857
// LCD init and function

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

void updateLCD(float data[2][4])
{
  //clear screen, then display: temp, humidity, pressure, and gas resistance on LCD
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.print("Temperature: ");
  tft.print(data[0][0]);
  tft.println(" C");

  tft.print("Humidity:    ");
  tft.print(data[0][1]);
  tft.println(" %");

  tft.print("Pressure:    ");
  tft.print(int(data[0][2])); //force to int as original float contains useless decimal (e.g. 12345.00)
  tft.println(" Pa");

  tft.print("Gas Resis:   ");
  tft.print(int(data[0][3])); //force to int as original float contains useless decimal
  tft.println(" kOhms");

  tft.print("Light level:   ");
  tft.print(data[1][0]);
  tft.println(" Lux");
}

void displayAlert(uint8_t alert_flag)
{
  // do nothing if no alerts
  if (alert_flag == 0)
  {
    return;
  }

  // clear screen and set text colour to red
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.fillScreen(ST77XX_BLACK);
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
  // reset text colour and size back to what it was
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
}