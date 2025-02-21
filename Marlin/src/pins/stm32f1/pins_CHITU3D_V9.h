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

#define BOARD_INFO_NAME "Chitu3D V9"

#define Z_STOP_PIN                          PA14

#define Z2_ENABLE_PIN                       PF3
#define Z2_STEP_PIN                         PF5
#define Z2_DIR_PIN                          PF1
#define Z2_STOP_PIN                         PA13

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PG9
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PF13
#endif

#include "pins_CHITU3D_common.h"
