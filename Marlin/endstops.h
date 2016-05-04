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
 *  endstops.h - manages endstops
 */

#ifndef ENDSTOPS_H
#define ENDSTOPS_H

enum EndstopEnum {X_MIN = 0, Y_MIN = 1, Z_MIN = 2, Z_MIN_PROBE = 3, X_MAX = 4, Y_MAX = 5, Z_MAX = 6, Z2_MIN = 7, Z2_MAX = 8};

class Endstops {

  public:

    volatile char endstop_hit_bits; // use X_MIN, Y_MIN, Z_MIN and Z_MIN_PROBE as BIT value

    #if ENABLED(Z_DUAL_ENDSTOPS)
      uint16_t current_endstop_bits = 0,
                   old_endstop_bits = 0;
    #else
      byte current_endstop_bits = 0,
               old_endstop_bits = 0;
    #endif
        

    bool enabled = true;
    bool enabled_globally =
      #if ENABLED(ENDSTOPS_ONLY_FOR_HOMING)
        false
      #else
        true
      #endif
    ;

    Endstops();

    /**
     * Initialize the endstop pins
     */
    void init();

    /**
     * Update the endstops bits from the pins
     */
    void update();

    /**
     * Print an error message reporting the position when the endstops were last hit.
     */
    void report_state(); //call from somewhere to create an serial error message with the locations the endstops where hit, in case they were triggered

    /**
     * Report endstop positions in response to M119
     */
    void M119();

    // Enable / disable endstop checking globally
    FORCE_INLINE void enable_globally(bool onoff=true) { enabled_globally = enabled = onoff; }

    // Enable / disable endstop checking
    FORCE_INLINE void enable(bool onoff=true) { enabled = onoff; }

    // Disable / Enable endstops based on ENSTOPS_ONLY_FOR_HOMING and global enable
    FORCE_INLINE void not_homing() { enabled = enabled_globally; }

    // Clear endstops (i.e., they were hit intentionally) to suppress the report
    FORCE_INLINE void hit_on_purpose() { endstop_hit_bits = 0; }

    // Enable / disable endstop z-probe checking
    #if HAS_BED_PROBE
      volatile bool z_probe_enabled = false;
      FORCE_INLINE void enable_z_probe(bool onoff=true) { z_probe_enabled = onoff; }
    #endif

  private:

    #if ENABLED(Z_DUAL_ENDSTOPS)
      void test_dual_z_endstops(EndstopEnum es1, EndstopEnum es2);
    #endif
};

extern Endstops endstops;

#endif // ENDSTOPS_H
