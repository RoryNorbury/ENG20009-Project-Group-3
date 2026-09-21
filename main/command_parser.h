// Author: NOAH MUNRO | 105912100
// Function for parsing the SDI-12 Commands
// Example use:
// string = interpret_command(command);
// Function takes in a command string
// Function outputs string for testing 
// first character of output string is either a 0 or 1 for any functions that need a bool

// each section with ------------------------
// requires a function for performing the commanded task
// you can use the provied funciton name and simply uncomment the line
// or you can name it what you want and change the line

#include "send_data.h"
#include "start_measurement.h"
#include "query_and_address.h"

// enums for interpret_command
enum Command_List {
  ADDRESS_QUERY_L = 63,     // Decimal ASCII for "?"
  CHANGE_ADDRESS_L = 65,    // Decimal ASCII for "A"
  START_MEASUREMENT_L = 77, // Decimal ASCII for "M"
  SEND_DATA_L = 68,         // Decimal ASCII for "D"
  SEND_ID_L = 73            // Decimal ASCII for "I"
};

// Parse Command Function
String interpret_command(String Command){
  // checks if the command ends with "!"
  if (!(Command.endsWith("!"))){
    return "0 - Invalid Command Missing \"!\"";
  }
  // Checks if there is whitespace in the command
  for (int i = 0; i < Command.length(); i++){
    if (int(Command[i]) == 32){
      return "0 - Invalid Command";
    }
  }
  // Address Query Condition
  if ((Command[0] == ADDRESS_QUERY_L) && (Command.length() == 2)) {
    // Address Query Function goes here ----------------------------------
    // Example:
    address_query();
    return "1 - Address Query";
  }
  int command_address = Command[0] - 48; // Gets Address from Command String
  int command_value = Command[2] - 48; // Gets Command Value from Command String
  // Checks if the Command Address is within range
  if (command_address != sdiAddress) {
    return "0 - Invalid Address";
  }
  // checks if the Command Value is within range if applicable 
  if (Command.length() == 4){
    if ((command_value < 0) || (command_value > 9)){
      return "0 - Invalid Command";
    }
  }
  // switch case for the rest of the commands 
  switch (int(Command[1])){
    case CHANGE_ADDRESS_L:
      // Change Address Function goes here --------------------------------
      // Example:
      change_address(command_value);
      Serial.print("Change Address ");
      Serial.print(" To: ");
      Serial.println(command_value);
      return "1 - Change Address";
      break;
    case START_MEASUREMENT_L:
      // Start Measurement Function goes here ------------------------------
      // Example:
      start_measurement();
      Serial.print("Start Measurement");
      Serial.println(command_address);
      return "1 - Start Measurement";
      break;
    case SEND_DATA_L:
      // Send Data Function goes here -------------------------------
      // Example:
      send_data(command_value);
      Serial.print("Send Data ");
      Serial.print(" Data Selected: ");
      Serial.println(command_value);
      return "1 - Send Data";
      break;
    case SEND_ID_L:
      // Send Identification Function goes here --------------------------
      // Example:
      // send_identification(command_address);
      return "1 - Send Identification";
      break;
    default:
      return "0 - Invalid Command";
  }
}
