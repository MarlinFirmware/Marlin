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

/**
 * feature/z_stepper_align.cpp
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(Z_STEPPER_AUTO_ALIGN)

#include "z_stepper_align.h"
#include "../module/probe.h"

ZStepperAlign z_stepper_align;

xy_pos_t ZStepperAlign::xy[NUM_Z_STEPPER_DRIVERS];

#if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
  xy_pos_t ZStepperAlign::stepper_xy[NUM_Z_STEPPER_DRIVERS];
#endif

void ZStepperAlign::reset_to_default() {
  #ifdef Z_STEPPER_ALIGN_XY

    constexpr xy_pos_t xy_init[] = Z_STEPPER_ALIGN_XY;
    static_assert(COUNT(xy_init) == NUM_Z_STEPPER_DRIVERS,
      "Z_STEPPER_ALIGN_XY requires "
      #if NUM_Z_STEPPER_DRIVERS == 4
        "four {X,Y} entries (Z, Z2, Z3, and Z4)."
      #elif NUM_Z_STEPPER_DRIVERS == 3
        "three {X,Y} entries (Z, Z2, and Z3)."
      #else
        "two {X,Y} entries (Z and Z2)."
      #endif
    );

    #define VALIDATE_ALIGN_POINT(N) static_assert(N >= NUM_Z_STEPPER_DRIVERS || Probe::build_time::can_reach(xy_init[N]), \
      "Z_STEPPER_ALIGN_XY point " STRINGIFY(N) " is not reachable with the default NOZZLE_TO_PROBE offset and PROBING_MARGIN.")
    VALIDATE_ALIGN_POINT(0); VALIDATE_ALIGN_POINT(1); VALIDATE_ALIGN_POINT(2); VALIDATE_ALIGN_POINT(3);

  #else // !Z_STEPPER_ALIGN_XY

    const xy_pos_t xy_init[] = {
      #if NUM_Z_STEPPER_DRIVERS >= 3  // First probe point...
        #if !Z_STEPPERS_ORIENTATION
          { probe.min_x(), probe.min_y() }, // SW
        #elif Z_STEPPERS_ORIENTATION == 1
          { probe.min_x(), probe.max_y() }, // NW
        #elif Z_STEPPERS_ORIENTATION == 2
          { probe.max_x(), probe.max_y() }, // NE
        #elif Z_STEPPERS_ORIENTATION == 3
          { probe.max_x(), probe.min_y() }, // SE
        #else
          #error "Z_STEPPERS_ORIENTATION must be from 0 to 3 (first point SW, NW, NE, SE)."
        #endif
        #if NUM_Z_STEPPER_DRIVERS == 4    // 3 more points...
          #if !Z_STEPPERS_ORIENTATION
            { probe.min_x(), probe.max_y() }, { probe.max_x(), probe.max_y() }, { probe.max_x(), probe.min_y() }  // SW
          #elif Z_STEPPERS_ORIENTATION == 1
            { probe.max_x(), probe.max_y() }, { probe.max_x(), probe.min_y() }, { probe.min_x(), probe.min_y() }  // NW
          #elif Z_STEPPERS_ORIENTATION == 2
            { probe.max_x(), probe.min_y() }, { probe.min_x(), probe.min_y() }, { probe.min_x(), probe.max_y() }  // NE
          #elif Z_STEPPERS_ORIENTATION == 3
            { probe.min_x(), probe.min_y() }, { probe.min_x(), probe.max_y() }, { probe.max_x(), probe.max_y() }  // SE
          #endif
        #elif !Z_STEPPERS_ORIENTATION     // or 2 more points...
          { probe.max_x(), probe.min_y() }, { X_CENTER, probe.max_y() } // SW
        #elif Z_STEPPERS_ORIENTATION == 1
          { probe.min_x(), probe.min_y() }, { probe.max_x(), Y_CENTER } // NW
        #elif Z_STEPPERS_ORIENTATION == 2
          { probe.min_x(), probe.max_y() }, { X_CENTER, probe.min_y() } // NE
        #elif Z_STEPPERS_ORIENTATION == 3
          { probe.max_x(), probe.max_y() }, { probe.min_x(), Y_CENTER } // SE
        #endif
      #elif Z_STEPPERS_ORIENTATION
        { X_CENTER, probe.min_y() }, { X_CENTER, probe.max_y() }
      #else
        { probe.min_x(), Y_CENTER }, { probe.max_x(), Y_CENTER }
      #endif
    };

  #endif // !Z_STEPPER_ALIGN_XY

  COPY(xy, xy_init);

  #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
    constexpr xy_pos_t stepper_xy_init[] = Z_STEPPER_ALIGN_STEPPER_XY;
    static_assert(
      COUNT(stepper_xy_init) == NUM_Z_STEPPER_DRIVERS,
      "Z_STEPPER_ALIGN_STEPPER_XY requires "
      #if NUM_Z_STEPPER_DRIVERS == 4
        "four {X,Y} entries (Z, Z2, Z3, and Z4)."
      #elif NUM_Z_STEPPER_DRIVERS == 3
        "three {X,Y} entries (Z, Z2, and Z3)."
      #endif
    );
    COPY(stepper_xy, stepper_xy_init);
  #endif
}

#endif // Z_STEPPER_AUTO_ALIGN
