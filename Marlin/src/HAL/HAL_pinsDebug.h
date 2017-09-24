/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef HAL_PINSDEBUG_H
#define HAL_PINSDEBUG_H

#ifdef __AVR__
  #include "HAL_AVR/pinsDebug_AVR_8_bit.h"
#elif defined(ARDUINO_ARCH_SAM)
  #include "HAL_DUE/HAL_pinsDebug_Due.h"
#elif IS_32BIT_TEENSY
  #include "HAL_TEENSY35_36/HAL_pinsDebug_Teensy.h"
#elif defined(TARGET_LPC1768)
  #include "HAL_LPC1768/pinsDebug_Re_ARM.h"
#else
  #error Unsupported Platform!
#endif

#endif // HAL_PINSDEBUG_H
