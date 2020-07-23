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

// If no real EEPROM, Flash emulation, or SRAM emulation is available fall back to SD emulation
#if USE_FALLBACK_EEPROM
  #define SDCARD_EEPROM_EMULATION
#elif EITHER(I2C_EEPROM, SPI_EEPROM)
  #define USE_SHARED_EEPROM 1
#endif

// Allow SDSUPPORT to be disabled
#if DISABLED(SDSUPPORT)
  #undef SDIO_SUPPORT
#endif
