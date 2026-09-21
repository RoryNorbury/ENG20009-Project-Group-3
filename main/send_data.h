//Author: Hannah James || Student ID: 105339013
//SEND DATA FUNCTION

void send_data(int dIndex) {
  if (!dataReady) {
    Serial.print(sdiAddress);
    Serial.print("\r\n");
    return;
  }

  if (dIndex == 0) {
    String out = String(sdiAddress);

    for (int i = 0; i < NUM_VALUES; i++) {
      if (lastValues[i] >= 0) out += "+";
      out += String(lastValues[i], 1);
    }

    Serial.print(out);
    Serial.print("\r\n");
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