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

#ifndef __BEDLEVEL_H__
#define __BEDLEVEL_H__

#include "../../inc/MarlinConfig.h"

#if ENABLED(MESH_BED_LEVELING)
  #include "mbl/mesh_bed_leveling.h"
#elif ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl/ubl.h"
#elif HAS_ABL
  #include "abl/abl.h"
  //#include "../../libs/vector_3.h"
  #if ENABLED(AUTO_BED_LEVELING_LINEAR)
    #include "../../libs/least_squares_fit.h"
  #endif
#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR) || ENABLED(MESH_BED_LEVELING)

  #include <stdint.h>

  typedef float (*element_2d_fn)(const uint8_t, const uint8_t);

  /**
   * Print calibration results for plotting or manual frame adjustment.
   */
  void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, element_2d_fn fn);

#endif

#endif // __BEDLEVEL_H__
