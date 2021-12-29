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

#if AXIS_COLLISION('I')
  #error "M919 parameter collision with axis name."
#endif

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper/indirection.h"

#define DEBUG_OUT ENABLED(MARLIN_DEV_MODE)
#include "../../../core/debug_out.h"

template<typename TMC>
static void tmc_print_chopper_time(TMC &st) {
  st.printLabel();
  SERIAL_ECHOLNPGM(" chopper .toff: ", st.toff(),
                   " .hend: ", st.hysteresis_end(),
                   " .hstrt: ", st.hysteresis_start());
}

/**
 * M919: Set TMC stepper driver chopper times
 *
 * Parameters:
 *   XYZ...E     - Selected axes
 *   I[index]    - Axis sub-index (Omit for all XYZ steppers, 1 for X2, Y2, Z2; 2 for Z3; 3 for Z4)
 *   T[index]    - Extruder index (Zero-based. Omit for all extruders.)
 *   O           - time-off         [ 1..15]
 *   P           - hysteresis_end   [-3..12]
 *   S           - hysteresis_start [ 1...8]
 *
 * With no parameters report chopper times for all axes
 */
void GcodeSuite::M919() {
  #define TMC_SAY_CHOPPER_TIME(Q) tmc_print_chopper_time(stepper##Q)
  #define TMC_SET_CHOPPER_TIME(Q) stepper##Q.set_chopper_times(ct)

  bool report = true;
  chopper_timing_t ct = CHOPPER_TIMING;
    
  #if AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4)
    const int8_t index = parser.byteval('I', -1);
  #else
    constexpr int8_t index = -1;
  #endif

  LOOP_LOGICAL_AXES(i) if (parser.seen_test(axis_codes[i])) {
    report = false;

    switch (i) {
      case X_AXIS:
        #if AXIS_IS_TMC(X)
          if (index < 0 || index == 0) ct = CHOPPER_TIMING_X;
        #endif
        #if AXIS_IS_TMC(X2)
          if (index < 0 || index == 1) ct = CHOPPER_TIMING_X2;
        #endif
        break;

      #if HAS_Y_AXIS
        case Y_AXIS:
          #if AXIS_IS_TMC(Y)
            if (index < 0 || index == 0) ct = CHOPPER_TIMING_Y;
          #endif
          #if AXIS_IS_TMC(Y2)
            if (index < 0 || index == 1) ct = CHOPPER_TIMING_Y2;
          #endif
          break;
      #endif

      #if HAS_Z_AXIS
        case Z_AXIS:
          #if AXIS_IS_TMC(Z)
            if (index < 0 || index == 0) ct = CHOPPER_TIMING_Z;
          #endif
          #if AXIS_IS_TMC(Z2)
            if (index < 0 || index == 1) ct = CHOPPER_TIMING_Z2;
          #endif
          #if AXIS_IS_TMC(Z3)
            if (index < 0 || index == 2) ct = CHOPPER_TIMING_Z3;
          #endif
          #if AXIS_IS_TMC(Z4)
            if (index < 0 || index == 3) ct = CHOPPER_TIMING_Z4;
          #endif
          break;
      #endif

      //Configuration_adv.h does not cover yet chopper times for I, J, K axis

      #if E_STEPPERS
        case E_AXIS: {
          const int8_t eindex = get_target_e_stepper_from_command();
          #if AXIS_IS_TMC(E0)
            if (eindex < 0 || eindex == 0) ct = CHOPPER_TIMING_E;
          #endif
          #if AXIS_IS_TMC(E1)
            if (eindex < 0 || eindex == 1) ct = CHOPPER_TIMING_E1;
          #endif
          #if AXIS_IS_TMC(E2)
            if (eindex < 0 || eindex == 2) ct = CHOPPER_TIMING_E2;
          #endif
          #if AXIS_IS_TMC(E3)
            if (eindex < 0 || eindex == 3) ct = CHOPPER_TIMING_E3;
          #endif
          #if AXIS_IS_TMC(E4)
            if (eindex < 0 || eindex == 4) ct = CHOPPER_TIMING_E4;
          #endif
          #if AXIS_IS_TMC(E5)
            if (eindex < 0 || eindex == 5) ct = CHOPPER_TIMING_E5;
          #endif
          #if AXIS_IS_TMC(E6)
            if (eindex < 0 || eindex == 6) ct = CHOPPER_TIMING_E6;
          #endif
          #if AXIS_IS_TMC(E7)
            if (eindex < 0 || eindex == 7) ct = CHOPPER_TIMING_E7;
          #endif
        } break;
      #endif
      default:
        ct = CHOPPER_TIMING;
    }


    if (parser.seenval('O')) {
      const uint8_t v = parser.value_byte();
      if (WITHIN(v, 1, 15)) {
        ct.toff = v;
        DEBUG_ECHOLNPGM(".toff: ", v);
      }
      else
        SERIAL_ECHOLNPGM("?O out of range (1..15)");
    }

    if (parser.seenval('P')) {
      const int8_t v = (int8_t)constrain(parser.value_long(), -127, 127);
      if (WITHIN(v, -3, 12)) {
        ct.hend = v;
        DEBUG_ECHOLNPGM(".hend: ", v);
      }
      else
        SERIAL_ECHOLNPGM("?P out of range (-3..12)");
    }

    if (parser.seenval('S')) {
      const uint8_t v = parser.value_byte();
      if (WITHIN(v, 1, 8)) {
        ct.hstrt = v;
        DEBUG_ECHOLNPGM(".hstrt: ", v);
      }
      else
        SERIAL_ECHOLNPGM("?S out of range (1..8)");
    }

    switch (i) {
      case X_AXIS:
        #if AXIS_IS_TMC(X)
          if (index < 0 || index == 0) TMC_SET_CHOPPER_TIME(X);
        #endif
        #if AXIS_IS_TMC(X2)
          if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(X2);
        #endif
        break;

      #if HAS_Y_AXIS
        case Y_AXIS:
          #if AXIS_IS_TMC(Y)
            if (index < 0 || index == 0) TMC_SET_CHOPPER_TIME(Y);
          #endif
          #if AXIS_IS_TMC(Y2)
            if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(Y2);
          #endif
          break;
      #endif

      #if HAS_Z_AXIS
        case Z_AXIS:
          #if AXIS_IS_TMC(Z)
            if (index < 0 || index == 0) TMC_SET_CHOPPER_TIME(Z);
          #endif
          #if AXIS_IS_TMC(Z2)
            if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(Z2);
          #endif
          #if AXIS_IS_TMC(Z3)
            if (index < 0 || index == 2) TMC_SET_CHOPPER_TIME(Z3);
          #endif
          #if AXIS_IS_TMC(Z4)
            if (index < 0 || index == 3) TMC_SET_CHOPPER_TIME(Z4);
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
          const int8_t eindex = get_target_e_stepper_from_command();
          #if AXIS_IS_TMC(E0)
            if (eindex < 0 || eindex == 0) TMC_SET_CHOPPER_TIME(E0);
          #endif
          #if AXIS_IS_TMC(E1)
            if (eindex < 0 || eindex == 1) TMC_SET_CHOPPER_TIME(E1);
          #endif
          #if AXIS_IS_TMC(E2)
            if (eindex < 0 || eindex == 2) TMC_SET_CHOPPER_TIME(E2);
          #endif
          #if AXIS_IS_TMC(E3)
            if (eindex < 0 || eindex == 3) TMC_SET_CHOPPER_TIME(E3);
          #endif
          #if AXIS_IS_TMC(E4)
            if (eindex < 0 || eindex == 4) TMC_SET_CHOPPER_TIME(E4);
          #endif
          #if AXIS_IS_TMC(E5)
            if (eindex < 0 || eindex == 5) TMC_SET_CHOPPER_TIME(E5);
          #endif
          #if AXIS_IS_TMC(E6)
            if (eindex < 0 || eindex == 6) TMC_SET_CHOPPER_TIME(E6);
          #endif
          #if AXIS_IS_TMC(E7)
            if (eindex < 0 || eindex == 7) TMC_SET_CHOPPER_TIME(E7);
          #endif
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
