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
#pragma once

/**
 * Texas Instruments ADS1115 16-bit I2C ADC, used to read thermistors.
 * https://www.ti.com/product/ADS1115
 */

#include "../../inc/MarlinConfigPre.h"

class ADS1115 {
  public:
    static void init();                 // Start I2C and read every channel once
    static void update();               // Non-blocking. Call often from the main loop.
    static bool faulted() { return fault; }

    // Latest reading of an input, as a fraction of the pull-up voltage scaled to 0..range-1
    static raw_adc_t raw(const uint8_t channel, const uint32_t range);

  private:
    static int16_t code[4];             // Latest conversion result for each input
    static uint8_t scan_index;          // The input being converted
    static bool converting, scanned, fault;
    static millis_t next_ms, scan_ms;
};

extern ADS1115 ads1115;
