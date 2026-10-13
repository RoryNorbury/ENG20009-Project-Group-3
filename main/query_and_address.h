// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// TASK 2: Query and Change Address Module

String address_query() {
  return String(sdiAddress) + "\r\n";
}

String change_address(int newAddr) {
  if (newAddr >= 0 && newAddr <= 9) {
    sdiAddress = newAddr;
    return String(sdiAddress) + "\r\n";
  }
  return"";
}