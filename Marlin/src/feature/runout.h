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

class FilamentMonitorBase {
  public:
    static bool enabled;

  protected:
    static bool filament_ran_out;
};

template<class RESPONSE_T, class SENSOR_T>
class TFilamentMonitor : public FilamentMonitorBase {
  private:
    typedef RESPONSE_T response_t;
    typedef SENSOR_T   sensor_t;
    static  response_t response;
    static  sensor_t   sensor;

  public:
    static inline void setup() {
      sensor.setup();
      reset();
    }

    static inline void reset() {
      filament_ran_out = false;
      response.reset();
    }

    // Call this method when filament is present,
    // so the response can reset its counter.
    static inline void filament_present(const uint8_t extruder) {
      response.filament_present(extruder);
    }

    // Handle a block completion. RunoutResponseDelayed uses this to
    // add up the length of filament moved while the filament is out.
    static inline void block_completed(const block_t* const b) {
      if (enabled) {
        response.block_completed(b);
        sensor.block_completed(b);
      }
    }

    // Give the response a chance to update its counter.
    static inline void run() {
      if (enabled && !filament_ran_out && (IS_SD_PRINTING() || print_job_timer.isRunning())) {
        #if FILAMENT_RUNOUT_DISTANCE_MM > 0
          cli(); // Prevent RunoutResponseDelayed::block_completed from accumulating here
        #endif
        response.run();
        sensor.run();
        const bool ran_out = response.has_run_out();
        #if FILAMENT_RUNOUT_DISTANCE_MM > 0
          sei();
        #endif
        if (ran_out) {
          filament_ran_out = true;
          #if ENABLED(EXTENSIBLE_UI)
            ExtUI::onFilamentRunout();
          #endif
          enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
          planner.synchronize();
        }
      }
    }
};

/*************************** FILAMENT PRESENCE SENSORS ***************************/

class FilamentSensorBase {
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

#if ENABLED(FILAMENT_MOTION_SENSOR)

  /**
   * This sensor uses a magnetic encoder disc and a Hall effect
   * sensor (or a slotted disc and optical sensor). The state
   * will toggle between 0 and 1 on filament movement. It can detect
   * filament runout and stripouts or jams.
   */
  class FilamentSensorEncoder : public FilamentSensorBase {
    private:
      static uint8_t motion_detected;

      static inline void poll_motion_sensor() {
        static uint8_t old_state;
        const uint8_t new_state = poll_runout_pins(),
                      change    = old_state ^ new_state;
        old_state = new_state;

        #ifdef FILAMENT_RUNOUT_SENSOR_DEBUG
          if (change) SERIAL_PROTOCOLLNPAIR("Motion detected: ", int(change));
        #endif

        motion_detected |= change;
      }

    public:
      static inline void block_completed(const block_t* const b) {
        // If the sensor wheel has moved since the last call to
        // this method reset the runout counter for the extruder.
        if (TEST(motion_detected, b->extruder))
          filament_present(b->extruder);

        // Clear motion triggers for next block
        motion_detected = 0;
      }

      static inline void run() { poll_motion_sensor(); }
  };

#else

  /**
   * This is a simple endstop switch in the path of the filament.
   * It can detect filament runout, but not stripouts or jams.
   */
  class FilamentSensorSwitch : public FilamentSensorBase {
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
      static inline void block_completed(const block_t* const b) {}

      static inline void run() {
        const bool out = poll_runout_pin(active_extruder);
        if (!out) filament_present(active_extruder);
        #ifdef FILAMENT_RUNOUT_SENSOR_DEBUG
          static bool was_out = false;
          if (out != was_out) {
            was_out = out;
            SERIAL_PROTOCOL("Filament ");
            serialprintPGM(out ? PSTR("OUT\n") : PSTR("IN\n"));
          }
        #endif
      }
  };


#endif // !FILAMENT_MOTION_SENSOR

/********************************* RESPONSE TYPE *********************************/

#if FILAMENT_RUNOUT_DISTANCE_MM > 0

  // RunoutResponseDelayed triggers a runout event only if the length
  // of filament specified by FILAMENT_RUNOUT_DISTANCE_MM has been fed
  // during a runout condition.
  class RunoutResponseDelayed {
    private:
      static volatile float runout_mm_countdown[EXTRUDERS];

    public:
      static float runout_distance_mm;

      static void reset() {
        LOOP_L_N(i, EXTRUDERS) filament_present(i);
      }

      static inline void run() {
        #ifdef FILAMENT_RUNOUT_SENSOR_DEBUG
          static millis_t t = 0;
          const millis_t ms = millis();
          if (ELAPSED(ms, t)) {
            t = millis() + 1000UL;
            LOOP_L_N(i, EXTRUDERS) {
              serialprintPGM(i ? PSTR(", ") : PSTR("Remaining mm: "));
              SERIAL_PROTOCOL(runout_mm_countdown[i]);
            }
            SERIAL_EOL();
          }
        #endif
      }

      static inline bool has_run_out() {
        return runout_mm_countdown[active_extruder] < 0;
      }

      static inline void filament_present(const uint8_t extruder) {
        runout_mm_countdown[extruder] = runout_distance_mm;
      }

      static inline void block_completed(const block_t* const b) {
        const uint8_t e = b->extruder;
        const int32_t steps = b->steps[E_AXIS];
        runout_mm_countdown[e] -= (TEST(b->direction_bits, E_AXIS) ? -steps : steps) * planner.steps_to_mm[E_AXIS_N(e)];
      }
  };

#else // !FILAMENT_RUNOUT_DISTANCE_MM

  // RunoutResponseDebounced triggers a runout event after a runout
  // condition has been detected runout_threshold times in a row.

  class RunoutResponseDebounced {
    private:
      static constexpr int8_t runout_threshold = 5;
      static int8_t runout_count;
    public:
      static inline void reset()                                  { runout_count = runout_threshold; }
      static inline void run()                                    { runout_count--; }
      static inline bool has_run_out()                            { return runout_count < 0; }
      static inline void block_completed(const block_t* const b)  {}
      static inline void filament_present(const uint8_t extruder) { runout_count = runout_threshold; UNUSED(extruder); }
  };

#endif // !FILAMENT_RUNOUT_DISTANCE_MM

/********************************* TEMPLATE SPECIALIZATION *********************************/

typedef TFilamentMonitor<
  #if FILAMENT_RUNOUT_DISTANCE_MM > 0
    #if ENABLED(FILAMENT_MOTION_SENSOR)
      RunoutResponseDelayed, FilamentSensorEncoder
    #else
      RunoutResponseDelayed, FilamentSensorSwitch
    #endif
  #else
    RunoutResponseDebounced, FilamentSensorSwitch
  #endif
> FilamentMonitor;

extern FilamentMonitor runout;
