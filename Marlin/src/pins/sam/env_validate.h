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

// Why ATmega2560? The ATsam board are based on ARM Cortex microprocessors which is
// an entirely different architecture than 8bit AVR MCUs. This file is in practice only
// allowing SAM MCU boards anyway. So the ATmega MCU support can be cut out of here, in favor
// of own board pin header definitions in the ramps folder!

#if BOTH(ALLOW_MEGA1280, ALLOW_MEGA2560) && NOT_TARGET(__SAM3X8E__, __AVR_ATmega1280__, __AVR_ATmega2560__)
  #error "Oops! Select 'Arduino Due or Mega' in 'Tools > Board.'"
#elif ENABLED(ALLOW_MEGA2560) && NOT_TARGET(__SAM3X8E__, __AVR_ATmega2560__)
  #error "Oops! Select 'Arduino Due or Mega' in 'Tools > Board.'"
#elif ENABLED(ALLOW_MEGA1280) && NOT_TARGET(__SAM3X8E__, __AVR_ATmega1280__)
  #error "Oops! Select 'Arduino Due' in 'Tools > Board.'"
#endif

#undef ALLOW_MEGA1280
#undef ALLOW_MEGA2560
