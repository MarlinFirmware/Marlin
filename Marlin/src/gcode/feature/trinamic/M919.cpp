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
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("O out of range (1..15)"));
      err = true;
    }
  }

  int8_t hend = int8_t(parser.intval('P', -127));
  if (hend != -127) {
    if (WITHIN(hend, -3, 12))
      DEBUG_ECHOLNPGM(".hend: ", hend);
    else {
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("P out of range (-3..12)"));
      err = true;
    }
  }

  int8_t hstrt = int8_t(parser.intval('S', -127));
  if (hstrt != -127) {
    if (WITHIN(hstrt, 1, 8))
      DEBUG_ECHOLNPGM(".hstrt: ", hstrt);
    else {
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("S out of range (1..8)"));
      err = true;
    }
  }

  if (err) return;

  #if ANY(X2_IS_TRINAMIC, Y2_IS_TRINAMIC, Z2_IS_TRINAMIC, Z3_IS_TRINAMIC, Z4_IS_TRINAMIC)
    const int8_t index = parser.byteval('I');
  #elif ANY(X_IS_TRINAMIC, Y_IS_TRINAMIC, Z_IS_TRINAMIC)
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

  #if ANY(E0_IS_TRINAMIC, E1_IS_TRINAMIC, E2_IS_TRINAMIC, E3_IS_TRINAMIC, E4_IS_TRINAMIC, E5_IS_TRINAMIC, E6_IS_TRINAMIC, E7_IS_TRINAMIC)
    #define HAS_E_CHOPPER 1
    int8_t eindex = -1;
  #endif
  bool report = true;
  LOOP_LOGICAL_AXES(i) if (parser.seen_test(AXIS_CHAR(i))) {
    report = false;

    // Get the chopper timing for the specified axis and index
    switch (i) {
      default: // A specified axis isn't Trinamic
        SERIAL_ECHOLNPGM(GCODE_ERR_MSG("Axis ", C(AXIS_CHAR(i)), " has no TMC drivers."));
        break;

      #if X_IS_TRINAMIC || X2_IS_TRINAMIC
        case X_AXIS:
          TERN_(X_IS_TRINAMIC, if (index <= 0) TMC_SET_CHOPPER_TIME(X));
          TERN_(X2_IS_TRINAMIC, if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(X2));
          break;
      #endif

      #if Y_IS_TRINAMIC || Y2_IS_TRINAMIC
        case Y_AXIS:
          TERN_(Y_IS_TRINAMIC, if (index <= 0) TMC_SET_CHOPPER_TIME(Y));
          TERN_(Y2_IS_TRINAMIC, if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(Y2));
          break;
      #endif

      #if ANY(Z_IS_TRINAMIC, Z2_IS_TRINAMIC, Z3_IS_TRINAMIC, Z4_IS_TRINAMIC)
        case Z_AXIS:
          TERN_(Z_IS_TRINAMIC, if (index <= 0) TMC_SET_CHOPPER_TIME(Z));
          TERN_(Z2_IS_TRINAMIC, if (index < 0 || index == 1) TMC_SET_CHOPPER_TIME(Z2));
          TERN_(Z3_IS_TRINAMIC, if (index < 0 || index == 2) TMC_SET_CHOPPER_TIME(Z3));
          TERN_(Z4_IS_TRINAMIC, if (index < 0 || index == 3) TMC_SET_CHOPPER_TIME(Z4));
          break;
      #endif

      #if I_IS_TRINAMIC
        case I_AXIS: TMC_SET_CHOPPER_TIME(I); break;
      #endif
      #if J_IS_TRINAMIC
        case J_AXIS: TMC_SET_CHOPPER_TIME(J); break;
      #endif
      #if K_IS_TRINAMIC
        case K_AXIS: TMC_SET_CHOPPER_TIME(K); break;
      #endif
      #if U_IS_TRINAMIC
        case U_AXIS: TMC_SET_CHOPPER_TIME(U); break;
      #endif
      #if V_IS_TRINAMIC
        case V_AXIS: TMC_SET_CHOPPER_TIME(V); break;
      #endif
      #if W_IS_TRINAMIC
        case W_AXIS: TMC_SET_CHOPPER_TIME(W); break;
      #endif

      #if HAS_E_CHOPPER
        case E_AXIS: {
          TERN_(E0_IS_TRINAMIC, if (eindex <= 0) TMC_SET_CHOPPER_TIME(E0));
          TERN_(E1_IS_TRINAMIC, if (eindex < 0 || eindex == 1) TMC_SET_CHOPPER_TIME(E1));
          TERN_(E2_IS_TRINAMIC, if (eindex < 0 || eindex == 2) TMC_SET_CHOPPER_TIME(E2));
          TERN_(E3_IS_TRINAMIC, if (eindex < 0 || eindex == 3) TMC_SET_CHOPPER_TIME(E3));
          TERN_(E4_IS_TRINAMIC, if (eindex < 0 || eindex == 4) TMC_SET_CHOPPER_TIME(E4));
          TERN_(E5_IS_TRINAMIC, if (eindex < 0 || eindex == 5) TMC_SET_CHOPPER_TIME(E5));
          TERN_(E6_IS_TRINAMIC, if (eindex < 0 || eindex == 6) TMC_SET_CHOPPER_TIME(E6));
          TERN_(E7_IS_TRINAMIC, if (eindex < 0 || eindex == 7) TMC_SET_CHOPPER_TIME(E7));
        } break;
      #endif
    }
  }

  if (report) {
    #define TMC_SAY_CHOPPER_TIME(Q) OPTCODE(Q##_IS_TRINAMIC, tmc_print_chopper_time(stepper##Q))
    MAP(TMC_SAY_CHOPPER_TIME, ALL_AXIS_NAMES)
  }
}

#endif // HAS_TRINAMIC_CONFIG
