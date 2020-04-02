/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#if CUTTER_DISPLAY_IS(RPM) && SPEED_POWER_MAX > 255
  #define cutter_power_t              uint16_t
  #define cutter_setPower_t           uint16_t
  #define CUTTER_MENU_POWER_TYPE      uint16_5
#else
  #define cutter_power_t              uint8_t
  #define cutter_setPower_t           uint8_t
  #define CUTTER_MENU_POWER_TYPE      uint8
#endif

#if ENABLED(MARLIN_DEV_MODE)
  #define cutter_frequency_t          uint16_t
  #define CUTTER_MENU_FREQUENCY_TYPE  uint16_5
#endif
