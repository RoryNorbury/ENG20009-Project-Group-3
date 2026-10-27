//Author: Hannah James || Student ID: 105339013
//SEND DATA FUNCTION
#pragma once

// takes in the index for the set of data to be sent
String send_data(int dIndex) {
  // debug message for if the data is not ready
  if (!dataReady) {
    Serial.print(sdiAddress);
    Serial.print("\r\n");
    return String(sdiAddress);
  }

  // checks if the index is within the range of sets for our application
  if (dIndex <= 1) {
    String out = String(sdiAddress); // Starts the string with the sdi address

    // for loop running through all the values in the selected data set
    for (int i = 0; i < sizeof(lastValues[dIndex]) / sizeof(lastValues[0][0]); i++) {
      if (lastValues[dIndex][i] >= 0){
        out += "+"; // adds a "+" between every term in the string
        out += String(lastValues[dIndex][i], 1); // adds a single measurement to the string
      } 
    }

    Serial.print(out);
    Serial.print("\r\n");
    return out; // returns the data as a string
  } 
  else {
    Serial.print(sdiAddress);
    Serial.print("\r\n");
  }
}

// Commented out so it doesn't mess with the main setup and loop functions - RORY

// void setup() {
//   Serial.begin(9600);
// }

// //loop was to test it
// void loop() {
//   if (Serial.available()) {
//     String cmd = Serial.readStringUntil('\n');
//     cmd.trim();
//     Serial.println(interpret_command(cmd));
//   }
// }