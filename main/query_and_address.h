// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// TASK 2: Query and Change Address Module



void address_query() {
  Serial.print(sdiAddress);
  Serial.print("\r\n");
}

void change_address(int newAddr) {
  sdiAddress = newAddr;
  Serial.print(sdiAddress);
  Serial.print("\r\n");
}