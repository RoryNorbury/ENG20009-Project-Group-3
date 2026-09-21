// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// TASK 2: Query and Change Address Module

int sdiAddress = 0;

void address_query() {
  Serial.print(sdiAddress);
  Serial.print("\r\n");
}

void change_address(int currentAddr, int newAddr) {
  if (currentAddr != sdiAddress) return;

  if (newAddr >= 0 && newAddr <= 9) {
    sdiAddress = newAddr;
    Serial.print(sdiAddress);
    Serial.print("\r\n");
  }
}