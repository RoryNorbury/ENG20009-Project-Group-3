// Author: Joseph Cowell | Student ID: 104888857
// Communication to Serial Debug Assistant (SDA) function
#pragma once

void sendToSDA(String str) {
  digitalWrite(7, LOW); // set to WRITE
  Serial1.print(str + "!");
  Serial1.flush();      // wait until everything is sent
  while (Serial1.available()) {
    Serial1.read();     // throw away echo
  }
  digitalWrite(7, HIGH); // set to READ
}