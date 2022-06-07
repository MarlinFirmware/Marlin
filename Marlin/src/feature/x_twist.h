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

<<<<<<<< HEAD:Marlin/src/feature/x_twist.h
#include "../inc/MarlinConfigPre.h"

typedef float xatc_array_t[XATC_MAX_POINTS];

class XATC {
  static bool enabled;
public:
  static float spacing, start;
  static xatc_array_t z_offset;

  static void reset();
  static void set_enabled(const bool ena) { enabled = ena; }
  static float compensation(const xy_pos_t &raw);
  static void print_points();
};

extern XATC xatc;
========
#define BOARD_INFO_NAME "FYSETC SPIDER V22"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#define TEMP_3_PIN                          PC3
#define TEMP_4_PIN                          PB1
#define TEMP_BED_PIN                        PB0

#define FAN_PIN                             PA13
#define FAN1_PIN                            PA14

#include "pins_FYSETC_SPIDER.h"
>>>>>>>> upstream/bugfix-2.0.x:Marlin/src/pins/stm32f4/pins_FYSETC_SPIDER_V2_2.h
