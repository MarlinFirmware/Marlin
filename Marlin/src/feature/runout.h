/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * feature/runout.h - Runout sensor support
 */

//#define FILAMENT_SENSOR_DEBUG

#include "../sd/cardreader.h"
#include "../module/printcounter.h"
#include "../module/stepper.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extensible_ui/ui_api.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "pause.h"
#endif

#if FRS(PAT9125)
  #include "runout/PAT9125Sensor.h"
#elif FRS(ENCODER)
  #include "runout/EncoderSensor.h"
#elif FRS(SWITCH)
  #include "runout/SwitchSensor.h"
#else
  #error Unhandled runout sensor type
#endif

class FilamentMonitorBase {
  public:
    static bool enabled, filament_ran_out;

    #if ENABLED(HOST_ACTION_COMMANDS)
      static bool host_handling;
    #else
      static constexpr bool host_handling = false;
    #endif

    #if HAS_DELAYED_RUNOUT
      static int16_t st_cnt;
    #endif
};

template<class RESPONSE_T, class SENSOR_T>
class TFilamentMonitor : public FilamentMonitorBase {
  private:
    typedef RESPONSE_T response_t;
    typedef SENSOR_T sensor_t;
    static response_t response;
    static sensor_t sensor;

  public:
    static inline void setup() {
      sensor.setup();
      reset();
    }

    static inline void reset() {
      filament_ran_out = false;
      sensor.reset();
      response.reset();
    }

    #if HAS_FILAMENT_MOTION_SENSOR
      static inline void updateSensorResolutionSteps() { sensor.initResolutionSteps(); }
    #endif

    #if HAS_DELAYED_RUNOUT
      static inline int16_t sensorResolutionSteps() { return sensor.getResolutionSteps(); }

      static inline void block_begin(const block_t* const bl) {
        if (!enabled || !st_cnt) return;
        if ((st_cnt > 0) != TEST(bl->direction_bits, E_AXIS))  // Direction change
          checkRunout();
      }

      static inline void block_chunk(const block_t* const bl, const int cnt) {
        if (!enabled || !cnt) return;
        st_cnt += TEST(bl->direction_bits, E_AXIS) ? -cnt : cnt;
        if (st_cnt >= sensorResolutionSteps() || st_cnt <= -sensorResolutionSteps())
          checkRunout();
      }

      static inline void checkRunout() {
        #if HAS_FILAMENT_MOTION_SENSOR
            sensor.checkMotion();
        #endif
        if (sensor.getRunoutState())
          response.updateRunoutDistance(st_cnt *
            #if ENABLED(DISTINCT_E_FACTORS)
              planner.settings.axis_steps_per_mm[E_AXIS + active_extruder]
            #else
              planner.settings.axis_steps_per_mm[E_AXIS]
            #endif
          );
        else
          response.reset();
        st_cnt = 0;
      }
    #endif

    // Give the response a chance to update its counter.
    static inline void run() {
      if (enabled && !filament_ran_out && (IS_SD_PRINTING() || print_job_timer.isRunning() || did_pause_print)) {
        if (sensor.getRunoutState()) {
          if (response.getRunoutState()) {
            filament_ran_out = true;
            event_filament_runout();
            planner.synchronize();
          }
        }
      #if !HAS_DELAYED_RUNOUT
        else
          response.reset();
      #endif
      }
    }
};

/********************************* RESPONSE TYPE *********************************/

#if HAS_DELAYED_RUNOUT

// RunoutResponseDelayed triggers a runout event after a specified amount of filament has been fed.
// If FILAMENT_RUNOUT_DISTANCE_MM > 0 runout is fired after such distance else is
// fired after sensor sensibility distance
class RunoutResponseDelayed {
  private:
    static float runoutDistance;

  public:
    static inline void reset() { runoutDistance = 0; }

    static inline void updateRunoutDistance(const float distance) { runoutDistance += distance; }

    static inline bool getRunoutState() {
      #if FILAMENT_RUNOUT_DISTANCE_MM <= 0
        return true;
      #else
        return runoutDistance >= FILAMENT_RUNOUT_DISTANCE_MM;
      #endif
    }
};

#else // !HAS_DELAYED_RUNOUT

// RunoutResponseDebounced triggers a runout event after a runout
// condition has been detected runout_threshold times in a row.
class RunoutResponseDebounced {
  private:
    static constexpr int8_t runout_threshold = 5;
    static int8_t runout_count;

  public:
    static inline void reset() { runout_count = runout_threshold; }

    static inline bool getRunoutState() { return --runout_count <= 0; }
};

#endif // !HAS_DELAYED_RUNOUT

/********************************* TEMPLATE SPECIALIZATION *********************************/

typedef TFilamentMonitor<
  #if HAS_DELAYED_RUNOUT
    RunoutResponseDelayed,
  #else
    RunoutResponseDebounced,
  #endif
  #if FRS(PAT9125)
    FilamentSensorPAT9125
  #elif FRS(ENCODER)
    FilamentSensorEncoder
  #elif FRS(SWITCH)
    FilamentSensorSwitch
  #else
    #error Unhandled runout sensor type
  #endif
> FilamentMonitor;

extern FilamentMonitor runout;
