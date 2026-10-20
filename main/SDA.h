// Author: Joseph Cowell | Student ID: 104888857
// Communication to Serial Debug Assistant (SDA) function
#pragma once

void sendToSDA(String str) {
  digitalWrite(8, LOW); // set to WRITE
  Serial1.print(str + "\r\n");
  Serial1.flush(); // wait until everything is sent
  Serial1.end(); // restart serial to avoid echo
  Serial1.begin(1200, SERIAL_7E1);
  digitalWrite(8, HIGH); // set to READ
}