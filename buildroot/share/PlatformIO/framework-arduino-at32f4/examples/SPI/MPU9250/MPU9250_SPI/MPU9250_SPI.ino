#include "SPI.h"

SPIClass SPI(SPI2);

#define MPU9250_ADDRESS            0x68
#define MPU9250_WHO_AM_I           0x75
#define MPU9250_PWR_MGMT_1         0x6B
#define MPU9250_CONFIG             0x1A
#define MPU9250_GYRO_CONFIG        0x1B
#define MPU9250_ACCEL_CONFIG       0x1C
#define MPU9250_ACCEL_CONFIG2      0x1D
#define MPU9250_INT_PIN_CFG        0x37
#define MPU9250_ACCEL_XOUT_H       0x3B
#define MPU9250_GYRO_XOUT_H        0x43

const uint8_t MPU9250_CS_PIN = PB12;

// Calibration Variables
float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;

// Filtered Final Angles (for Drone Control Loop)
float pitch = 0, roll = 0; 
unsigned long lastTime = 0;

// Drone Filter Coefficient (98% Gyroscope to completely eliminate motor vibrations)
const float alpha = 0.98; 

uint8_t spiTransfer(uint8_t reg, uint8_t data, bool read = false) {
    uint8_t result;
    uint8_t txByte = read ? (reg | 0x80) : reg;
    digitalWrite(MPU9250_CS_PIN, LOW);
    SPI.transfer(txByte);
    result = SPI.transfer(data);
    digitalWrite(MPU9250_CS_PIN, HIGH);
    delayMicroseconds(10);
    return result;
}

void writeRegister(uint8_t reg, uint8_t value) {
    spiTransfer(reg, value, false);
    delay(10);
}

uint8_t readRegister(uint8_t reg) {
    return spiTransfer(reg, 0x00, true);
}

void readRegisters(uint8_t reg, uint8_t* data, uint8_t length) {
    digitalWrite(MPU9250_CS_PIN, LOW);
    SPI.transfer(reg | 0x80);
    for (uint8_t i = 0; i < length; i++) {
        data[i] = SPI.transfer(0x00);
    }
    digitalWrite(MPU9250_CS_PIN, HIGH);
    delayMicroseconds(10);
}

int16_t read16BitRegister(uint8_t regHigh) {
    uint8_t buffer[2];
    readRegisters(regHigh, buffer, 2);
    return (int16_t)((buffer[0] << 8) | buffer[1]);
}

bool initializeMPU9250() {
    Serial.println("Initializing MPU9250 via SPI...");
    uint8_t whoami = readRegister(MPU9250_WHO_AM_I);
    if (whoami != 0x71 && whoami != 0x73) {
        Serial.println("CRITICAL ERROR: MPU9250 missing.");
        return false;
    }
    
    writeRegister(MPU9250_PWR_MGMT_1, 0x80);  
    delay(100);
    writeRegister(MPU9250_PWR_MGMT_1, 0x01);  // The most reliable time source: Automatic gyroscope clock.
    delay(100);
    
   // IMPORTANT FOR DRONE: Digital Low-Pass Filter (DLPF) Setting
   // Hardware dampens motor vibrations by setting the gyro bandwidth to ~41Hz and the accelerometer to ~45Hz.

    writeRegister(MPU9250_CONFIG, 0x03);       
    writeRegister(MPU9250_ACCEL_CONFIG2, 0x03);

    writeRegister(MPU9250_GYRO_CONFIG, 0x08);  // ±500 dps (Sensibility: 65.5 LSB/dps)
    writeRegister(MPU9250_ACCEL_CONFIG, 0x08); // ±4g (Sensibility: 8192 LSB/g)
    writeRegister(MPU9250_INT_PIN_CFG, 0x02);  
    delay(100);
    return true;
}

void calibrateMPU9250(int samples = 1000) { // We increased the sample count for the drone to 1000.
    Serial.println("\n[KALİBRASYON] Sensörü düz bir zeminde tamamen HAREKETSİZ bırakın!");
    delay(3000);
    long axSum = 0, aySum = 0, azSum = 0;
    long gxSum = 0, gySum = 0, gzSum = 0;
    
    for (int i = 0; i < samples; i++) {
        axSum += read16BitRegister(MPU9250_ACCEL_XOUT_H);
        aySum += read16BitRegister(MPU9250_ACCEL_XOUT_H + 2);
        azSum += read16BitRegister(MPU9250_ACCEL_XOUT_H + 4);
        gxSum += read16BitRegister(MPU9250_GYRO_XOUT_H);
        gySum += read16BitRegister(MPU9250_GYRO_XOUT_H + 2);
        gzSum += read16BitRegister(MPU9250_GYRO_XOUT_H + 4);
        delayMicroseconds(2000); // 2ms aralıklarla hızlı örnekleme
    }
    
    accelOffsetX = axSum / samples;
    accelOffsetY = aySum / samples;
    accelOffsetZ = (azSum / samples) - 8192; // Gravity balance (1g) on ​​a scale of ±4g
    
    gyroOffsetX = gxSum / samples;
    gyroOffsetY = gySum / samples;
    gyroOffsetZ = gzSum / samples;
    Serial.println("[OK] Calibration is complete. The motors can now operate.");
}

void readSensorData() {
    // Calculate cycle time (dt) with microsecond precision.
    unsigned long currentTime = micros();
    float dt = (currentTime - lastTime) / 1000000.0; 
    lastTime = currentTime;
    if (dt <= 0 || dt > 0.1) dt = 0.004; // Firewall (e.g., for a 250Hz loop)

    // 1. Accelerometer Data (Reset and converted to g units)
    float accelX = (read16BitRegister(MPU9250_ACCEL_XOUT_H) - accelOffsetX) / 8192.0;
    float accelY = (read16BitRegister(MPU9250_ACCEL_XOUT_H + 2) - accelOffsetY) / 8192.0;
    float accelZ = (read16BitRegister(MPU9250_ACCEL_XOUT_H + 4) - accelOffsetZ) / 8192.0;
    
    // Geometric Angle Formulas Compliant with Drone Aviation Standards
    float pitchAcc = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0 / M_PI;
    float rollAcc  = atan2(accelY, accelZ) * 180.0 / M_PI; // Düzeltilen formül

    // 2. Gyroscope Data (Reset and converted to dps/degrees)
    float gyroX = (read16BitRegister(MPU9250_GYRO_XOUT_H) - gyroOffsetX) / 65.5;
    float gyroY = (read16BitRegister(MPU9250_GYRO_XOUT_H + 2) - gyroOffsetY) / 65.5;

    // 3. Drone Type Heavy Duty Complementary Filter
    // It takes 98% of the angle accurately from the gyroscope to filter out vibrations, and equates the remaining 2% with the accelerometer.
    pitch = alpha * (pitch + gyroY * dt) + (1.0 - alpha) * pitchAcc;
    roll  = alpha * (roll + gyroX * dt) + (1.0 - alpha) * rollAcc;

    // Stable outputs that you will feed to the PID controller:
    Serial.print("P:"); Serial.print(pitch, 2);
    Serial.print(" R:"); Serial.println(roll, 2);
}

void setup() {
    Serial.begin(115200);
    pinMode(MPU9250_CS_PIN, OUTPUT);
    digitalWrite(MPU9250_CS_PIN, HIGH);
    
    SPI.begin();
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0)); 
    
    if (!initializeMPU9250()) {
        Serial.println(" MPU9250 Not initialize");
        while (1); 
    }
    
    calibrateMPU9250();
    lastTime = micros(); // Start the timer.
}

void loop() {
    readSensorData();    
    delay(4); // ~250Hz loop rate (ideal standard for drone controllers)
}
