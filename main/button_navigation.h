//Hannah James || 105339013 - Button Navigation
//include other functions written by the other members...
#include <Arduino.h>
#include "periodic_logging.h"
#include "LCD.h"
#include <SdFat.h>  //provides the SdFs type, which is needed to ask the SD card for free space

extern SdFs sd; 
extern float data [2][4];

const uint8_t  log_BUTTON_PIN    = 2;    // manual log button
const uint8_t  toggle_BUTTON_PIN = 3;    // screen toggle button
const uint32_t DEBOUNCE_MS = 30;

const uint32_t LIVE_REFRESH_MS = 1000;
uint32_t lastDrawMs = 0;


// defining all button components
struct Button {
  uint8_t  pin;
  bool     rawLast;
  bool     stable;
  uint32_t tChange;

  //starting values for the button after being pressed
  Button(uint8_t p) : pin(p), rawLast(false), stable(false), tChange(0) {}

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

//defining buttons - each has all factors defined before
Button logBtn(log_BUTTON_PIN);
Button toggleBtn(toggle_BUTTON_PIN);

//printing on the screen
enum Screen { SCREEN_LIVE, SCREEN_SYSINFO };
Screen   currentScreen = SCREEN_LIVE;
bool     redrawNeeded  = true;     // set this to true whenever new sensor values arrive
uint32_t logCount      = 0;
uint32_t cachedFreeKB  = 0;

Print &lcd = tft;

//how many unused clusters does the SD card have?
int32_t freeStorageKB() { //means it will return numbers - void returns nothing
int32_t freeClusters = sd.vol()->freeClusterCount();   // -1 means a read error
if (freeClusters < 0) return 0;
uint64_t bytes = (uint64_t)freeClusters * sd.vol()->bytesPerCluster();
return (uint32_t)(bytes / 1024);                       // KB
}

//Button 1 action
void manualLog() {
  logSensordata();
  Serial.println("manual log");
  logCount++;                      // only count it if the SD write succeeded
  redrawNeeded = true;
}

//PRINT THE LCD USING LCD.h            // global...
   void printLcdScreen() {
     updateLCD(data);
}

void drawSystemInfo() {
  tft.setCursor(0, 0); //cursor top left
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  lcd.println("SYSTEM INFO");
  lcd.print("Logs: ");  lcd.println(logCount);
  lcd.print("Free: ");  lcd.print(cachedFreeKB);  lcd.println(" KB");
}


void buttonSetup() { //NEEDS TO BE CALLED IN MAIN --
  Serial.begin(115200);
  logBtn.begin();
  toggleBtn.begin();
}

void buttonLoop() { //NEEDS TO BE CALLED IN MAIN -- apparently...
  if (logBtn.pressed()) manualLog();                 // Button 1

  if (toggleBtn.pressed()) {                         // Button 2
    currentScreen = (currentScreen == SCREEN_LIVE) ? SCREEN_SYSINFO : SCREEN_LIVE;
    if (currentScreen == SCREEN_SYSINFO) cachedFreeKB = freeStorageKB();  // once per visit, can be slow
    tft.fillScreen(ST77XX_BLACK);
    redrawNeeded = true;
  }

  if (redrawNeeded || (millis() - lastDrawMs >= LIVE_REFRESH_MS)) {                                // draw only when something changed
    redrawNeeded = false;
    lastDrawMs = millis();
    if (currentScreen == SCREEN_LIVE) printLcdScreen();
    else                              drawSystemInfo();
  }
}