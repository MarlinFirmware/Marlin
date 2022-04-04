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
  #error "M919 parameter 'I' collision with axis name."
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
  bool err = false;

  int8_t toff = int8_t(parser.intval('O', -127));
  if (toff != -127) {
    if (WITHIN(toff, 1, 15))
      DEBUG_ECHOLNPGM(".toff: ", toff);
    else {
      SERIAL_ECHOLNPGM("?O out of range (1..15)");
      err = true;
    }
  }

  int8_t hend = int8_t(parser.intval('P', -127));
  if (hend != -127) {
    if (WITHIN(hend, -3, 12))
      DEBUG_ECHOLNPGM(".hend: ", hend);
    else {
      SERIAL_ECHOLNPGM("?P out of range (-3..12)");
      err = true;
    }
  }

  int8_t hstrt = int8_t(parser.intval('S', -127));
  if (hstrt != -127) {
    if (WITHIN(hstrt, 1, 8))
      DEBUG_ECHOLNPGM(".hstrt: ", hstrt);
    else {
      SERIAL_ECHOLNPGM("?S out of range (1..8)");
      err = true;
    }
  }

  if (err) return;

  #if AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4)
    const int8_t index = parser.byteval('I');
  #elif AXIS_IS_TMC(X) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Z)
    constexpr int8_t index = -1;
  #endif

  auto make_chopper_timing = [](chopper_timing_t bct, const int8_t toff, const int8_t hend, const int8_t hstrt) {
    chopper_timing_t uct = bct;
    if (toff  != -127) uct.toff  = toff;
    if (hend  != -127) uct.hend  = hend;
    if (hstrt != -127) uct.hstrt = hstrt;
    return uct;
  };

  #define TMC_SET_CHOPPER_TIME(Q) stepper##Q.set_chopper_times(make_chopper_timing(CHOPPER_TIMING_##Q, toff, hend, hstrt))

  #if AXIS_IS_TMC(E0) || AXIS_IS_TMC(E1) || AXIS_IS_TMC(E2) || AXIS_IS_TMC(E3) || AXIS_IS_TMC(E4) || AXIS_IS_TMC(E5) || AXIS_IS_TMC(E6) || AXIS_IS_TMC(E7)
    #define HAS_E_CHOPPER 1
    int8_t eindex = -1;
  #endif
  bool report = true;
  LOOP_LOGICAL_AXES(i) if (parser.seen_test(AXIS_CHAR(i))) {
    report = false;

    // Get the chopper timing for the specified axis and index
    switch (i) {
      default: // A specified axis isn't Trinamic
        SERIAL_ECHOLNPGM("?Axis ", AS_CHAR(AXIS_CHAR(i)), " has no TMC drivers.");
        break;

      #if AXIS_IS_TMC(X) || AXIS_IS_TMC(X2)
        case X_AXIS:
          #if AXIS_IS_TMC(X)
            if (index <= 0) TMC_SET_CHOPPER_TIME(X);
          #endif
          #if AXIS_IS_TMC(X2)
            if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(X2);
          #endif
          break;
      #endif

      #if AXIS_IS_TMC(Y) || AXIS_IS_TMC(Y2)
        case Y_AXIS:
          #if AXIS_IS_TMC(Y)
            if (index <= 0) TMC_SET_CHOPPER_TIME(Y);
          #endif
          #if AXIS_IS_TMC(Y2)
            if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(Y2);
          #endif
          break;
      #endif

      #if AXIS_IS_TMC(Z) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4)
        case Z_AXIS:
          #if AXIS_IS_TMC(Z)
            if (index <= 0) TMC_SET_CHOPPER_TIME(Z);
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

      #if AXIS_IS_TMC(I)
        case I_AXIS: TMC_SET_CHOPPER_TIME(I); break;
      #endif
      #if AXIS_IS_TMC(J)
        case J_AXIS: TMC_SET_CHOPPER_TIME(J); break;
      #endif
      #if AXIS_IS_TMC(K)
        case K_AXIS: TMC_SET_CHOPPER_TIME(K); break;
      #endif

      #if HAS_E_CHOPPER
        case E_AXIS: {
          #if AXIS_IS_TMC(E0)
            if (eindex <= 0) TMC_SET_CHOPPER_TIME(E0);
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
    #define TMC_SAY_CHOPPER_TIME(Q) tmc_print_chopper_time(stepper##Q)
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
