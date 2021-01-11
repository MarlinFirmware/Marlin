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

/**
 * feature/spindle_laser_types.h
 * Support for Laser Power or Spindle Power & Direction
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(SPINDLE_FEATURE)
  #define _MSG_CUTTER(M) MSG_SPINDLE_##M
#else
  #define _MSG_CUTTER(M) MSG_LASER_##M
#endif
#define MSG_CUTTER(M) _MSG_CUTTER(M)

typedef IF<(SPEED_POWER_MAX > 255), uint16_t, uint8_t>::type cutter_cpower_t;

#if CUTTER_UNIT_IS(RPM) && SPEED_POWER_MAX > 255
  typedef uint16_t cutter_power_t;
  #define CUTTER_MENU_POWER_TYPE   uint16_5
  #define cutter_power2str         ui16tostr5rj
#else
  typedef uint8_t cutter_power_t;
  #if CUTTER_UNIT_IS(PERCENT)
    #define CUTTER_MENU_POWER_TYPE percent_3
    #define cutter_power2str       pcttostrpctrj
  #else
    #define CUTTER_MENU_POWER_TYPE uint8
    #define cutter_power2str       ui8tostr3rj
  #endif
#endif

#if ENABLED(LASER_FEATURE)
  typedef uint16_t cutter_test_pulse_t;
  #define CUTTER_MENU_PULSE_TYPE uint16_3
#endif

#if ENABLED(MARLIN_DEV_MODE)
  typedef uint16_t cutter_frequency_t;
  #define CUTTER_MENU_FREQUENCY_TYPE uint16_5
#endif
