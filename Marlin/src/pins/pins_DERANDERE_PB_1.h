/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * PipetBot-A8 by DerAndere pin assignments
 */

#define DEFAULT_MACHINE_NAME "PipetBot-A8"
#define BOARD_NAME           "Derandere (Anet)"

#include "pins_ANET_10.h"

#if ENABLED(ULTRA_LCD)
  #if ENABLED(ADC_KEYPAD)
    #undef BTN_EN1
    #undef BTN_EN2
    #undef BTN_ENC
  #endif
#else
  #define SERVO1_PIN         28
  #define SERVO2_PIN         29
  #define SERVO3_PIN         30
  //#define SERVO4_PIN         10
  #if ENABLED(E_AXIS_HOMING)
    #define E_STOP_PIN       11
  #endif
#endif
