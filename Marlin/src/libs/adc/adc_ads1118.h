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
      ADC_ADS1118(uint8_t cs_pin, uint8_t mosi_pin, uint8_t miso_pin, uint8_t sck_pin);

      void init();
      int16_t readChannel(uint8_t channel);
      float readVoltage(uint8_t channel, float vref = 2.048f);
      float readInternalTemp();    

      bool Error;
    private:
      uint8_t cs, mosi, miso, sck;

      uint16_t transfer16(uint16_t data);
      uint8_t transfer8(uint8_t data);

      void select();
      void deselect();

      void sckHigh();
      void sckLow();

      // Configuración de registros
      uint16_t configForChannel(uint8_t channel, bool temp = false);      
  };

  extern ADS1118 ads1118;


#endif // ENABLED(HAS_ADS1118)