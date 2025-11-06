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
#pragma once

/**
 * Based on Arduino Library for Texas Instruments ADS1118 - 16-Bit Analog-to-Digital Converter with internal Reference and Temperature Sensor 
 * https://www.ti.com/product/ADS1118
 * https://github.com/ADS1xxx-Series-ADC-Libraries/ADS1118
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(HAS_ADS1118)

  #include <stdint.h>

  #define TEMP_SENSOR_ADS1118 -18

  typedef struct {
     int16_t adc;
     int16_t temp;
  } ADC_Lookup;

  const static ADC_Lookup PROGMEM table_thermocouple_k[] = {
      { -304, -64},
      { -232, -48},
      { -157, -32},
      {  -79, -16},
      {    0,   0},
      {   82,  16},
      {  164,  32},
      {  248,  48},
      {  333,  64},
      {  418,  80},
      {  503,  96},
      {  588, 112},
      {  672, 128},
      {  755, 144},
      {  837, 160},
      {  919, 176},
      { 1001, 192},
      { 1083, 208},
      { 1165, 224},
      { 1248, 240},
      { 1331, 256},
      { 1415, 272},
      { 1499, 288},
      { 1584, 304},
      { 1754, 336},
      { 1840, 352},
      { 1926, 368},
      { 2012, 384},
      { 2099, 400}

  };  

  #define TEMP_TABLE_SIZE (sizeof(table_thermocouple_k) / sizeof(table_thermocouple_k[0]))
  #define TEMP_MIN_TEMP     0
  #define TEMP_MAX_TEMP   300
  #define TEMP_TABLE_OFFSET 0 // grados Celsius por índice

  class ADS1118 {
    public:
      static void init(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck);
      static void startConversion(uint8_t pair); // inicia conversión
      static bool ready();                       // indica si ya se puede leer
      static int16_t read();                     // lee valor convertido
      static bool busy();                        // estado de conversión
      static void loop();                        // ciclo no bloqueante

      static bool checkDataReady();
      static void startContinuousConversion(uint8_t channel_pair);
      static uint16_t readData();
      static int16_t readChannel(uint8_t channel); 
      static uint16_t readConfig (); 
      
    private:
      static void spiTransfer(uint8_t data, uint8_t &resp);
      static void writeWord(uint16_t word);
      static void readWord(uint16_t &word);

      static uint32_t transfer32 (uint16_t data);
      static uint16_t transfer16 (uint16_t data);
      static uint8_t transfer8 (uint8_t data);
      

      static void select();
      static void deselect();  
      
      static void sckHigh();
      static void sckLow();      

      static uint8_t cs_pin, mosi_pin, miso_pin, sck_pin;
      static unsigned long startTime;
      static int16_t lastValue;
      static int16_t config;
      static bool isBusy;  
      static uint8_t currentchannel;  

      uint16_t configForChannel(uint8_t channel); 
      static uint16_t configChannel(uint8_t channel); 
      float readVoltage(uint8_t channel, float vref); 
      float readInternalTemp();

  };

  extern ADS1118 ads1118;

  class ThermocoupleK {
  public:
    void init();
    float tempReadtoCelsius(int16_t rawADC);
    float getTempCelsius();
    float _Tcold,  _Thot;

    void setTcold(float tcold);
    float getTcold();

    void setThot(float thot);
    float getThot();  
};


#endif // ENABLED(HAS_ADS1118)