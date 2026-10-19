// Button navigation task (ENG20009 Credit)
// Button 1: force an immediate manual log entry to the SD card.
// Button 2: toggle between the live data screen and the "System Info" screen
//           (total log count and free storage/status).
//
// Standalone test version: output goes to Serial so you can test the buttons now.
// Lines marked TODO are where main's real code (SD, LCD, sensors) plugs in.

//include other functions written by the other members...
#include <Arduino.h>
#include "periodic_logging.h"


const uint8_t  log_BUTTON_PIN    = 2;    // manual log button      (check the schematic)
const uint8_t  toggle_BUTTON_PIN = 3;    // screen toggle button   (check the schematic)
const uint32_t DEBOUNCE_MS       = 30;

//defining the lcd screen
#define TFT_CS 10
#define TFT_RST 6 
#define TFT_DC 7 
#define TFT_SCLK 13   
#define TFT_MOSI 11   

//creates the display and tells program the pins
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


// defining all button components
struct Button {
  uint8_t  pin;
  bool     rawLast;
  bool     stable;
  uint32_t tChange;

  Button(uint8_t p) : pin(p), rawLast(false), stable(false), tChange(0) {} //define start states

  void begin() { pinMode(pin, INPUT_PULLUP); }      

  bool pressed() {                                  
    bool raw = (digitalRead(pin) == LOW);
    uint32_t now = millis();
    if (raw != rawLast) { rawLast = raw; tChange = now; }
    if ((now - tChange) >= DEBOUNCE_MS && raw != stable) {
      stable = raw;
      if (stable) return true;
    }
    return false;
  }
};

//defining buttons
Button logBtn(log_BUTTON_PIN);
Button toggleBtn(toggle_BUTTON_PIN);

//printing on the screen
enum Screen { SCREEN_LIVE, SCREEN_SYSINFO };
Screen   currentScreen = SCREEN_LIVE;
bool     redrawNeeded  = true;     // set this to true whenever new sensor values arrive
uint32_t logCount      = 0;
uint32_t cachedFreeKB  = 0;

Print &lcd = tft;               // TODO: replace with your real display object

uint32_t freeStorageKB() { return 0; }   // TODO: free space on the SD card, in KB

//Button 1 action
void manualLog() {
  logSensordata();
  Serial.println("manual log");
  logCount++;                      // only count it if the SD write succeeded
  redrawNeeded = true;
}

// printing on the LCD screen
void drawLiveScreen() { //printing everything on the same line so it is all one line
  lcd.print("Temperature: "); lcd.print(latest.temperature, 1);    lcd.println(" C");
  lcd.print("Humidity: ");    lcd.print(latest.humidity, 1);       lcd.println(" %");
  lcd.print("Pressure: ");    lcd.print(latest.pressure, 1);       lcd.println(" hPa");
  lcd.print("Gas: ");         lcd.print(latest.gas_resistance, 1); lcd.println();
  lcd.print("Lux: ");         lcd.print(latest.lux, 0);            lcd.println(" lx");
}

void drawSystemInfo() {
  lcd.println("SYSTEM INFO");
  lcd.print("Logs: ");  lcd.println(logCount);
  lcd.print("Free: ");  lcd.print(cachedFreeKB);  lcd.println(" KB");
}

void setup() {
  Serial.begin(115200);
  logBtn.begin();
  toggleBtn.begin();
}

void loop() {
  if (logBtn.pressed()) manualLog();                 // Button 1

  if (toggleBtn.pressed()) {                         // Button 2
    currentScreen = (currentScreen == SCREEN_LIVE) ? SCREEN_SYSINFO : SCREEN_LIVE;
    if (currentScreen == SCREEN_SYSINFO) cachedFreeKB = freeStorageKB();  // once per visit, can be slow
    redrawNeeded = true;
    // TODO: clear the LCD here before drawing the new screen
  }

  if (redrawNeeded) {                                // draw only when something changed
    redrawNeeded = false;
    if (currentScreen == SCREEN_LIVE) drawLiveScreen();
    else                              drawSystemInfo();
  }
}