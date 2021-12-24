/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC_CONFIG

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper/indirection.h"

#define DEBUG_OUT ENABLED(MARLIN_DEV_MODE)
#include "../../../core/debug_out.h"

/**
 * M919: Set TMC stepper drive chopper times
 *
 * Parameters:
 *   XYZIJKE...
 *   I[index]    - Axis sub-index (Omit or 0 for X, Y, Z, E0; 1 for X2, Y2, Z2, E1; 2 for Z3, E2; 3 for Z4, E3; 4 for E4; 5 for E5; 6 for E6; 7 for E7)
 *   T[index]    - Extruder index (Zero-based. Omit for E0 only.)
 *   O           - time-off         [ 1..15]
 *   V           - hysteresis_end   [-3..12]
 *   S           - hysteresis_start [ 1...8]
 * 
 * With no parameters report chopper times for all axis
 */

void GcodeSuite::M919() {
  #define TMC_SAY_CHOPPER_TIME(Q) tmc_print_chopper_time(stepper##Q)
  #define TMC_SET_CHOPPER_TIME(Q) stepper##Q.set_chopper_times(time_off, hysteresis_end, hysteresis_start)

  bool report = true;
  uint8_t time_off;
  uint8_t hysteresis_start;
  int8_t hysteresis_end;

  #if AXIS_IS_TMC(X) || AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4)
    const uint8_t index = parser.byteval('I');
  #endif
    
  static constexpr chopper_timing_t chopper_timing = CHOPPER_TIMING;
  time_off = chopper_timing.toff;
  hysteresis_end = chopper_timing.hend;
  hysteresis_start = chopper_timing.hstrt;

  LOOP_LOGICAL_AXES(i) if (parser.seen_test(axis_codes[i])) {
    report = false;

    if (parser.seenval('O')) {
      const uint8_t v = parser.value_byte();
      if (WITHIN(v, 1, 15)) {
        time_off = v;
        DEBUG_ECHOLNPGM("time_off: ", v);
      }
      else
        SERIAL_ECHOLNPGM("?O out of range (1..15)");
    }

    if (parser.seenval('V')) {
      const int8_t v = (int8_t)constrain(parser.value_long(), -127, 127);
      if (WITHIN(v, -3, 12)) {
        hysteresis_end = v;
        DEBUG_ECHOLNPGM("hysteresis_end: ", v);
      }
      else
        SERIAL_ECHOLNPGM("?V out of range (-3..12)");
    }

    if (parser.seenval('S')) {
      const uint8_t v = parser.value_byte();
      if (WITHIN(v, 1, 8)) {
        hysteresis_start = v;
        DEBUG_ECHOLNPGM("hysteresis_start: ", v);
      }
      else
        SERIAL_ECHOLNPGM("?S out of range (1..8)");
    }

    switch (i) {
      case X_AXIS:
        #if AXIS_IS_TMC(X)
          if (index == 0) TMC_SET_CHOPPER_TIME(X);
        #endif
        #if AXIS_IS_TMC(X2)
          if (index == 1) TMC_SET_CHOPPER_TIME(X2);
        #endif
        break;

      #if HAS_Y_AXIS
        case Y_AXIS:
          #if AXIS_IS_TMC(Y)
            if (index == 0) TMC_SET_CHOPPER_TIME(Y);
          #endif
          #if AXIS_IS_TMC(Y2)
            if (index == 1) TMC_SET_CHOPPER_TIME(Y2);
          #endif
          break;
      #endif

      #if HAS_Z_AXIS
        case Z_AXIS:
          #if AXIS_IS_TMC(Z)
            if (index == 0) TMC_SET_CHOPPER_TIME(Z);
          #endif
          #if AXIS_IS_TMC(Z2)
            if (index == 1) TMC_SET_CHOPPER_TIME(Z2);
          #endif
          #if AXIS_IS_TMC(Z3)
            if (index == 2) TMC_SET_CHOPPER_TIME(Z3);
          #endif
          #if AXIS_IS_TMC(Z4)
            if (index == 3) TMC_SET_CHOPPER_TIME(Z4);
          #endif
          break;
      #endif

      #if LINEAR_AXES >= 4
        case I_AXIS:
          #if AXIS_IS_TMC(I)
            TMC_SET_CHOPPER_TIME(I);
          #endif
          break;
      #endif

      #if LINEAR_AXES >= 5
        case J_AXIS:
          #if AXIS_IS_TMC(J)
            TMC_SET_CHOPPER_TIME(J);
          #endif
          break;
      #endif

      #if LINEAR_AXES >= 6
        case K_AXIS:
          #if AXIS_IS_TMC(K)
            TMC_SET_CHOPPER_TIME(K);
          #endif
          break;
      #endif

      #if E_STEPPERS
        case E_AXIS: {
          const int8_t target_e_stepper = get_target_e_stepper_from_command(0);
          if (target_e_stepper < 0) return;
          switch (target_e_stepper) {
            #if AXIS_IS_TMC(E0)
              case 0: TMC_SET_CHOPPER_TIME(E0); break;
            #endif
            #if AXIS_IS_TMC(E1)
              case 1: TMC_SET_CHOPPER_TIME(E1); break;
            #endif
            #if AXIS_IS_TMC(E2)
              case 2: TMC_SET_CHOPPER_TIME(E2); break;
            #endif
            #if AXIS_IS_TMC(E3)
              case 3: TMC_SET_CHOPPER_TIME(E3); break;
            #endif
            #if AXIS_IS_TMC(E4)
              case 4: TMC_SET_CHOPPER_TIME(E4); break;
            #endif
            #if AXIS_IS_TMC(E5)
              case 5: TMC_SET_CHOPPER_TIME(E5); break;
            #endif
            #if AXIS_IS_TMC(E6)
              case 6: TMC_SET_CHOPPER_TIME(E6); break;
            #endif
            #if AXIS_IS_TMC(E7)
              case 7: TMC_SET_CHOPPER_TIME(E7); break;
            #endif
          }
        } break;
      #endif
    }
  }

  if (report) {
    #if AXIS_IS_TMC(X)
      TMC_SAY_CHOPPER_TIME(X);
    #endif
    #if AXIS_IS_TMC(X2)
      TMC_SAY_CHOPPER_TIME(X2);
    #endif
    #if AXIS_IS_TMC(Y)
      TMC_SAY_CHOPPER_TIME(Y);
    #endif
    #if AXIS_IS_TMC(Y2)
      TMC_SAY_CHOPPER_TIME(Y2);
    #endif
    #if AXIS_IS_TMC(Z)
      TMC_SAY_CHOPPER_TIME(Z);
    #endif
    #if AXIS_IS_TMC(Z2)
      TMC_SAY_CHOPPER_TIME(Z2);
    #endif
    #if AXIS_IS_TMC(Z3)
      TMC_SAY_CHOPPER_TIME(Z3);
    #endif
    #if AXIS_IS_TMC(Z4)
      TMC_SAY_CHOPPER_TIME(Z4);
    #endif
    #if AXIS_IS_TMC(I)
      TMC_SAY_CHOPPER_TIME(I);
    #endif
    #if AXIS_IS_TMC(J)
      TMC_SAY_CHOPPER_TIME(J);
    #endif
    #if AXIS_IS_TMC(K)
      TMC_SAY_CHOPPER_TIME(K);
    #endif
    #if AXIS_IS_TMC(E0)
      TMC_SAY_CHOPPER_TIME(E0);
    #endif
    #if AXIS_IS_TMC(E1)
      TMC_SAY_CHOPPER_TIME(E1);
    #endif
    #if AXIS_IS_TMC(E2)
      TMC_SAY_CHOPPER_TIME(E2);
    #endif
    #if AXIS_IS_TMC(E3)
      TMC_SAY_CHOPPER_TIME(E3);
    #endif
    #if AXIS_IS_TMC(E4)
      TMC_SAY_CHOPPER_TIME(E4);
    #endif
    #if AXIS_IS_TMC(E5)
      TMC_SAY_CHOPPER_TIME(E5);
    #endif
    #if AXIS_IS_TMC(E6)
      TMC_SAY_CHOPPER_TIME(E6);
    #endif
    #if AXIS_IS_TMC(E7)
      TMC_SAY_CHOPPER_TIME(E7);
    #endif
  }
}

#endif // HAS_TRINAMIC_CONFIG
