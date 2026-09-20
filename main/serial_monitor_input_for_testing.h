// Author: NOAH MUNRO | 105912100
// FOR TESTING ONLY !!!! ------------
// very useful if you would like to test your functions without needing actual SDI-12 Input
// will return a string

// Example use:
// value = serial_test_input_string();

// will not be part of final program
// makes use of a while loop, which will effectivly pause the program, so may break things

String serial_test_input_string(){
  Serial.println("");
  Serial.println("Enter String: ");
  Serial.print("> ");
  while (Serial.available() == 0) {} // waits for a value in the Serial monitor
  // Needs a gap here or it breaks lol
  String string = Serial.readString(); // reads string from Serial Monitor
  string.trim(); //removes trailing whitespace
  Serial.println(string); 
  return string;
}