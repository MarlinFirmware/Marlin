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

  class ADS1118 {
    public:
      static void init(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck);
      static void startConversion(uint8_t pair); // inicia conversión
      static bool ready();                       // indica si ya se puede leer
      static int16_t read();                     // lee valor convertido
      static bool busy();                        // estado de conversión
      static void loop();                        // ciclo no bloqueante

      static bool checkDataReady();
      static void startContinuousConversion(uint8_t channel_pair)
      static uint16_t readData();
      
    private:
      static void spiTransfer(uint8_t data, uint8_t &resp);
      static void writeWord(uint16_t word);
      static void readWord(uint16_t &word);

      void select();
      void deselect();  
      
      void sckHigh();
      void sckLow();      

      static uint8_t _cs, _mosi, _miso, _sck;
      static unsigned long _startTime;
      static int16_t _lastValue;
      static bool _isBusy;  
      static uint8_t _currentchannel;  

      uint16_t configForChannel(uint8_t channel); 

  };

  extern ADS1118 ads1118;


#endif // ENABLED(HAS_ADS1118)