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
#pragma once

#include "../../inc/MarlinConfig.h"

class DIFilamentSensorBase {
  protected:
    static inline void setup() {
      #if ENABLED(FIL_RUNOUT_PULLUP)
        #define INIT_RUNOUT_PIN(P) SET_INPUT_PULLUP(P)
      #elif ENABLED(FIL_RUNOUT_PULLDOWN)
        #define INIT_RUNOUT_PIN(P) SET_INPUT_PULLDOWN(P)
      #else
        #define INIT_RUNOUT_PIN(P) SET_INPUT(P)
      #endif

      INIT_RUNOUT_PIN(FIL_RUNOUT_PIN);
      #if NUM_RUNOUT_SENSORS > 1
        INIT_RUNOUT_PIN(FIL_RUNOUT2_PIN);
        #if NUM_RUNOUT_SENSORS > 2
          INIT_RUNOUT_PIN(FIL_RUNOUT3_PIN);
          #if NUM_RUNOUT_SENSORS > 3
            INIT_RUNOUT_PIN(FIL_RUNOUT4_PIN);
            #if NUM_RUNOUT_SENSORS > 4
              INIT_RUNOUT_PIN(FIL_RUNOUT5_PIN);
              #if NUM_RUNOUT_SENSORS > 5
                INIT_RUNOUT_PIN(FIL_RUNOUT6_PIN);
              #endif
            #endif
          #endif
        #endif
      #endif
    }

    static inline uint8_t poll_runout_pins() {
      return (
        (READ(FIL_RUNOUT_PIN ) ? _BV(0) : 0)
        #if NUM_RUNOUT_SENSORS > 1
          | (READ(FIL_RUNOUT2_PIN) ? _BV(1) : 0)
          #if NUM_RUNOUT_SENSORS > 2
            | (READ(FIL_RUNOUT3_PIN) ? _BV(2) : 0)
            #if NUM_RUNOUT_SENSORS > 3
              | (READ(FIL_RUNOUT4_PIN) ? _BV(3) : 0)
              #if NUM_RUNOUT_SENSORS > 4
                | (READ(FIL_RUNOUT5_PIN) ? _BV(4) : 0)
                #if NUM_RUNOUT_SENSORS > 5
                  | (READ(FIL_RUNOUT6_PIN) ? _BV(5) : 0)
                #endif
              #endif
            #endif
          #endif
        #endif
      );
    }
};
