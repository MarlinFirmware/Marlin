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

#include "DIFilamentSensorBase.h"
#include "../../module/motion.h"

/**
 * This is a simple endstop switch in the path of the filament.
 * It can detect filament runout, but not stripouts or jams.
 */
class FilamentSensorSwitch : private DIFilamentSensorBase {
  private:
    // Return a bitmask of runout flag states (bits set always indicates runout)
    static inline uint8_t poll_runout_states() {
      return poll_runout_pins() ^ uint8_t(
        #if DISABLED(FIL_RUNOUT_INVERTING)
          _BV(NUM_RUNOUT_SENSORS) - 1
        #else
          0
        #endif
      );
    }

    static inline bool poll_runout_state() {
      const uint8_t runout_states = poll_runout_states();
      #if NUM_RUNOUT_SENSORS == 1
        return runout_states;                     // A single sensor applying to all extruders
      #else
        #if ENABLED(DUAL_X_CARRIAGE)
          if (dual_x_carriage_mode == DXC_DUPLICATION_MODE || dual_x_carriage_mode == DXC_SCALED_DUPLICATION_MODE)
            return runout_states;                 // Any extruder
          else
        #elif ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
          if (extruder_duplication_enabled)
            return runout_states;                 // Any extruder
          else
        #endif
            return TEST(runout_states, active_extruder); // Specific extruder
      #endif
    }

  public:
    static inline bool setup() {
      DIFilamentSensorBase::setup();
      return true;
    }

    #if FILAMENT_RUNOUT_DISTANCE_MM > 0
      static inline int16_t getResolutionSteps() { return 0; }    // Can detect presence so 0 step resolution
    #endif

    static inline void reset() {}

    static inline bool getRunoutState() {
      const bool out = poll_runout_state();
      #ifdef FILAMENT_SENSOR_DEBUG
        static bool was_out = false;
        if (out != was_out) {
          was_out = out;
          SERIAL_ECHOPGM("Filament ");
          serialprintPGM(out ? PSTR("OUT\n") : PSTR("IN\n"));
        }
      #endif

      return out;
    }
};
