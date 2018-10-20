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

/**
 * feature/runout.h - Runout sensor support
 */

#include "../sd/cardreader.h"
#include "../module/printcounter.h"
#include "../module/stepper.h"
#include "../gcode/queue.h"

#include "../inc/MarlinConfig.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extensible_ui/ui_api.h"
#endif

//#define FILAMENT_RUNOUT_SENSOR_DEBUG

class FilamentSensorBase {
  public:
    static bool enabled;

  protected:
    static bool filament_ran_out;
};

template<class RESPONSE_T, class SENSOR_T>
class TFilamentSensor : public FilamentSensorBase {
  private:
    typedef RESPONSE_T response_t;
    typedef SENSOR_T   sensor_t;
    static response_t response;
    static sensor_t   sensor;

  public:
    static void setup() {
      sensor.setup();
    }

    inline static void reset() {
      filament_ran_out = false;
      response.reset();
    }

    // The sensor calls this method when filament is present
    inline static void filament_present(const uint8_t extruder) {
      response.filament_present(extruder);
    }

    inline static void block_complete(const block_t *b) {
      response.block_complete(b);
      sensor.block_complete(b);
    }

    static void run() {
      if (enabled && !filament_ran_out && (IS_SD_PRINTING() || print_job_timer.isRunning())) {
        response.run();
        sensor.run();
        if (response.has_runout()) {
          filament_ran_out = true;
          #if ENABLED(EXTENSIBLE_UI)
            UI::onFilamentRunout();
          #endif
          enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
          planner.synchronize();
        }
      }
    }
};

/*************************** FILAMENT PRESENCE SENSORS ***************************/

class FilamentSensorTypeBase {
  protected:
    static void filament_present(const uint8_t extruder);

  public:
    static void setup() {
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

    #if FIL_RUNOUT_INVERTING
      #define FIL_RUNOUT_INVERT_MASK (_BV(NUM_RUNOUT_SENSORS) - 1)
    #else
      #define FIL_RUNOUT_INVERT_MASK 0
    #endif

    // Return a bitmask of all runout sensor states
    static uint8_t poll_runout_pins() {
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
      ) ^ FIL_RUNOUT_INVERT_MASK;
    }
};

/**
 * This sensor is a simple endstop
 * switch in the path of the filament. It detects
 * filament runout, but not stripouts or jams.
 */

class FilamentSensorTypeSwitch : public FilamentSensorTypeBase {
  private:
    static bool poll_runout_pin(const uint8_t extruder) {
      const uint8_t runout_bits = poll_runout_pins();
      #if NUM_RUNOUT_SENSORS == 1
        return runout_bits;                     // A single sensor applying to all extruders
      #else
        #if ENABLED(DUAL_X_CARRIAGE)
          if (dual_x_carriage_mode == DXC_DUPLICATION_MODE || dual_x_carriage_mode == DXC_SCALED_DUPLICATION_MODE)
            return runout_bits;                 // Any extruder
          else
        #elif ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
          if (extruder_duplication_enabled)
            return runout_bits;                 // Any extruder
          else
        #endif
            return TEST(runout_bits, extruder); // Specific extruder
      #endif
    }

  public:
    FORCE_INLINE static void block_complete(const block_t *b) {}

    FORCE_INLINE static void run() {
      if (!poll_runout_pin(active_extruder))
        filament_present(active_extruder);
    }
};

// This filament sensor uses a magnetic encoder disc and a hall
// effect sensor (or a slitted disc and an optical sensor). The state
// will toggle between 0 and 1 with filament movement. It can detect
// filament runout and stripouts or jams.

class FilamentSensorTypeEncoder : public FilamentSensorTypeBase {
  private:
    static uint8_t motion_detected, old_state;

    static void poll_motion_sensor() {
      const uint8_t new_state = poll_runout_pins(),
                    change    = old_state ^ new_state;
      old_state = new_state;

      #ifdef FILAMENT_RUNOUT_SENSOR_DEBUG
        if (change) SERIAL_PROTOCOLLNPAIR("motion detected: ", change);
      #endif

      motion_detected |= change;
    }

  public:
    static void block_complete(const block_t *b) {
      // If the just-executed block caused the sensor wheel
      // to turn, reset the runout counter for that extruder.
      if (TEST(motion_detected, b->extruder))
        filament_present(b->extruder);

      // Clear motion triggers for next block
      motion_detected = 0;
    }

    FORCE_INLINE static void run() { poll_motion_sensor(); }
};

/********************************* RESPONSE TYPE *********************************/

#if FILAMENT_RUNOUT_DISTANCE_MM > 0

  // The RunoutResponseDelayed will trigger an runout event only after
  // RUNOUT_DISTANCE_MM of filament have been fed after a runout condition.
  class RunoutResponseDelayed {
    private:
      static int32_t steps_since_detection[EXTRUDERS];

      static float get_mm_since_runout(const uint8_t extruder) {
        return (steps_since_detection[extruder] / planner.settings.axis_steps_per_mm[E_AXIS_N(extruder)]);
      }

    public:
      static float runout_distance_mm;

      FORCE_INLINE static bool has_runout() {
        return get_mm_since_runout(active_extruder) > runout_distance_mm;
      }

      static inline void filament_present(const uint8_t extruder) {
        steps_since_detection[extruder] = 0;
      }

      static inline void run() {
        #ifdef FILAMENT_RUNOUT_SENSOR_DEBUG
          static uint16_t r = 0;
          if ((r++ % 24000) == 0) {
            SERIAL_PROTOCOLPGM("mm since filament detection: ");
            LOOP_L_N(i, NUM_RUNOUT_SENSORS) {
              if (i > 0) SERIAL_PROTOCOLPGM(", ");
              SERIAL_PROTOCOL(get_mm_since_runout(i));
            }
            SERIAL_EOL();
          }
        #endif
      }

      static void reset() {
        LOOP_L_N(i, NUM_RUNOUT_SENSORS) steps_since_detection[i] = 0;
      }

      static inline void block_complete(const block_t *b) {
        steps_since_detection[b->extruder] += TEST(b->direction_bits, E_AXIS) ? -b->steps[E_AXIS] : b->steps[E_AXIS];
      }
  };

#else // !FILAMENT_RUNOUT_DISTANCE_MM

  // The RunoutResponseDebounced will trigger an runout event after
  // a runout condition is detected FIL_RUNOUT_THRESHOLD times in a row.

  class RunoutResponseDebounced {
    private:
      static constexpr uint8_t FIL_RUNOUT_THRESHOLD = 5;
      static uint8_t runout_count;
    public:
      FORCE_INLINE static bool has_runout()                             { return runout_count > FIL_RUNOUT_THRESHOLD; }
      FORCE_INLINE static void block_complete(const block_t *b)         {}
      FORCE_INLINE static void filament_present(const uint8_t extruder) { runout_count = 0; UNUSED(extruder); }
      FORCE_INLINE static void run()                                    { runout_count++; }
      FORCE_INLINE static void reset()                                  { runout_count = 0; }
  };

#endif // !FILAMENT_RUNOUT_DISTANCE_MM

/********************************* TEMPLATE SPECIALIZATION *********************************/

typedef TFilamentSensor<
  #if FILAMENT_RUNOUT_DISTANCE_MM > 0
    #if ENABLED(FILAMENT_MOTION_SENSOR)
      RunoutResponseDelayed, FilamentSensorTypeEncoder
    #else
      RunoutResponseDelayed, FilamentSensorTypeSwitch
    #endif
  #else
    RunoutResponseDebounced, FilamentSensorTypeSwitch
  #endif
> FilamentRunoutSensor;

extern FilamentRunoutSensor runout;
