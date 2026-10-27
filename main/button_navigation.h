// Button navigation task (ENG20009 Credit)
// Button 1: force an immediate manual log entry to the SD card.
// Button 2: toggle between the live data screen and the "System Info" screen
//           (total log count and free storage/status).
//
// Standalone test version: output goes to Serial so you can test the buttons now.
// Lines marked TODO are where main's real code (SD, LCD, sensors) plugs in.

//include other functions written by the other members...
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Arduino.h>
#include "interrupt_logging.h"
#include "sd_card_logging.h"

const uint8_t  log_BUTTON_PIN    = 2;    // manual log button      (check the schematic)
const uint8_t  toggle_BUTTON_PIN = 3;    // screen toggle button   (check the schematic)
const uint32_t DEBOUNCE_MS       = 30;


int buttonLogTime;
int buttonSwitchTime;


//Button 1 action
void manualLog() {
  if(buttonLogTime + 50 < millis()){ // debouncing
    buttonLogTime = millis();
    executeSDCardLogging(); // calls the logging function logs are handled there
  }

}
// Button 2 action
void switchScreen(){
  if(buttonSwitchTime + 50 < millis()){ // debouncing
    buttonSwitchTime = millis();
    liveDataScreen = !liveDataScreen; // toggles screen mode
    screenChanged = true; // tells the lcd screen to refresh
    
  }
}

// setup button interupts
void setupNavigationInterrupt(){
  pinMode(log_BUTTON_PIN, INPUT_PULLUP); // enables button 1
  attachInterrupt(digitalPinToInterrupt(log_BUTTON_PIN), manualLog, RISING); // attaches button 1 to manual log function
  pinMode(toggle_BUTTON_PIN, INPUT_PULLUP); // enables button 2
  attachInterrupt(digitalPinToInterrupt(toggle_BUTTON_PIN), switchScreen, RISING); // attaches button 2 to switch screens funciton
}


String getLogCount(){
  String logcount = String(int(getLineCount(file)));
  return String(logcount);
}

// gets the remaining free storage of the sdcard
String getFreeStorage(){

  String cachedFreeKB = String(int(getFreeBytes() / 1000)); // gets the value in bytes and converts to kb
  return String(cachedFreeKB);
}

//////////////////////////////////////////////////////////////
// Old code

// void drawSystemInfo() {
//   lcd.println("SYSTEM INFO");
//   lcd.print("Logs: ");  lcd.println(logCount);
//   lcd.print("Free: ");  lcd.print(cachedFreeKB);  lcd.println(" KB");
// }

// void setup() {
//   Serial.begin(115200);
//   logBtn.begin();
//   toggleBtn.begin();
// }

// void loop() {
//   if (logBtn.pressed()) manualLog();                 // Button 1

//   if (toggleBtn.pressed()) {                         // Button 2
//     currentScreen = (currentScreen == SCREEN_LIVE) ? SCREEN_SYSINFO : SCREEN_LIVE;
//     if (currentScreen == SCREEN_SYSINFO) cachedFreeKB = freeStorageKB();  // once per visit, can be slow
//     redrawNeeded = true;
//     // TODO: clear the LCD here before drawing the new screen
//   }

//   if (redrawNeeded) {                                // draw only when something changed
//     redrawNeeded = false;
//     if (currentScreen == SCREEN_LIVE) drawLiveScreen();
//     else                              drawSystemInfo();
//   }
// }


// defining all button components
// struct Button {
//   uint8_t  pin;
//   bool     rawLast;
//   bool     stable;
//   uint32_t tChange;

//   Button(uint8_t p) : pin(p), rawLast(false), stable(false), tChange(0) {} //define start states

//   void begin() { pinMode(pin, INPUT_PULLUP); }      // LOW = pressed

//   bool pressed() {                                  // true exactly once per press
//     bool raw = (digitalRead(pin) == LOW);
//     uint32_t now = millis();
//     if (raw != rawLast) { rawLast = raw; tChange = now; }
//     if ((now - tChange) >= DEBOUNCE_MS && raw != stable) {
//       stable = raw;
//       if (stable) return true;
//     }
//     return false;
//   }
// };