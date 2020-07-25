/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../core/macros.h"

#if BOTH(SDSUPPORT, HAS_GRAPHICAL_LCD) && (LCD_PINS_D4 == SCK_PIN || LCD_PINS_ENABLE == MOSI_PIN || DOGLCD_SCK == SCK_PIN || DOGLCD_MOSI == MOSI_PIN)
  #define LPC_SOFTWARE_SPI  // If the SD card and LCD adapter share the same SPI pins, then software SPI is currently
                            // needed due to the speed and mode required for communicating with each device being different.
                            // This requirement can be removed if the SPI access to these devices is updated to use
                            // spiBeginTransaction.
#endif

/** onboard SD card */
//#define SCK_PIN           P0_07
//#define MISO_PIN          P0_08
//#define MOSI_PIN          P0_09
//#define SS_PIN            P0_06
/** external */
#ifndef SCK_PIN
  #define SCK_PIN           P0_15
#endif
#ifndef MISO_PIN
  #define MISO_PIN          P0_17
#endif
#ifndef MOSI_PIN
  #define MOSI_PIN          P0_18
#endif
#ifndef SS_PIN
  #define SS_PIN            P1_23
#endif
#if !defined(SDSS) || SDSS == P_NC // gets defaulted in pins.h
  #undef SDSS
  #define SDSS              SS_PIN
#endif
