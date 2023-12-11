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

#if HAS_U8GLIB_I2C_OLED
  // Define this to reduce build size and optimize performance
  //#define COMPILE_TIME_I2C_IS_HARDWARE true   // true: Hardware  false: Software  undefined: Solve at runtime

  #ifdef COMPILE_TIME_I2C_IS_HARDWARE
    #if COMPILE_TIME_I2C_IS_HARDWARE
      #define U8G_USES_HW_I2C
    #else
      #define U8G_USES_SW_I2C
    #endif
  #else
    #define U8G_USES_HW_I2C
    #define U8G_USES_SW_I2C
  #endif

#endif
