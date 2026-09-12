#include <Wire.h>
#include <math.h>

#define WIRE_SCL_PIN PB10
#define WIRE_SDA_PIN PB11

TwoWire Wire(WIRE_SCL_PIN, WIRE_SDA_PIN, WIRE_DELAY);

const uint8_t MPU_ADDR = 0x68;

// Accelerometer sensitivity (2g mode)
#define ACCEL_SENSITIVITY 16384.0

// Calibration offsets (should be measured with the sensor level)
float accOffsetX = 0;
float accOffsetY = 0;
float accOffsetZ = 0;

void calibrateMPU6050() {
  Serial.println("Calibration is starting... Hold the sensor level.");
  delay(3000);
  
  float sumX = 0, sumY = 0, sumZ = 0;
  int samples = 100;
  
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, (uint8_t)6);
    
    int16_t rawX = (Wire.read() << 8) | Wire.read();
    int16_t rawY = (Wire.read() << 8) | Wire.read();
    int16_t rawZ = (Wire.read() << 8) | Wire.read();
    
    sumX += rawX;
    sumY += rawY;
    sumZ += rawZ;
    delay(10);
  }
  
  // Ortalamaları al ve offset'leri hesapla
  // Z ekseni yaklaşık 1g (16384) göstermeli
  accOffsetX = sumX / samples;
  accOffsetY = sumY / samples;
  accOffsetZ = (sumZ / samples) - ACCEL_SENSITIVITY;  // 1g çıkar
  
  Serial.print("Kalibrasyon tamam. Offsetler: X=");
  Serial.print(accOffsetX);
  Serial.print(" Y=");
  Serial.print(accOffsetY);
  Serial.print(" Z=");
  Serial.println(accOffsetZ);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // MPU6050'i uyandır
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  // Accelerometer'ı +/- 2g moduna ayarla
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  // Kalibrasyon yap
  calibrateMPU6050();
  
  Serial.println("X(g)\tY(g)\tZ(g)\tPitch(°)\tRoll(°)");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, (uint8_t)6);
  
  // Ham değerleri oku
  int16_t rawX = (Wire.read() << 8) | Wire.read();
  int16_t rawY = (Wire.read() << 8) | Wire.read();
  int16_t rawZ = (Wire.read() << 8) | Wire.read();
  
  // Offset çıkar ve g'ye çevir
  float accX = (rawX - accOffsetX) / ACCEL_SENSITIVITY;
  float accY = (rawY - accOffsetY) / ACCEL_SENSITIVITY;
  float accZ = (rawZ - accOffsetZ) / ACCEL_SENSITIVITY;
  
  // Pitch ve Roll açılarını hesapla
  float pitch = atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180.0 / PI;
  float roll = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180.0 / PI;
  
  // Sonuçları yazdır
  Serial.print(accX, 3); Serial.print("\t");
  Serial.print(accY, 3); Serial.print("\t");
  Serial.print(accZ, 3); Serial.print("\t");
  Serial.print(pitch, 1); Serial.print("\t\t");
  Serial.print(roll, 1); Serial.println();
  
  delay(100);
}