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

// Button logBtn(log_BUTTON_PIN);
// Button toggleBtn(toggle_BUTTON_PIN);

int buttonLogTime;
int buttonSwitchTime;

// ---------- 3. Navigation state ----------
enum Screen { SCREEN_LIVE, SCREEN_SYSINFO };
Screen   currentScreen = SCREEN_LIVE;
bool     redrawNeeded  = true;     // set this to true whenever new sensor values arrive
uint32_t logCount      = 0;
uint32_t cachedFreeKB  = 0;

// Print &lcd = Serial;               // TODO: replace with your real display object

uint32_t freeStorageKB() { return 0; }   // TODO: free space on the SD card, in KB


//Button 1 action
void manualLog() {
  if(buttonLogTime + 50 < millis()){
    buttonLogTime = millis();
    executeSDCardLogging();
  }
  // TODO: call your real logger here, e.g.
  //   saveLog(now, latest.temperature, latest.humidity, latest.pressure, latest.lux);
  // Serial.println("manual log");
  // logCount++;                      // only count it if the SD write succeeded
  // redrawNeeded = true;
}
void switchScreen(){
  if(buttonSwitchTime + 50 < millis()){
    buttonSwitchTime = millis();
    liveDataScreen = !liveDataScreen;
    screenChanged = true;
    
  }
}

void setupNavigationInterrupt(){
  // setup button interupts
  pinMode(log_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(log_BUTTON_PIN), manualLog, RISING);
  pinMode(toggle_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(toggle_BUTTON_PIN), switchScreen, RISING);
}

// printing on the LCD screen
// void drawLiveScreen() { //printing everything on the same line so it is all one line
//   lcd.print("Temperature: "); lcd.print(latest.temperature, 1);    lcd.println(" C");
//   lcd.print("Humidity: ");    lcd.print(latest.humidity, 1);       lcd.println(" %");
//   lcd.print("Pressure: ");    lcd.print(latest.pressure, 1);       lcd.println(" hPa");
//   lcd.print("Gas: ");         lcd.print(latest.gas_resistance, 1); lcd.println();
//   lcd.print("Lux: ");         lcd.print(latest.lux, 0);            lcd.println(" lx");
// }

String getLogCount(){
  String logcount = String(int(getLineCount(file)));
  return String(logcount);
}

String getFreeStorage(){

  String cachedFreeKB = String(int(getFreeBytes()));
  return String(cachedFreeKB);
}

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