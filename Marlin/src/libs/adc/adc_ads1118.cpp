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
    
  #define ADS1118_CONV_MS 10
  #define ADS1118_CH_MASK 12
  
    // Constructor
  void ADS1118::init(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck) {
    cs_pin = cs; mosi_pin = mosi; miso_pin = miso; sck_pin = sck;

// #define TEMP_0_CS_PIN                         79  // E6
// #define TEMP_0_SCK_PIN                        78  // E2
// #define TEMP_0_MISO_PIN                       80  // E7
// #define TEMP_0_MOSI_PIN                       84  // H2

      pinMode(cs_pin, OUTPUT);
      pinMode(mosi_pin, OUTPUT);
      pinMode(sck_pin, OUTPUT);
      pinMode(miso_pin, INPUT);
    
      deselect();
      sckLow();
      
    }
    
  // Sets ADS to start a single shot conversion, it will be read async when ready (after ADS1118_CONV_MS), non blocking
    void ADS1118::startConversion(uint8_t pair) {
      if (isBusy) return;
  
      uint16_t config = 0x858B; // 0b 1000 0101 1000 1011 : SS start, single-ended off, gain ±2.048V, single-shot mode, 128SPS, ADC mode, Pullup enable, Write config
      switch (pair) {
        case 0: config |= (0x0 << ADS1118_CH_MASK); currentchannel = 0; break; // AIN0-AIN1
        case 1: config |= (0x3 << ADS1118_CH_MASK); currentchannel = 1; break; // AIN2-AIN3
        default: config |= (0x0 << ADS1118_CH_MASK); currentchannel = 0; break;
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
      uint16_t raw;
      digitalWrite(cs_pin, LOW);
      transfer16(raw);
      digitalWrite(cs_pin, HIGH);
      lastValue = (int16_t)raw;
      isBusy = false;
    }
    return !isBusy;
  }
  
    int16_t ADS1118::read() {
      return lastValue;
    }
  
    bool ADS1118::busy() {
      return isBusy;
    }
  
    void ADS1118::loop() {
      ready(); // updates status and value
    }  
  
  // Sets ADS to start Continuous conversion mode
    void ADS1118::startContinuousConversion(uint8_t channel_pair) {
      SERIAL_ECHOLNPGM("ADS1118 Set to start conv");
      if (isBusy) return;
  
      uint16_t config = 0x848B; // b 1000 0100 1000 1011 : SS start, single-ended off, gain ±2.048V, Continuous conversion mode, 128SPS, ADC mode, Pullup enable, Write config
      switch (channel_pair) {
        case 0: config |= (0x0 << ADS1118_CH_MASK); currentchannel = 0; break; // AIN0-AIN1
        case 1: config |= (0x3 << ADS1118_CH_MASK); currentchannel = 1; break; // AIN2-AIN3
        default: config |= (0x0 << ADS1118_CH_MASK); currentchannel = 0; break;
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
      uint8_t isReady = !digitalRead(miso_pin); // Read MISO, is low when ready
      digitalWrite(cs_pin, HIGH);
      
      return isReady;

    }    

    // Check if ADS has a complete conversion
    uint16_t ADS1118::readData() {

      digitalWrite(cs_pin, LOW);
      uint16_t data = transfer16(0);
      digitalWrite(cs_pin, HIGH);      
      return data;

    }       
  

    // Reads and returns a single channel inmediately with delay (blocking)
    int16_t ADS1118::readChannel(uint8_t channel) {
      uint16_t config = configChannel(channel);
      transfer16(config);
    
      // ADS1118 takes ~8 ms to convert
      delay(10);
    
      int16_t value = (int16_t)transfer16(config);
      return value;
    }
    
    float ADS1118::readVoltage(uint8_t channel, float vref) {
      int16_t raw = readChannel(channel);
      return (raw / 32768.0f) * vref; // scale 16 bits to voltage
    }
    
    float ADS1118::readInternalTemp() {
      uint16_t config =  0x8F80; // Config internal temp read & start SS conversion
      transfer16(config);
    
      delay(10);
    
      int16_t raw = (int16_t)transfer16(config);
      return raw * 0.03125f; // 0.03125 °C by LSB and datasheet
    }
    
    uint16_t ADS1118::configChannel(uint8_t channel) {
      uint16_t config = 0;
    
  
      // Config single-ended inputs PGA ±2.048V, SS mode,  128SPS no pull up 
      switch(channel) {
        case 0: config = 0xC583; break;   // 0b 1100 0101 1000 0011 Ch1 & start SS conversion 
        case 1: config = 0xD583; break;   // 0b 1101 0101 1000 0011 Ch2 & start SS conversion 
        case 2: config = 0xE583; break;   // 0b 1110 0101 1000 0011 Ch3 & start SS conversion 
        case 3: config = 0xF583; break;   // 0b 1111 0101 1000 0011 Ch4 & start SS conversion 
        default: config = 0x8583; break;
      }
    
    
      return config;
    }
    
    uint32_t ADS1118::transfer32(uint16_t data) {
      uint16_t result_prev, config_echo;

      result_prev = transfer16(data);    // envía config, recibe resultado anterior
      config_echo = transfer16(0x0000);    // envía dummy, recibe eco de configuración

      // Combina ambos en un solo valor de 32 bits
      return ((uint32_t)result_prev << 16) | config_echo;
    }

    uint16_t ADS1118::readConfig() {
      uint16_t config_echo;

      transfer16(0x0000);   
      config_echo = transfer16(0x0000);    // envía dummy, recibe eco de configuración
      SERIAL_ECHOPGM("ADS1118 Configuration: 0x"); SERIAL_ECHOLN(config_echo, HEX);
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

    // Definiciones de variables estáticas
    uint8_t ADS1118::cs_pin = 0;
    uint8_t ADS1118::mosi_pin = 0;
    uint8_t ADS1118::miso_pin = 0;
    uint8_t ADS1118::sck_pin = 0;
    unsigned long ADS1118::startTime = 0;
    int16_t ADS1118::lastValue = 0;
    int16_t ADS1118::config = 0;
    bool ADS1118::isBusy = false;
    uint8_t ADS1118::currentchannel = 0;    
    
    // ADS1118, global instance
    ADS1118 ads1118;
    
#endif // HAS_ADS1118_ADC
