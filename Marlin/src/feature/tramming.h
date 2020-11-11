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

#include "../inc/MarlinConfigPre.h"

#if !WITHIN(TRAMMING_SCREW_THREAD, 30, 51) || TRAMMING_SCREW_THREAD % 10 > 1
  #error "TRAMMING_SCREW_THREAD must be equal to 30, 31, 40, 41, 50, or 51."
#endif

constexpr xy_pos_t screws_tilt_adjust_pos[] = TRAMMING_POINT_XY;

#define G35_PROBE_COUNT COUNT(screws_tilt_adjust_pos)
static_assert(G35_PROBE_COUNT >= 3, "TRAMMING_POINT_XY requires at least 3 XY positions.");

extern const char point_name_1[], point_name_2[], point_name_3[]
  #ifdef TRAMMING_POINT_NAME_4
    , point_name_4[]
    #ifdef TRAMMING_POINT_NAME_5
      , point_name_5[]
    #endif
  #endif
;

#define _NR_TRAM_NAMES 2
#ifdef TRAMMING_POINT_NAME_3
  #undef _NR_TRAM_NAMES
  #define _NR_TRAM_NAMES 3
  #ifdef TRAMMING_POINT_NAME_4
    #undef _NR_TRAM_NAMES
    #define _NR_TRAM_NAMES 4
    #ifdef TRAMMING_POINT_NAME_5
      #undef _NR_TRAM_NAMES
      #define _NR_TRAM_NAMES 5
    #endif
  #endif
#endif
static_assert(_NR_TRAM_NAMES >= G35_PROBE_COUNT, "Define enough TRAMMING_POINT_NAME_s for all TRAMMING_POINT_XY entries.");
#undef _NR_TRAM_NAMES

extern PGM_P const tramming_point_name[];
