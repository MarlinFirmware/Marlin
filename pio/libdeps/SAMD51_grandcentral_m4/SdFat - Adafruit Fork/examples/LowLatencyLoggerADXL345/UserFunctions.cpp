#include "UserTypes.h"
// User data functions.  Modify these functions for your data items.

// Start time for data
static uint32_t startMicros;

const uint8_t ADXL345_CS = 9;

const uint8_t POWER_CTL = 0x2D;  //Power Control Register
const uint8_t DATA_FORMAT = 0x31;
const uint8_t DATAX0 = 0x32; //X-Axis Data 0
const uint8_t DATAX1 = 0x33; //X-Axis Data 1
const uint8_t DATAY0 = 0x34; //Y-Axis Data 0
const uint8_t DATAY1 = 0x35; //Y-Axis Data 1
const uint8_t DATAZ0 = 0x36; //Z-Axis Data 0
const uint8_t DATAZ1 = 0x37; //Z-Axis Data 1

void writeADXL345Register(const uint8_t registerAddress, const uint8_t value) {
  // Max SPI clock frequency is 5 MHz with CPOL = 1 and CPHA = 1.
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));  
  digitalWrite(ADXL345_CS, LOW);
  SPI.transfer(registerAddress);
  SPI.transfer(value);
  digitalWrite(ADXL345_CS, HIGH);
  SPI.endTransaction();  
}

void userSetup() {
  SPI.begin();
  pinMode(ADXL345_CS, OUTPUT);
  digitalWrite(ADXL345_CS, HIGH);
  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeADXL345Register(DATA_FORMAT, 0x01);
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeADXL345Register(POWER_CTL, 0x08);  //Measurement mode  
}

// Acquire a data record.
void acquireData(data_t* data) {
  // Max SPI clock frequency is 5 MHz with CPOL = 1 and CPHA = 1.
  SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE3));
  data->time = micros();
  digitalWrite(ADXL345_CS, LOW);
  // Read multiple bytes so or 0XC0 with address.
  SPI.transfer(DATAX0 | 0XC0);
  data->accel[0] = SPI.transfer(0) | (SPI.transfer(0) << 8);
  data->accel[1] = SPI.transfer(0) | (SPI.transfer(0) << 8);
  data->accel[2] = SPI.transfer(0) | (SPI.transfer(0) << 8); 
  digitalWrite(ADXL345_CS, HIGH);
  SPI.endTransaction();
}

// Print a data record.
void printData(Print* pr, data_t* data) {
  if (startMicros == 0) {
    startMicros = data->time;
  }
  pr->print(data->time - startMicros);
  for (int i = 0; i < ACCEL_DIM; i++) {
    pr->write(',');
    pr->print(data->accel[i]);
  }
  pr->println();
}

// Print data header.
void printHeader(Print* pr) {
  startMicros = 0;
  pr->println(F("micros,ax,ay,az"));
}
