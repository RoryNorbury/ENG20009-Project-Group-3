volatile bool interruptLoggingFlag = false;
const int logChipSelect = 10;

void setupInterruptLogging() {
  if (!SD.begin(logChipSelect)) {
    Serial.println("SD Card initialization failed for Interrupt Logging.");
  } else {
    Serial.println("SD Card ready for Interrupt Logging.");
  }

  Timer3.attachInterrupt(vTimerLoggingISR);
  Timer3.start(30000000);
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

void executeSDCardLogging() {
  extern float smoothedTemp;
  extern float smoothedHumidity;
  extern float smoothedPressure;
  extern float smoothedGas;
  extern float smoothedLux;
  extern String currentTimestamp;

  File logFile = SD.open("LOG.CSV", FILE_WRITE);
  if (logFile) {
    logFile.print(currentTimestamp);
    logFile.print(",");
    logFile.print(smoothedTemp);
    logFile.print(",");
    logFile.print(smoothedHumidity);
    logFile.print(",");
    logFile.print(smoothedPressure);
    logFile.print(",");
    logFile.print(smoothedGas);
    logFile.print(",");
    logFile.println(smoothedLux);
    logFile.close();
    Serial.println("Data appended to LOG.CSV via Interrupt.");
  } else {
    Serial.println("Could not open LOG.CSV for writing.");
  }
}