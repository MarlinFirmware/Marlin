/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * adc_ads1118.cpp -  library for Texas Instruments ADS1118 - 16-Bit Analog-to-Digital Converter
 * based in the sailfish code for ADS1118, ThermocoupleReader, TemperatureTable
 * For implementation details, please take a look at the datasheet:
 * https://www.ti.com/product/ADS1118
 */

#include "../../inc/MarlinConfig.h"

#if HAS_ADS1118

#include "adc_ads1118.h"

#include "../../HAL/shared/Delay.h"
#include "../../core/macros.h"

#define ADS1118_CONV_MS 10
#define ADS1118_CH_MASK 12

// Constructor
void ADS1118::init(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck) {
  cs_pin = cs; mosi_pin = mosi; miso_pin = miso; sck_pin = sck;

  //#define TEMP_0_CS_PIN                         79  // E6
  //#define TEMP_0_SCK_PIN                        78  // E2
  //#define TEMP_0_MISO_PIN                       80  // E7
  //#define TEMP_0_MOSI_PIN                       84  // H2

  pinMode(cs_pin, OUTPUT);
  pinMode(mosi_pin, OUTPUT);
  pinMode(sck_pin, OUTPUT);
  pinMode(miso_pin, INPUT);

  deselect();
  sckLow();
}

// Sets ADS to start a single shot conversion. It will be read async when ready (after ADS1118_CONV_MS), non blocking
void ADS1118::startConversion(const uint8_t pair) {
  if (isBusy) return;

  uint16_t config = 0x858B; // 0b 1000 0101 1000 1011 : SS start, single-ended off, gain ±2.048V, single-shot mode, 128SPS, ADC mode, Pullup enable, Write config
  switch (pair) {
    default:
    case 0:  config |= (0x0 << ADS1118_CH_MASK); currentchannel = 0; break; // AIN0-AIN1
    case 1:  config |= (0x3 << ADS1118_CH_MASK); currentchannel = 1; break; // AIN2-AIN3
  }

  digitalWrite(cs_pin, LOW);
  transfer16(config);
  digitalWrite(cs_pin, HIGH);

  isBusy = true;
  startTime = millis();
}

// Determine that a conversion is ready by its elapsed time, if true, reads data and stores in _lastValue
bool ADS1118::ready() {
  if (!isBusy) return true;
  if (millis() - startTime >= ADS1118_CONV_MS) {
    digitalWrite(cs_pin, LOW);
    uint16_t raw = transfer16(0);
    digitalWrite(cs_pin, HIGH);
    lastValue = (int16_t)raw;
    isBusy = false;
  }
  return !isBusy;
}

int16_t ADS1118::read() { return lastValue; }

bool ADS1118::busy() { return isBusy; }

void ADS1118::loop() { ready(); } // updates status and value

// Sets ADS to start Continuous conversion mode
void ADS1118::startContinuousConversion(const uint8_t channel_pair) {
  SERIAL_ECHOLNPGM("ADS1118 Set to start conv");
  if (isBusy) return;
  // 0x048B b 0000 0100 1000 1011 : SS start, single-ended off, gain ±2.048V, Continuous conversion mode, 128SPS, ADC mode, Pullup enable, Write config
  // 0x049B: read internal temp
  uint16_t config = 0x048B;
  switch (channel_pair) {
    default:
    case 0:  config |= (0x0 << ADS1118_CH_MASK); currentchannel = 0; break; // AIN0-AIN1
    case 1:  config |= (0x3 << ADS1118_CH_MASK); currentchannel = 1; break; // AIN2-AIN3
  }

  digitalWrite(cs_pin, LOW);
  transfer16(config);
  digitalWrite(cs_pin, HIGH);

  isBusy = true;
  startTime = millis();
  SERIAL_ECHOLNPGM("ADS1118 Leaving start conv");
}

// Check if ADS has a complete conversion
bool ADS1118::checkDataReady() {
  digitalWrite(cs_pin, LOW);
  const uint8_t isReady = !digitalRead(miso_pin); // Read MISO, is low when ready
  digitalWrite(cs_pin, HIGH);
  return isReady;
}

// Read ADS data
uint16_t ADS1118::readData() {
  digitalWrite(cs_pin, LOW);
  const uint16_t data = transfer16(0);
  digitalWrite(cs_pin, HIGH);
  return data;
}

// Read and write ADS data
uint16_t ADS1118::readWriteData(const uint16_t config) {
  digitalWrite(cs_pin, LOW);
  const uint16_t data = transfer16(config);
  digitalWrite(cs_pin, HIGH);
  return data;
}

// Reads and returns a single channel inmediately with delay (blocking)
int16_t ADS1118::readChannel(const uint8_t channel) {
  const uint16_t config = configChannel(channel);
  transfer16(config);

  // ADS1118 takes ~8 ms to convert
  delay(10);

  const int16_t value = (int16_t)transfer16(config);
  return value;
}

float ADS1118::readVoltage(const uint8_t channel, const float vref) {
  const int16_t raw = readChannel(channel);
  return (raw / 32768.0f) * vref; // scale 16 bits to voltage
}

float ADS1118::readInternalTemp() {
  const uint16_t config = config_ADC_SS_TEMP; // Config internal temp read 0x8F80; // Config internal temp read & start SS conversion
  select();
  transfer16(config);
  deselect();
  delay(10);
  select();
  int16_t raw = (int16_t)transfer16(config);
  deselect();
  return convertInternalTemp(raw);
}

// Convert raw internal temperature data to °C
float ADS1118::convertInternalTemp(const int16_t data) {
  return float(data >> 2) * 0.03125f; // 14 bit left aligned, 0.03125 °C per LSB as datasheet
}

uint16_t ADS1118::configChannel(const uint8_t channel) {
  uint16_t config = 0;

  // Config single-ended inputs PGA ±2.048V, SS mode,  128SPS no pull up
  switch (channel) {
    case 0: config = 0xC583; break;   // 0b 1100 0101 1000 0011 Ch1 & start SS conversion
    case 1: config = 0xD583; break;   // 0b 1101 0101 1000 0011 Ch2 & start SS conversion
    case 2: config = 0xE583; break;   // 0b 1110 0101 1000 0011 Ch3 & start SS conversion
    case 3: config = 0xF583; break;   // 0b 1111 0101 1000 0011 Ch4 & start SS conversion
    default: config = 0x8583; break;
  }

  return config;
}

uint32_t ADS1118::transfer32(uint16_t data) {
  uint16_t result_prev = transfer16(data);    // send config, receive previous result
  uint16_t config_echo = transfer16(0x0000);  // send dummy, receive echo of configuration

  // Combine both into a single 32-bit value
  return ((uint32_t)result_prev << 16) | config_echo;
}

uint16_t ADS1118::readConfig() {
  uint16_t config_echo;
  select();
  transfer16(0x0000);                  // send 16‑bit dummy, ignore reply; configuration is in the following 16‑bit transfer
  config_echo = transfer16(0x0000);    // send dummy, receive echo of configuration
  SERIAL_ECHOPGM("ADS1118 Configuration: "); SERIAL_ECHOLN(config_echo);
  deselect();
  return config_echo;
}

uint16_t ADS1118::transfer16(uint16_t data) {
  uint8_t high = transfer8((uint8_t)(data >> 8));
  uint8_t low  = transfer8((uint8_t)(data & 0xFF));
  return (uint16_t)(high << 8) | low;
}

uint8_t ADS1118::transfer8(uint8_t data) {
  uint8_t recv = 0;
  for (uint8_t i = 0; i < 8; i++) {
    // Send MSB first
    if (data & 0x80) digitalWrite(mosi_pin, HIGH); else digitalWrite(mosi_pin, LOW);
    data <<= 1;

    sckHigh();
    DELAY_NS(100); // small delay for stability

    recv <<= 1;
    if (digitalRead(miso_pin)) recv |= 0x01;

    sckLow();
    DELAY_NS(100);
  }
  return recv;
}

void ADS1118::select()   { digitalWrite(cs_pin, LOW); }
void ADS1118::deselect() { digitalWrite(cs_pin, HIGH); }

void ADS1118::sckHigh()  { digitalWrite(sck_pin, HIGH); }
void ADS1118::sckLow()   { digitalWrite(sck_pin, LOW); }

// Static variable definitions
uint8_t ADS1118::cs_pin = 0;
uint8_t ADS1118::mosi_pin = 0;
uint8_t ADS1118::miso_pin = 0;
uint8_t ADS1118::sck_pin = 0;
unsigned long ADS1118::startTime = 0;
int16_t ADS1118::lastValue = 0;
int16_t ADS1118::config = 0;
uint16_t ADS1118::current_config = 0;
uint16_t ADS1118::previous_config = 0;

bool ADS1118::isBusy = false;
uint8_t ADS1118::currentchannel = 0;

// ADS1118, global instance
ADS1118 ads1118;
ThermocoupleK thck_0;
ThermocoupleK thck_1;

//#if TEMP_SENSOR_0_IS_ADS1118
//  #warning "ThcK 0 is enabled"
//  ThermocoupleK thck_0;
//#endif

//#if TEMP_SENSOR_1_IS_ADS1118
//  #warning "ThcK 1 is enabled"
//  ThermocoupleK thck_1;
//#endif

void ThermocoupleK::init() {}

// --- Convert ADC reading to °C ---
float ThermocoupleK:: tempReadtoCelsius(const int16_t rawADC) {
  //Serial.println((int16_t)pgm_read_word(&ThermocoupleK_Lookup[TEMP_TABLE_SIZE - 1]));

  if (rawADC > (int16_t) pgm_read_word(&table_thermocouple_k[TEMP_TABLE_SIZE - 1].adc))
      return TEMP_MAX_TEMP;
  if (rawADC < (int16_t) pgm_read_word(&table_thermocouple_k[0].adc))
      return TEMP_MIN_TEMP;

  // Linear search in the table (from lowest to highest ADC value)
  for (uint16_t i = 0; i < TEMP_TABLE_SIZE - 1; i++) {

    int16_t adc1 = pgm_read_word(&table_thermocouple_k[i].adc);
    int16_t adc2  = pgm_read_word(&table_thermocouple_k[i + 1].adc);
    // Serial.print(i); Serial.print(" "); Serial.print(adc1); Serial.print(" "); Serial.print(adc2);

    if (rawADC >= adc1 && rawADC < adc2) { // in-between tableValue and nextValue
      // Approximate temperature via linear interpolation
      //float frac = float(rawADC - adc2) / float(adc1 - adc2);
      int16_t t1  = pgm_read_word(&table_thermocouple_k[i].temp);
      int16_t t2  = pgm_read_word(&table_thermocouple_k[i+1].temp);
      // Serial.print(" "); Serial.print(t1); Serial.print(" "); Serial.print(t2);
      float tempC = t1 + (float) (rawADC - adc1) * (float(t2 - t1) / float(adc2 - adc1)); // TEMP_TABLE_OFFSET + i + frac;
      return tempC;
    }
    // Serial.println("");
  }

  return TEMP_MIN_TEMP;
}

float ThermocoupleK:: calcTempCelsius() {
  _Tcold = ads1118.convertInternalTemp(_raw_cold);
  _Thot = tempReadtoCelsius(_raw_hot);
  //SERIAL_ECHOPGM("ADS1118 TCold "); SERIAL_ECHOLN(_Tcold);
  //SERIAL_ECHOPGM("ADS1118 THot "); SERIAL_ECHOLN(_Thot);
  return _Thot + _Tcold;
}

void ThermocoupleK::setRawCold(const int16_t raw_cold) {
  _raw_cold = raw_cold;
}

void ThermocoupleK::setRawHot(const int16_t raw_hot) {
  _raw_hot = raw_hot;
}
void ThermocoupleK::setTcold(const float tcold) {
  _Tcold = tcold;
}

float ThermocoupleK::getTcold() {
  return _Tcold;
}

void ThermocoupleK::setThot(const float thot) {
  _Thot = thot;
}

float ThermocoupleK::getThot() {
  return _Thot;
}

float ThermocoupleK:: getTempCelsius() {
  return _Thot + _Tcold;
}

#endif // HAS_ADS1118
