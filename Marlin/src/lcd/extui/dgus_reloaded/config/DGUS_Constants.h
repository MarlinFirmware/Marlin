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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../../inc/MarlinConfigPre.h"

#include "DGUS_Addr.h"

static_assert((DGUS_LEVEL_GRID_SIZE == GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y), "DGUS_LEVEL_GRID_SIZE incompatible with current mesh.");

#ifndef DGUS_DEFAULT_VOLUME
  #define DGUS_DEFAULT_VOLUME 50
#endif

#ifndef DGUS_DEFAULT_BRIGHTNESS
  #define DGUS_DEFAULT_BRIGHTNESS 100
#endif

#ifndef DGUS_STATUS_EXPIRATION_MS
  #define DGUS_STATUS_EXPIRATION_MS 30000
#endif

#ifndef DGUS_PRINT_BABYSTEP
  #define DGUS_PRINT_BABYSTEP 0.01f
#endif

#ifndef DGUS_PLA_TEMP_HOTEND
  #define DGUS_PLA_TEMP_HOTEND 200
#endif

#ifndef DGUS_PLA_TEMP_BED
  #define DGUS_PLA_TEMP_BED 60
#endif

#ifndef DGUS_ABS_TEMP_HOTEND
  #define DGUS_ABS_TEMP_HOTEND 240
#endif

#ifndef DGUS_ABS_TEMP_BED
  #define DGUS_ABS_TEMP_BED 80
#endif

#ifndef DGUS_PETG_TEMP_HOTEND
  #define DGUS_PETG_TEMP_HOTEND 240
#endif

#ifndef DGUS_PETG_TEMP_BED
  #define DGUS_PETG_TEMP_BED 60
#endif

#ifndef DGUS_DEFAULT_FILAMENT_LEN
  #define DGUS_DEFAULT_FILAMENT_LEN 10
#endif

#ifndef LEVEL_CORNERS_Z_HOP
  #define LEVEL_CORNERS_Z_HOP 4.0
#endif

#ifndef LEVEL_CORNERS_HEIGHT
  #define LEVEL_CORNERS_HEIGHT 0.0
#endif

static_assert(LEVEL_CORNERS_Z_HOP >= 0, "LEVEL_CORNERS_Z_HOP must be >= 0. Please update your configuration.");

#ifndef DGUS_LEVEL_CENTER_X
  #define DGUS_LEVEL_CENTER_X ((X_BED_SIZE) / 2)
#endif

#ifndef DGUS_LEVEL_CENTER_Y
  #define DGUS_LEVEL_CENTER_Y ((Y_BED_SIZE) / 2)
#endif

#if ENABLED(BLTOUCH)
  #ifndef DGUS_RESET_BLTOUCH
    #define DGUS_RESET_BLTOUCH "M999\nM280P0S160"
  #endif
#endif
