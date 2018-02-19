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

/**
 * feature/runout.h - Runout sensor support
 */

#ifndef _RUNOUT_H_
#define _RUNOUT_H_

#include "../sd/cardreader.h"
#include "../module/printcounter.h"
#include "../module/stepper.h"
#include "../gcode/queue.h"

#include "../inc/MarlinConfig.h"

class FilamentRunoutSensor {

  FilamentRunoutSensor() {}

  static bool filament_ran_out;
  static void setup();

  FORCE_INLINE static reset() { filament_ran_out = false; }

  FORCE_INLINE static bool check() {
    #if NUM_RUNOUT_SENSORS < 2
      // A single sensor applying to all extruders
      return READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING;
    #else
      // Read the sensor for the active extruder
      switch (active_extruder) {
        case 0: return READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING;
        case 1: return READ(FIL_RUNOUT2_PIN) == FIL_RUNOUT_INVERTING;
        #if NUM_RUNOUT_SENSORS > 2
          case 2: return READ(FIL_RUNOUT3_PIN) == FIL_RUNOUT_INVERTING;
          #if NUM_RUNOUT_SENSORS > 3
            case 3: return READ(FIL_RUNOUT4_PIN) == FIL_RUNOUT_INVERTING;
            #if NUM_RUNOUT_SENSORS > 4
              case 4: return READ(FIL_RUNOUT5_PIN) == FIL_RUNOUT_INVERTING;
            #endif
          #endif
        #endif
      }
    #endif
    return false;
  }

  FORCE_INLINE static void run() {
    if ((IS_SD_PRINTING || print_job_timer.isRunning()) && check() && !filament_ran_out) {
      filament_ran_out = true;
      enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
      stepper.synchronize();
    }
  }

};

extern FilamentRunoutSensor runout;

#endif // _RUNOUT_H_
