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
#include "../../module/stepper.h"

/**
 * This sensor uses a magnetic encoder disc and a Hall effect
 * sensor (or a slotted disc and optical sensor). The state
 * will toggle between 0 and 1 on filament movement. It can detect
 * filament runout and stripouts or jams.
 */
class FilamentSensorEncoder : private DIFilamentSensorBase {
  private:
    #define ActiveExtruderSensor()  MIN(active_extruder, NUM_RUNOUT_SENSORS - 1)

    constexpr static float sensorResolution =
      #ifdef SENSOR_RESOLUTION
        SENSOR_RESOLUTION
      #else
        2.5
      #endif
    ;

    static uint8_t old_state, changed, cnt, motion_detected;
    #if ENABLED(DISTINCT_E_FACTORS)
      static int16_t resolutionSteps[NUM_RUNOUT_SENSORS];
    #else
      static int16_t resolutionSteps[1];
    #endif

  public:
    static inline bool setup() {
      DIFilamentSensorBase::setup();
      initResolutionSteps();
      return true;
    }

    static inline void initResolutionSteps() {
      #if ENABLED(DISTINCT_E_FACTORS)
        for (uint8_t e = 0; e < NUM_RUNOUT_SENSORS; ++e)
      #else
        constexpr static uint8_t e = 0;
      #endif
          resolutionSteps[e] = (int16_t)(sensorResolution * planner.settings.axis_steps_per_mm[E_AXIS+e] * 2 / 3);
    }

    static inline int16_t getResolutionSteps() {
      #if ENABLED(DISTINCT_E_FACTORS)
        return resolutionSteps[ActiveExtruderSensor()];
      #else
        return resolutionSteps[0];
      #endif
    }

    static inline void reset() {
      old_state = poll_runout_pins();
      motion_detected = _BV(NUM_RUNOUT_SENSORS) - 1;
      changed = cnt = 0;
    }

    static inline void checkMotion() {
      const uint8_t new_state = poll_runout_pins();
      changed |= old_state ^ new_state;
      old_state = new_state;

      #ifdef FILAMENT_SENSOR_DEBUG
        if (change) {
          SERIAL_ECHOPGM("Motion detected:");
          for (uint8_t e = 0; e < NUM_RUNOUT_SENSORS; ++e)
            if (TEST(changed, e)) {
              SERIAL_CHAR(' ');
              SERIAL_CHAR('0' + e);
            }
          SERIAL_EOL();
        }
      #endif

      if (++cnt >= 2) {
        motion_detected = changed;
        changed = cnt = 0;
      }
    }

    static inline bool getRunoutState() {
      return !TEST(motion_detected, ActiveExtruderSensor());
    }
};
