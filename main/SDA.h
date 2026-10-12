void sendToSDI(String str) {
  digitalWrite(7, LOW); // set to WRITE
  Serial1.print(str + "!");
  Serial1.flush();      // wait until everything is sent
  while (Serial1.available()) {
    Serial1.read();     // throw away echo
  }
  digitalWrite(7, HIGH); // set to READ
}