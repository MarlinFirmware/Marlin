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
 * endstops.h - manages endstops
 */

#ifndef __ENDSTOPS_H__
#define __ENDSTOPS_H__

#include "../inc/MarlinConfig.h"
#include <stdint.h>

enum EndstopEnum : char {
  X_MIN,
  Y_MIN,
  Z_MIN,
  Z_MIN_PROBE,
  X_MAX,
  Y_MAX,
  Z_MAX,
  X2_MIN,
  X2_MAX,
  Y2_MIN,
  Y2_MAX,
  Z2_MIN,
  Z2_MAX,
  Z3_MIN,
  Z3_MAX
};

class Endstops {

  public:

    #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS
      typedef uint16_t esbits_t;
      #if ENABLED(X_DUAL_ENDSTOPS)
        static float x2_endstop_adj;
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        static float y2_endstop_adj;
      #endif
      #if Z_MULTI_ENDSTOPS
        static float z2_endstop_adj;
      #endif
      #if ENABLED(Z_TRIPLE_ENDSTOPS)
        static float z3_endstop_adj;
      #endif
    #else
      typedef uint8_t esbits_t;
    #endif

  private:
    static bool enabled, enabled_globally;
    static esbits_t live_state;
    static volatile uint8_t hit_state;      // Use X_MIN, Y_MIN, Z_MIN and Z_MIN_PROBE as BIT index

    #if ENDSTOP_NOISE_THRESHOLD
      static esbits_t validated_live_state;
      static uint8_t endstop_poll_count;    // Countdown from threshold for polling
    #endif

  public:
    Endstops() {};

    /**
     * Initialize the endstop pins
     */
    static void init();

    /**
     * Are endstops or the probe set to abort the move?
     */
    FORCE_INLINE static bool abort_enabled() {
      return (enabled
        #if HAS_BED_PROBE
          || z_probe_enabled
        #endif
      );
    }

    /**
     * Periodic call to poll endstops if required. Called from temperature ISR
     */
    static void poll();

    /**
     * Update endstops bits from the pins. Apply filtering to get a verified state.
     * If abort_enabled() and moving towards a triggered switch, abort the current move.
     * Called from ISR contexts.
     */
    static void update();

    /**
     * Get Endstop hit state.
     */
    FORCE_INLINE static uint8_t trigger_state() { return hit_state; }

    /**
     * Get current endstops state
     */
    FORCE_INLINE static esbits_t state() {
      return
        #if ENDSTOP_NOISE_THRESHOLD
          validated_live_state
        #else
          live_state
        #endif
      ;
    }

    /**
     * Report endstop hits to serial. Called from loop().
     */
    static void event_handler();

    /**
     * Report endstop positions in response to M119
     */
    static void M119();

    // Enable / disable endstop checking globally
    static void enable_globally(const bool onoff=true);

    // Enable / disable endstop checking
    static void enable(const bool onoff=true);

    // Disable / Enable endstops based on ENSTOPS_ONLY_FOR_HOMING and global enable
    static void not_homing();

    #if ENABLED(VALIDATE_HOMING_ENDSTOPS)
      // If the last move failed to trigger an endstop, call kill
      static void validate_homing_move();
    #else
      FORCE_INLINE static void validate_homing_move() { hit_on_purpose(); }
    #endif

    // Clear endstops (i.e., they were hit intentionally) to suppress the report
    FORCE_INLINE static void hit_on_purpose() { hit_state = 0; }

    // Enable / disable endstop z-probe checking
    #if HAS_BED_PROBE
      static volatile bool z_probe_enabled;
      static void enable_z_probe(const bool onoff=true);
    #endif

    // Debugging of endstops
    #if ENABLED(PINS_DEBUGGING)
      static bool monitor_flag;
      static void monitor();
      static void run_monitor();
    #endif
};

extern Endstops endstops;

#endif // __ENDSTOPS_H__
