// Wiring test: prints every I2C device it finds. MPU6050 should show 0x68 (or 0x69).
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("ESP32 alive");
  Wire.begin(8, 9);  // SDA=8, SCL=9
}

void loop() {
  int found = 0;
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at 0x");
      Serial.println(addr, HEX);
      found++;
    }
  }
  if (!found) Serial.println("No I2C device found - check wiring");
  delay(2000);
}
