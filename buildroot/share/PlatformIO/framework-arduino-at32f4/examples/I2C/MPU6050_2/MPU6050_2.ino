#include <Wire.h>

#define WIRE_SCL_PIN PB10
#define WIRE_SDA_PIN PB11

TwoWire Wire(WIRE_SCL_PIN, WIRE_SDA_PIN, WIRE_DELAY);

const uint8_t MPU_ADDR = 0x68;
#define ACCEL_SENSITIVITY 16384.0

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  Serial.println("MPU6050 - Matematik Opsiyonlari Olmadan");
  Serial.println("X(g)\tY(g)\tZ(g)\tAci Tahmini");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);
  
  int16_t rawX = (Wire.read() << 8) | Wire.read();
  int16_t rawY = (Wire.read() << 8) | Wire.read();
  int16_t rawZ = (Wire.read() << 8) | Wire.read();
  
  // g'ye çevir
  float gX = rawX / ACCEL_SENSITIVITY;
  float gY = rawY / ACCEL_SENSITIVITY;
  float gZ = rawZ / ACCEL_SENSITIVITY;
  
  // Basit açı hesaplama (matematik fonksiyonu olmadan)
  // Küçük açı yaklaşımı: açı ≈ sin(açı) ≈ gY/gZ veya -gX/gZ
  int pitch = 0, roll = 0;
  
  if (gZ > 0.1) {  // Z pozitif ise
    // -90 ila +90 derece arası
    pitch = (gY / gZ) * 57.3;  // 57.3 = 180/π yaklaşık
    roll = (-gX / gZ) * 57.3;
    
    // Sınırla
    if (pitch > 90) pitch = 90;
    if (pitch < -90) pitch = -90;
    if (roll > 90) roll = 90;
    if (roll < -90) roll = -90;
  }
  
  // Durum tespiti
  String orientation = "Duz";
  
  if (gZ > 0.8) {
    if (abs(gX) < 0.2 && abs(gY) < 0.2) orientation = "Duz";
    else if (gX < -0.7) orientation = "Saga Yatik";
    else if (gX > 0.7) orientation = "Sola Yatik";
    else if (gY > 0.7) orientation = "One Egik";
    else if (gY < -0.7) orientation = "Arka Egik";
  } else if (gZ < 0.3) {
    orientation = "Dik (yatay)";
  }
  
  Serial.print(gX, 3); Serial.print("\t");
  Serial.print(gY, 3); Serial.print("\t");
  Serial.print(gZ, 3); Serial.print("\t");
  Serial.print(orientation);
  Serial.print(" Pitch:"); Serial.print(pitch);
  Serial.print(" Roll:"); Serial.println(roll);
  
  delay(100);
}