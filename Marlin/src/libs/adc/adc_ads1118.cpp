/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 *
 * For implementation details, please take a look at the datasheet:
 * https://www.ti.com/product/ADS1118
 * 
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(HAS_ADS1118)
  
  #include "adc_ads1118.h"
  
  #include "../../HAL/shared/Delay.h"
  #include "../../core/macros.h"
  
  // Initialize the ADS1118, global instance
  ADC_ADS1118 ads1118(ADS1118_CS_PIN, ADS1118_MOSI_PIN, ADS1118_MISO_PIN, ADS1118_SCK_PIN);
  
  // Constructor
  ADC_ADS1118::ADC_ADS1118(uint8_t cs_pin, uint8_t mosi_pin, uint8_t miso_pin, uint8_t sck_pin)
    : cs(cs_pin), mosi(mosi_pin), miso(miso_pin), sck(sck_pin) {}
  
  
  void ADC_ADS1118::init() {
    SET_OUTPUT(cs);
    SET_OUTPUT(mosi);
    SET_OUTPUT(sck);
    SET_INPUT(miso);
  
    deselect();
    sckLow();
  }
  
  int16_t ADC_ADS1118::readChannel(uint8_t channel) {
    uint16_t config = configForChannel(channel, false);
    transfer16(config);
  
    // ADS1118 takes ~8 ms to convert
    delay(10);
  
    int16_t value = (int16_t)transfer16(config);
    return value;
  }
  
  float ADC_ADS1118::readVoltage(uint8_t channel, float vref) {
    int16_t raw = readChannel(channel);
    return (raw / 32768.0f) * vref; // scale 16 bits to voltage
  }
  
  float ADC_ADS1118::readInternalTemp() {
    uint16_t config = configForChannel(0, true);
    transfer16(config);
  
    delay(10);
  
    int16_t raw = (int16_t)transfer16(config);
    return raw * 0.03125f; // 0.03125 °C by LSB and datasheet
  }
  
  uint16_t ADC_ADS1118::configChannel(uint8_t channel, bool temp) {
    uint16_t config = 0;
  
    if (temp) {
      config = 0x0F80; // Config internal temp 
    }
    else {
      // Config single-ended inputs PGA ±2.048V
      switch(channel) {
        case 0: config = 0x8583; break;
        case 1: config = 0x9083; break;
        case 2: config = 0xA083; break;
        case 3: config = 0xB083; break;
        default: config = 0x8583; break;
      }
    }
  
    return config;
  }
  
  uint16_t ADC_ADS1118::transfer16(uint16_t data) {
    uint8_t high = transfer8((uint8_t)(data >> 8));
    uint8_t low  = transfer8((uint8_t)(data & 0xFF));
    return (uint16_t)(high << 8) | low;
  }
  
  uint8_t ADC_ADS1118::transfer8(uint8_t data) {
    uint8_t recv = 0;
    for (uint8_t i = 0; i < 8; i++) {
      // Send MSB first
      if (data & 0x80) WRITE(mosi, HIGH); else WRITE(mosi, LOW);
      data <<= 1;
  
      sckHigh();
      DELAY_NS(100); // small delay for stability
  
      recv <<= 1;
      if (READ(miso)) recv |= 0x01;
  
      sckLow();
      DELAY_NS(100);
    }
    return recv;
  }
  
  void ADC_ADS1118::select()   { WRITE(cs, LOW); }
  void ADC_ADS1118::deselect() { WRITE(cs, HIGH); }
  
  void ADC_ADS1118::sckHigh()  { WRITE(sck, HIGH); }
  void ADC_ADS1118::sckLow()   { WRITE(sck, LOW); }
  
  ADS1118 ads1118;
  
#endif // HAS_ADS1118_ADC
