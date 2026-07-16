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
 */
#pragma once

/**
 * AT32-specific LCD conditionals
 */

#ifdef ARDUINO_ARCH_AT32

// Default graphical display delays for AT32F403 (Cortex-M4 @ 240 MHz)
#ifndef CPU_ST7920_DELAY_1
  #define CPU_ST7920_DELAY_1 300
#endif
#ifndef CPU_ST7920_DELAY_2
  #define CPU_ST7920_DELAY_2  40
#endif
#ifndef CPU_ST7920_DELAY_3
  #define CPU_ST7920_DELAY_3 340
#endif

#endif // ARDUINO_ARCH_AT32
