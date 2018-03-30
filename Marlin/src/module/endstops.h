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
  Z2_MAX
};

class Endstops {

  public:

    static bool enabled, enabled_globally;
    static volatile char endstop_hit_bits; // use X_MIN, Y_MIN, Z_MIN and Z_MIN_PROBE as BIT value

    #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
      typedef uint16_t esbits_t;
      #if ENABLED(X_DUAL_ENDSTOPS)
        static float x_endstop_adj;
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        static float y_endstop_adj;
      #endif
      #if ENABLED(Z_DUAL_ENDSTOPS)
        static float z_endstop_adj;
      #endif
    #else
      typedef byte esbits_t;
    #endif

    static esbits_t current_endstop_bits, old_endstop_bits;

    Endstops() {
      enable_globally(
        #if ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
          true
        #else
          false
        #endif
      );
    };

    /**
     * Initialize the endstop pins
     */
    static void init();

    /**
     * Update the endstops bits from the pins
     */
    static void update();

    /**
     * Print an error message reporting the position when the endstops were last hit.
     */
    static void report_state(); //call from somewhere to create an serial error message with the locations the endstops where hit, in case they were triggered

    /**
     * Report endstop positions in response to M119
     */
    static void M119();

    // Enable / disable endstop checking globally
    static void enable_globally(bool onoff=true) { enabled_globally = enabled = onoff; }

    // Enable / disable endstop checking
    static void enable(bool onoff=true) { enabled = onoff; }

    // Disable / Enable endstops based on ENSTOPS_ONLY_FOR_HOMING and global enable
    static void not_homing() { enabled = enabled_globally; }

    // Clear endstops (i.e., they were hit intentionally) to suppress the report
    static void hit_on_purpose() { endstop_hit_bits = 0; }

    // Enable / disable endstop z-probe checking
    #if HAS_BED_PROBE
      static volatile bool z_probe_enabled;
      static void enable_z_probe(bool onoff=true) { z_probe_enabled = onoff; }
    #endif

    // Debugging of endstops
    #if ENABLED(PINS_DEBUGGING)
      static bool monitor_flag;
      static void monitor();
      FORCE_INLINE static void run_monitor() {
        if (!monitor_flag) return;
        static uint8_t monitor_count = 16;  // offset this check from the others
        monitor_count += _BV(1);            //  15 Hz
        monitor_count &= 0x7F;
        if (!monitor_count) monitor();      // report changes in endstop status
      }
    #endif

  private:

    #if ENABLED(X_DUAL_ENDSTOPS)
      static void test_dual_x_endstops(const EndstopEnum es1, const EndstopEnum es2);
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      static void test_dual_y_endstops(const EndstopEnum es1, const EndstopEnum es2);
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      static void test_dual_z_endstops(const EndstopEnum es1, const EndstopEnum es2);
    #endif
};

extern Endstops endstops;

#if HAS_BED_PROBE
  #define ENDSTOPS_ENABLED  (endstops.enabled || endstops.z_probe_enabled)
#else
  #define ENDSTOPS_ENABLED  endstops.enabled
#endif

#endif // __ENDSTOPS_H__
