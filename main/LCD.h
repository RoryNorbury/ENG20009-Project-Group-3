// Author: Joseph Cowell | Student ID: 104888857
// LCD init and function

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h> // Hardware SPI library

//pins for LCD
#define TFT_CS    10
#define TFT_RST   6 
#define TFT_DC    7 
#define TFT_SCLK 13   
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST); //creates object called tft

void updateLCD(float* data)
{
  //clear screen, then display: temp, humidity, pressure, and gas resistance on LCD
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.print("Temperature: ");
  tft.print(data[0]);
  tft.println(" C");

  tft.print("Humidity:    ");
  tft.print(data[1]);
  tft.println(" %");

  tft.print("Pressure:    ");
  tft.print(int(data[2])); //force to int as original float contains useless decimal (e.g. 12345.00)
  tft.println(" Pa");

  tft.print("Gas Resis:   ");
  tft.print(int(data[3])); //force to int as original float contains useless decimal
  tft.println(" kOhms");
}