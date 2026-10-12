#include <Arduino.h>
#include <Wire.h>

constexpr uint8_t directionPin = 7;

void scanI2C() {
  Serial.println("I2C scan:");
  bool found = false;

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      found = true;
      Serial.print("  found 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (!found) {
    Serial.println("  no I2C devices found");
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(directionPin, OUTPUT);
  digitalWrite(directionPin, HIGH); // UART board receive mode
  Serial1.begin(1200, SERIAL_7E1);

  Serial.println();
  Serial.println("SDI-12 diagnostic ready");
  Serial.println("Serial1: 1200 7E1, DIRO: HIGH");
  scanI2C();
  Serial.println("Listening for raw Serial1 bytes...");
}

void loop() {
  while (Serial1.available()) {
    uint8_t value = static_cast<uint8_t>(Serial1.read());

    Serial.print("RX 0x");
    if (value < 16) {
      Serial.print("0");
    }
    Serial.print(value, HEX);
    Serial.print(" (");
    if (value >= 32 && value <= 126) {
      Serial.write(value);
    } else {
      Serial.print("non-printable");
    }
    Serial.println(")");
  }
}
