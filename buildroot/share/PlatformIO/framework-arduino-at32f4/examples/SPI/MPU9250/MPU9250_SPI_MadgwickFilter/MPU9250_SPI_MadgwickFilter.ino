#include "SPI.h"

// MPU9250 Register Adresleri
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
#define MPU9250_MAG_ADDRESS        0x0C


// Chip Select (CS) Pini
const uint8_t MPU9250_CS_PIN = PA4;

SPIClass SPI(SPI1);
//SPIClass SPI(SPI2);
//SPIClass SPI(SPI3);


// Açılar (derece cinsinden)
float pitch = 0, roll = 0, yaw = 0;
float pitchAcc = 0, rollAcc = 0;

// Jiroskop entegrasyonu için
float pitchGyro = 0, rollGyro = 0, yawGyro = 0;
unsigned long lastTime = 0;



// SPI transfer fonksiyonu (Read/Write bit dahil)
uint8_t spiTransfer(uint8_t reg, uint8_t data, bool read = false) {
    uint8_t result;
    
    // MPU9250 SPI formatı: 
    // Read için: 0x80 | register
    // Write için: register (MSB = 0)
    uint8_t txByte = read ? (reg | 0x80) : reg;
    
    digitalWrite(MPU9250_CS_PIN, LOW);
    SPI.transfer(txByte);
    result = SPI.transfer(data);
    digitalWrite(MPU9250_CS_PIN, HIGH);
    
    delayMicroseconds(10);
    return result;
}

// Register yazma
void writeRegister(uint8_t reg, uint8_t value) {
    spiTransfer(reg, value, false);
    delay(10);
}

// Register okuma
uint8_t readRegister(uint8_t reg) {
    return spiTransfer(reg, 0x00, true);
}

// Birden çok byte okuma
void readRegisters(uint8_t reg, uint8_t* data, uint8_t length) {
    digitalWrite(MPU9250_CS_PIN, LOW);
    SPI.transfer(reg | 0x80);  // Read bit set
    
    for (uint8_t i = 0; i < length; i++) {
        data[i] = SPI.transfer(0x00);
    }
    
    digitalWrite(MPU9250_CS_PIN, HIGH);
    delayMicroseconds(10);
}

// MPU9250 başlatma
bool initializeMPU9250() {
    Serial.println("MPU9250 SPI Başlatılıyor...");
    
    // 1. WHO_AM_I kontrolü
    uint8_t whoami = readRegister(MPU9250_WHO_AM_I);
    Serial.print("WHO_AM_I: 0x");
    Serial.println(whoami, HEX);
    
    if (whoami != 0x71 && whoami != 0x73) {
        Serial.println("HATA: MPU9250 bulunamadı!");
        return false;
    }
    
    // 2. Power Management - Reset ve Clock Source
    writeRegister(MPU9250_PWR_MGMT_1, 0x80);  // Device Reset
    delay(100);
    writeRegister(MPU9250_PWR_MGMT_1, 0x01);  // Auto select clock source
    delay(100);
    
    // 3. Konfigürasyonlar
    writeRegister(MPU9250_CONFIG, 0x03);      // DLPF_CFG = 3 (Gyro: 41Hz, Accel: 44Hz)
    
    // 4. Gyro ölçek faktörü (±500 dps)
    writeRegister(MPU9250_GYRO_CONFIG, 0x08); // FS_SEL = 1 (500 dps)
    
    // 5. Accel ölçek faktörü (±4g)
    writeRegister(MPU9250_ACCEL_CONFIG, 0x08); // AFS_SEL = 1 (4g)
    
    // 6. Accel DLPF
    writeRegister(MPU9250_ACCEL_CONFIG2, 0x03); // Accel DLPF_CFG = 3 (44Hz)
    
    // 7. Bypass Mode (Manyetometreye erişim için)
    writeRegister(MPU9250_INT_PIN_CFG, 0x02); // BYPASS_EN = 1
    delay(100);
    
    // 8. Manyetometre başlatma (isteğe bağlı)
    // initializeMagnetometer();
    
    Serial.println("MPU9250 başarıyla başlatıldı!");
    return true;
}

// 16-bit veriyi oku (signed)
int16_t read16BitRegister(uint8_t regHigh) {
    uint8_t buffer[2];
    readRegisters(regHigh, buffer, 2);
    return (int16_t)((buffer[0] << 8) | buffer[1]);
}

// Kalibrasyon için offset değerleri
float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;

void calibrateMPU9250(int samples = 500) {
    Serial.println("\nKalibrasyon başlıyor... Sensörü HAREKETSİZ tutun!");
    delay(3000);
    
    long axSum = 0, aySum = 0, azSum = 0;
    long gxSum = 0, gySum = 0, gzSum = 0;
    
    for (int i = 0; i < samples; i++) {
        int16_t ax = read16BitRegister(MPU9250_ACCEL_XOUT_H);
        int16_t ay = read16BitRegister(MPU9250_ACCEL_XOUT_H + 2);
        int16_t az = read16BitRegister(MPU9250_ACCEL_XOUT_H + 4);
        
        int16_t gx = read16BitRegister(MPU9250_GYRO_XOUT_H);
        int16_t gy = read16BitRegister(MPU9250_GYRO_XOUT_H + 2);
        int16_t gz = read16BitRegister(MPU9250_GYRO_XOUT_H + 4);
        
        axSum += ax; aySum += ay; azSum += az;
        gxSum += gx; gySum += gy; gzSum += gz;
        
        delay(5);
    }
    
    // Z ekseni offseti (1g = 8192 LSB)
    accelOffsetZ = (azSum / samples) - 8192;
    accelOffsetX = axSum / samples;
    accelOffsetY = aySum / samples;
    
    gyroOffsetX = gxSum / samples;
    gyroOffsetY = gySum / samples;
    gyroOffsetZ = gzSum / samples;
    
    Serial.println("\nKalibrasyon Tamamlandı!");
    Serial.print("Accel Offset - X:"); Serial.print(accelOffsetX);
    Serial.print(" Y:"); Serial.print(accelOffsetY);
    Serial.print(" Z:"); Serial.println(accelOffsetZ);
    
    Serial.print("Gyro Offset  - X:"); Serial.print(gyroOffsetX);
    Serial.print(" Y:"); Serial.print(gyroOffsetY);
    Serial.print(" Z:"); Serial.println(gyroOffsetZ);
    delay(7000);
}

// Sensör verilerini oku
void readSensorData() {
    // 3 eksen ivmeölçer verileri
    int16_t ax_raw = read16BitRegister(MPU9250_ACCEL_XOUT_H);
    int16_t ay_raw = read16BitRegister(MPU9250_ACCEL_XOUT_H + 2);
    int16_t az_raw = read16BitRegister(MPU9250_ACCEL_XOUT_H + 4);
    
    // 3 eksen jiroskop verileri
    int16_t gx_raw = read16BitRegister(MPU9250_GYRO_XOUT_H);
    int16_t gy_raw = read16BitRegister(MPU9250_GYRO_XOUT_H + 2);
    int16_t gz_raw = read16BitRegister(MPU9250_GYRO_XOUT_H + 4);
    
    // Sıcaklık (isteğe bağlı)
    int16_t temp = read16BitRegister(MPU9250_ACCEL_XOUT_H + 6);
    
    float ax = ax_raw - accelOffsetX;
    float ay = ay_raw - accelOffsetY;
    float az = az_raw - accelOffsetZ;

    float gx = gx_raw - gyroOffsetX;
    float gy = gy_raw - gyroOffsetY;
    float gz = gz_raw - gyroOffsetZ;

    // Ölçek faktörlerine göre dönüşüm
    // Accel: ±4g için 8192 LSB/g
    float accelX = ax / 8192.0;
    float accelY = ay / 8192.0;
    float accelZ = az / 8192.0;
    
    // Gyro: ±500 dps için 65.5 LSB/dps
    float gyroX = gx / 65.5;
    float gyroY = gy / 65.5;
    float gyroZ = gz / 65.5;
    
    // Sıcaklık dönüşümü
    float temperature = (temp / 333.87) + 21.0;
    

    float pitchAcc = atan2(-accelX, sqrt(accelY*accelY + accelZ*accelZ)) * 180.0 / M_PI;
    float rollAcc = atan2(accelY, sqrt(accelX*accelX + accelZ*accelZ)) * 180.0 / M_PI;

    // Sonuçları serial monitöre yazdır
   
    Serial.print("Pitch: "); Serial.print(pitchAcc);
    Serial.print(" Roll: "); Serial.println(rollAcc);
 
}




// Önceki SPI fonksiyonlarınız burada olacak...
// (spiTransfer, readRegister, read16BitRegister fonksiyonları)

// 1. SAF İVMEÖLÇER İLE AÇI HESAPLAMA (Sadece durağan durumda doğru)
void calculateAnglesFromAccel(float ax, float ay, float az) {
    // Derece cinsinden açılar
    pitchAcc = atan2(-ax, sqrt(ay*ay + az*az)) * 180.0 / M_PI;
    rollAcc = atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / M_PI;
    
    // Alternatif formül (daha kararlı):
    // pitchAcc = atan2(-ax, az) * 180.0 / M_PI;
    // rollAcc = atan2(ay, az) * 180.0 / M_PI;
}

// 2. TAMAMLAYICI FİLTRE (Complementary Filter) - İvme + Jiroskop
void complementaryFilter(float ax, float ay, float az, 
                        float gx, float gy, float gz, 
                        float dt) {
    // İvmeölçerden açıları hesapla
    float accelPitch = atan2(-ax, sqrt(ay*ay + az*az)) * 180.0 / M_PI;
    float accelRoll = atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / M_PI;
    
    // Jiroskop açı değişimini entegre et
    pitchGyro += gx * dt;
    rollGyro += gy * dt;
    yawGyro += gz * dt;  // Manyetometre olmadan yaw drift yapar
    
    // Complementary filter: %96 jiroskop, %4 ivmeölçer
    float alpha = 0.96;
    pitch = alpha * (pitch + gx * dt) + (1 - alpha) * accelPitch;
    roll = alpha * (roll + gy * dt) + (1 - alpha) * accelRoll;
    yaw = yaw + gz * dt;  // Yaw için manyetometre gerekli
}

// 3. MADGWICK FİLTRE (Daha Gelişmiş - 9DOF için)
class MadgwickFilter {
private:
    float beta = 0.1;  // Filtre katsayısı
    float q0 = 1.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;  // Kuaternion
    
public:
    void update(float gx, float gy, float gz, 
                float ax, float ay, float az,
                float mx, float my, float mz,
                float dt) {
        
        // Normalize ivme ve manyetik alan vektörleri
        float norm = sqrt(ax*ax + ay*ay + az*az);
        ax /= norm; ay /= norm; az /= norm;
        
        norm = sqrt(mx*mx + my*my + mz*mz);
        mx /= norm; my /= norm; mz /= norm;
        
        // Madgwick filtresi güncelleme algoritması
        // (basitleştirilmiş versiyon)
        float qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz);
        float qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy);
        float qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx);
        float qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx);
        
        // Kuaternion güncelleme
        q0 += qDot1 * dt;
        q1 += qDot2 * dt;
        q2 += qDot3 * dt;
        q3 += qDot4 * dt;
        
        // Normalize et
        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q0 /= norm; q1 /= norm; q2 /= norm; q3 /= norm;
    }
    
    // Kuaternion'dan Euler açılarına dönüşüm
    void getEulerAngles(float &pitch, float &roll, float &yaw) {
        // Pitch (X ekseni etrafında dönme)
        float sinp = 2.0 * (q0 * q2 - q3 * q1);
        if (fabs(sinp) >= 1)
            pitch = copysign(M_PI / 2, sinp) * 180.0 / M_PI;
        else
            pitch = asin(sinp) * 180.0 / M_PI;
        
        // Roll (Y ekseni etrafında dönme)
        float sinr_cosp = 2.0 * (q0 * q1 + q2 * q3);
        float cosr_cosp = 1.0 - 2.0 * (q1 * q1 + q2 * q2);
        roll = atan2(sinr_cosp, cosr_cosp) * 180.0 / M_PI;
        
        // Yaw (Z ekseni etrafında dönme)
        float siny_cosp = 2.0 * (q0 * q3 + q1 * q2);
        float cosy_cosp = 1.0 - 2.0 * (q2 * q2 + q3 * q3);
        yaw = atan2(siny_cosp, cosy_cosp) * 180.0 / M_PI;
    }
};

MadgwickFilter filter;

// 4. ÖRNEK UYGULAMA: 3 FARKLI YÖNTEMLE AÇI HESAPLAMA
void calculateAndPrintAngles() {
    static unsigned long lastTime = 0;
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0;  // saniye cinsinden
    if (dt == 0) dt = 0.01;
    lastTime = currentTime;
    
    // Ham verileri oku
    int16_t ax_raw = read16BitRegister(MPU9250_ACCEL_XOUT_H);
    int16_t ay_raw = read16BitRegister(MPU9250_ACCEL_XOUT_H + 2);
    int16_t az_raw = read16BitRegister(MPU9250_ACCEL_XOUT_H + 4);
    
    int16_t gx_raw = read16BitRegister(MPU9250_GYRO_XOUT_H);
    int16_t gy_raw = read16BitRegister(MPU9250_GYRO_XOUT_H + 2);
    int16_t gz_raw = read16BitRegister(MPU9250_GYRO_XOUT_H + 4);
    
    // Kalibrasyon uygula
    float ax = (ax_raw - accelOffsetX) / 8192.0;  // ±4g için
    float ay = (ay_raw - accelOffsetY) / 8192.0;
    float az = (az_raw - accelOffsetZ) / 8192.0;
    
    float gx = (gx_raw - gyroOffsetX) / 65.5;     // ±500dps için
    float gy = (gy_raw - gyroOffsetY) / 65.5;
    float gz = (gz_raw - gyroOffsetZ) / 65.5;
    
    // YÖNTEM 1: Sadece ivmeölçer (statik durumlar için)
    calculateAnglesFromAccel(ax, ay, az);
    
    // YÖNTEM 2: Complementary Filter
    complementaryFilter(ax, ay, az, gx, gy, gz, dt);
    
    // YÖNTEM 3: Madgwick Filtresi (manyetometre verisi eklenmeli)
    // filter.update(gx, gy, gz, ax, ay, az, mx, my, mz, dt);
    // filter.getEulerAngles(pitchMadg, rollMadg, yawMadg);
    
    // SONUÇLARI YAZDIR
    Serial.println("\n=== AÇI HESAPLAMA SONUÇLARI ===");
    
    Serial.println("1. SADECE İVMEÖLÇER (durağan için):");
    Serial.print("   Pitch: "); Serial.print(pitchAcc, 1); 
    Serial.print("°\tRoll: "); Serial.print(rollAcc, 1); Serial.println("°");
    
    Serial.println("2. COMPLEMENTARY FİLTRE (ivme+gyro):");
    Serial.print("   Pitch: "); Serial.print(pitch, 1);
    Serial.print("°\tRoll: "); Serial.print(roll, 1);
    Serial.print("°\tYaw: "); Serial.print(yaw, 1); Serial.println("°");
    
    // İvme vektör büyüklüğü (kalite kontrol)
    float accelMag = sqrt(ax*ax + ay*ay + az*az);
    Serial.print("3. KALİTE METRİKLERİ:");
    Serial.print("   İvme Büyüklüğü: "); Serial.print(accelMag, 3); 
    Serial.print("g (1.0 olmalı)");
    
    if (accelMag > 1.05 || accelMag < 0.95) {
        Serial.print(" ⚠️ Kalibrasyon gerekli!");
    }
    Serial.println();
}

/*
Accel Offset - X:-307.00 Y:138.00 Z:830.00
Gyro Offset  - X:-54.00 Y:40.00 Z:-13.00
*/



void setup() {
    Serial.begin(115200);

    
    Serial.println("\nMPU9250 SPI Test Programı");
    Serial.println("==========================");
    
    // CS pinini çıkış olarak ayarla
    pinMode(MPU9250_CS_PIN, OUTPUT);
    digitalWrite(MPU9250_CS_PIN, HIGH);
    
    // SPI başlat
    SPI.begin();
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0)); // 8 MHz
    
    // MPU9250'i başlat
    if (!initializeMPU9250()) {
        Serial.println("Program durduruldu.");
        while (1);
    }
    
    calibrateMPU9250();
    delay(1000);
    Serial.println("\nVeri okuma başlıyor...");
}

void loop() {
    //readSensorData();
    calculateAndPrintAngles();
    delay(20);  // 2 Hz örnekleme hızı
}