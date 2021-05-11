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

#if ENABLED(ASSISTED_TRAMMING)

#include "tramming.h"

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../core/debug_out.h"

PGMSTR(point_name_1, TRAMMING_POINT_NAME_1);
PGMSTR(point_name_2, TRAMMING_POINT_NAME_2);
PGMSTR(point_name_3, TRAMMING_POINT_NAME_3);
#ifdef TRAMMING_POINT_NAME_4
  PGMSTR(point_name_4, TRAMMING_POINT_NAME_4);
  #ifdef TRAMMING_POINT_NAME_5
    PGMSTR(point_name_5, TRAMMING_POINT_NAME_5);
    #ifdef TRAMMING_POINT_NAME_6
      PGMSTR(point_name_6, TRAMMING_POINT_NAME_6);
    #endif
  #endif
#endif

PGM_P const tramming_point_name[] PROGMEM = {
  point_name_1, point_name_2, point_name_3
  #ifdef TRAMMING_POINT_NAME_4
    , point_name_4
    #ifdef TRAMMING_POINT_NAME_5
      , point_name_5
      #ifdef TRAMMING_POINT_NAME_6
        , point_name_6
      #endif
    #endif
  #endif
};

#ifdef ASSISTED_TRAMMING_WAIT_POSITION

  // Move to the defined wait position
  void move_to_tramming_wait_pos() {
    constexpr xyz_pos_t wait_pos = ASSISTED_TRAMMING_WAIT_POSITION;
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("Moving away");
    do_blocking_move_to(wait_pos, XY_PROBE_FEEDRATE_MM_S);
  }

#endif

#endif // ASSISTED_TRAMMING
