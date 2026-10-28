#include <Wire.h>

#define SDA_PIN 8
#define SCL_PIN 9
#define MPU_ADDR 0x68

float pitch = 0, roll = 0;       // filtered angles (degrees)
float pitchOff = 0, rollOff = 0; // flat-hand calibration
unsigned long lastUs;

void mpuWrite(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

bool readMPU(float &ax, float &ay, float &az, float &gx, float &gy) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU_ADDR, 14) != 14) return false;
  int16_t v[7];
  for (int i = 0; i < 7; i++) {
    v[i] = (Wire.read() << 8) | Wire.read();
  }
  ax = v[0] / 16384.0; ay = v[1] / 16384.0; az = v[2] / 16384.0; // g
  gx = v[4] / 131.0;   gy = v[5] / 131.0;                         // deg/s
  return true;
}

void calibrate() {
  // Keep hand FLAT and still while this runs
  float sp = 0, sr = 0;
  const int N = 200;
  for (int i = 0; i < N; i++) {
    float ax, ay, az, gx, gy;
    if (readMPU(ax, ay, az, gx, gy)) {
      sr += atan2(ay, az) * 180.0 / PI;
      sp += atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    }
    delay(5);
  }
  rollOff = sr / N;
  pitchOff = sp / N;
  pitch = 0; roll = 0;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  mpuWrite(0x6B, 0x00); // wake up MPU6050
  mpuWrite(0x1A, 0x03); // low-pass filter
  delay(2000);          // time to put hand flat
  calibrate();
  lastUs = micros();
}

void loop() {
  // Send 'c' from PC to recalibrate
  if (Serial.available() && Serial.read() == 'c') calibrate();

  float ax, ay, az, gx, gy;
  if (!readMPU(ax, ay, az, gx, gy)) return;

  unsigned long now = micros();
  float dt = (now - lastUs) / 1e6;
  lastUs = now;

  float accRoll  = atan2(ay, az) * 180.0 / PI - rollOff;
  float accPitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI - pitchOff;

  // complementary filter (smooth + no drift)
  roll  = 0.96 * (roll  + gx * dt) + 0.04 * accRoll;
  pitch = 0.96 * (pitch + gy * dt) + 0.04 * accPitch;

  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 20) { // 50 Hz
    lastSend = millis();
    Serial.print(pitch, 1);
    Serial.print(",");
    Serial.println(roll, 1);
  }
}