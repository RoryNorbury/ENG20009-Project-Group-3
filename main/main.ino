// used by other files, so must be declared before
int sdiAddress = '0';
bool dataReady = false;
int NUM_VALUES = 5;
double lastValues[5] = {0, 0, 0, 0, 0};

#include "command_parser.h"
#include "serial_monitor_input_for_testing.h"

// variable declarations here


void setup()
{
    // setup here
    Serial.begin(9600);
}

void loop()
{
    // main code here
    while (1)
    {
        String str = serial_test_input_string();
        String ret = interpret_command(str);
        Serial.println(ret);
    }
}