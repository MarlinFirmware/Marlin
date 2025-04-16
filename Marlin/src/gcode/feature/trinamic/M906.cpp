/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

template<typename TMC>
static void tmc_print_current(TMC &st) {
  st.printLabel();
  SERIAL_ECHOLNPGM(" driver current: ", st.getMilliamps());
}

/**
 * M906: Set motor current in milliamps.
 *
 * With no parameters report driver currents.
 *
 * Parameters:
 *   X[current]  - Set mA current for X driver(s)
 *   Y[current]  - Set mA current for Y driver(s)
 *   Z[current]  - Set mA current for Z driver(s)
 *   A[current]  - Set mA current for A driver(s) (Requires AXIS*_NAME 'A')
 *   B[current]  - Set mA current for B driver(s) (Requires AXIS*_NAME 'B')
 *   C[current]  - Set mA current for C driver(s) (Requires AXIS*_NAME 'C')
 *   U[current]  - Set mA current for U driver(s) (Requires AXIS*_NAME 'U')
 *   V[current]  - Set mA current for V driver(s) (Requires AXIS*_NAME 'V')
 *   W[current]  - Set mA current for W driver(s) (Requires AXIS*_NAME 'W')
 *   E[current]  - Set mA current for E driver(s)
 *
 *   I[index]    - Axis sub-index (Omit or 0 for X, Y, Z; 1 for X2, Y2, Z2; 2 for Z3; 3 for Z4.)
 *   T[index]    - Extruder index (Zero-based. Omit for E0 only.)
 *
 * With EDITABLE_HOMING_CURRENT:
 *   H           - Set / Report Homing Current. Alias for M920.
 */
void GcodeSuite::M906() {
  #if ENABLED(EDITABLE_HOMING_CURRENT)
    if (parser.seen_test('H')) return M920();
  #endif

  #define TMC_SAY_CURRENT(Q) tmc_print_current(stepper##Q)
  #define TMC_SET_CURRENT(Q) stepper##Q.rms_current(value)

  bool report = true;

  #if ANY(X2_IS_TRINAMIC, Y2_IS_TRINAMIC, Z2_IS_TRINAMIC, Z3_IS_TRINAMIC, Z4_IS_TRINAMIC)
    const int8_t index = parser.byteval('I', -1);
  #elif ANY(X_IS_TRINAMIC, Y_IS_TRINAMIC, Z_IS_TRINAMIC)
    constexpr int8_t index = -1;
  #endif

  LOOP_LOGICAL_AXES(i) if (uint16_t value = parser.intval(AXIS_CHAR(i))) {
    report = false;
    switch (i) {
      #if X_IS_TRINAMIC || X2_IS_TRINAMIC
        case X_AXIS:
          TERN_(X_IS_TRINAMIC,  if (index < 0 || index == 0) TMC_SET_CURRENT(X));
          TERN_(X2_IS_TRINAMIC, if (index < 0 || index == 1) TMC_SET_CURRENT(X2));
          break;
      #endif

      #if Y_IS_TRINAMIC || Y2_IS_TRINAMIC
        case Y_AXIS:
          TERN_(Y_IS_TRINAMIC,  if (index < 0 || index == 0) TMC_SET_CURRENT(Y));
          TERN_(Y2_IS_TRINAMIC, if (index < 0 || index == 1) TMC_SET_CURRENT(Y2));
          break;
      #endif

      #if ANY(Z_IS_TRINAMIC, Z2_IS_TRINAMIC, Z3_IS_TRINAMIC, Z4_IS_TRINAMIC)
        case Z_AXIS:
          TERN_(Z_IS_TRINAMIC,  if (index < 0 || index == 0) TMC_SET_CURRENT(Z));
          TERN_(Z2_IS_TRINAMIC, if (index < 0 || index == 1) TMC_SET_CURRENT(Z2));
          TERN_(Z3_IS_TRINAMIC, if (index < 0 || index == 2) TMC_SET_CURRENT(Z3));
          TERN_(Z4_IS_TRINAMIC, if (index < 0 || index == 3) TMC_SET_CURRENT(Z4));
          break;
      #endif

      #if I_IS_TRINAMIC
        case I_AXIS: TMC_SET_CURRENT(I); break;
      #endif
      #if J_IS_TRINAMIC
        case J_AXIS: TMC_SET_CURRENT(J); break;
      #endif
      #if K_IS_TRINAMIC
        case K_AXIS: TMC_SET_CURRENT(K); break;
      #endif
      #if U_IS_TRINAMIC
        case U_AXIS: TMC_SET_CURRENT(U); break;
      #endif
      #if V_IS_TRINAMIC
        case V_AXIS: TMC_SET_CURRENT(V); break;
      #endif
      #if W_IS_TRINAMIC
        case W_AXIS: TMC_SET_CURRENT(W); break;
      #endif

      #if ANY(E0_IS_TRINAMIC, E1_IS_TRINAMIC, E2_IS_TRINAMIC, E3_IS_TRINAMIC, E4_IS_TRINAMIC, E5_IS_TRINAMIC, E6_IS_TRINAMIC, E7_IS_TRINAMIC)
        case E_AXIS: {
          const int8_t eindex = get_target_e_stepper_from_command(-2);
          TERN_(E0_IS_TRINAMIC, if (eindex < 0 || eindex == 0) TMC_SET_CURRENT(E0));
          TERN_(E1_IS_TRINAMIC, if (eindex < 0 || eindex == 1) TMC_SET_CURRENT(E1));
          TERN_(E2_IS_TRINAMIC, if (eindex < 0 || eindex == 2) TMC_SET_CURRENT(E2));
          TERN_(E3_IS_TRINAMIC, if (eindex < 0 || eindex == 3) TMC_SET_CURRENT(E3));
          TERN_(E4_IS_TRINAMIC, if (eindex < 0 || eindex == 4) TMC_SET_CURRENT(E4));
          TERN_(E5_IS_TRINAMIC, if (eindex < 0 || eindex == 5) TMC_SET_CURRENT(E5));
          TERN_(E6_IS_TRINAMIC, if (eindex < 0 || eindex == 6) TMC_SET_CURRENT(E6));
          TERN_(E7_IS_TRINAMIC, if (eindex < 0 || eindex == 7) TMC_SET_CURRENT(E7));
        } break;
      #endif
    }
  }

  if (report) {
    TERN_(X_IS_TRINAMIC,  TMC_SAY_CURRENT(X));
    TERN_(X2_IS_TRINAMIC, TMC_SAY_CURRENT(X2));
    TERN_(Y_IS_TRINAMIC,  TMC_SAY_CURRENT(Y));
    TERN_(Y2_IS_TRINAMIC, TMC_SAY_CURRENT(Y2));
    TERN_(Z_IS_TRINAMIC,  TMC_SAY_CURRENT(Z));
    TERN_(Z2_IS_TRINAMIC, TMC_SAY_CURRENT(Z2));
    TERN_(Z3_IS_TRINAMIC, TMC_SAY_CURRENT(Z3));
    TERN_(Z4_IS_TRINAMIC, TMC_SAY_CURRENT(Z4));
    TERN_(I_IS_TRINAMIC,  TMC_SAY_CURRENT(I));
    TERN_(J_IS_TRINAMIC,  TMC_SAY_CURRENT(J));
    TERN_(K_IS_TRINAMIC,  TMC_SAY_CURRENT(K));
    TERN_(U_IS_TRINAMIC,  TMC_SAY_CURRENT(U));
    TERN_(V_IS_TRINAMIC,  TMC_SAY_CURRENT(V));
    TERN_(W_IS_TRINAMIC,  TMC_SAY_CURRENT(W));
    TERN_(E0_IS_TRINAMIC, TMC_SAY_CURRENT(E0));
    TERN_(E1_IS_TRINAMIC, TMC_SAY_CURRENT(E1));
    TERN_(E2_IS_TRINAMIC, TMC_SAY_CURRENT(E2));
    TERN_(E3_IS_TRINAMIC, TMC_SAY_CURRENT(E3));
    TERN_(E4_IS_TRINAMIC, TMC_SAY_CURRENT(E4));
    TERN_(E5_IS_TRINAMIC, TMC_SAY_CURRENT(E5));
    TERN_(E6_IS_TRINAMIC, TMC_SAY_CURRENT(E6));
    TERN_(E7_IS_TRINAMIC, TMC_SAY_CURRENT(E7));
  }
}

void GcodeSuite::M906_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading(forReplay, F(STR_STEPPER_DRIVER_CURRENT));

  auto say_M906 = [](const bool forReplay) {
    report_echo_start(forReplay);
    SERIAL_ECHOPGM("  M906");
  };

  #if ANY(X_IS_TRINAMIC, Y_IS_TRINAMIC, Z_IS_TRINAMIC, I_IS_TRINAMIC, J_IS_TRINAMIC, K_IS_TRINAMIC, U_IS_TRINAMIC, V_IS_TRINAMIC, W_IS_TRINAMIC)
    say_M906(forReplay);
    TERN_(X_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_X_STR, stepperX.getMilliamps()));
    TERN_(Y_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_Y_STR, stepperY.getMilliamps()));
    TERN_(Z_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_Z_STR, stepperZ.getMilliamps()));
    TERN_(I_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_I_STR, stepperI.getMilliamps()));
    TERN_(J_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_J_STR, stepperJ.getMilliamps()));
    TERN_(K_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_K_STR, stepperK.getMilliamps()));
    TERN_(U_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_U_STR, stepperU.getMilliamps()));
    TERN_(V_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_V_STR, stepperV.getMilliamps()));
    TERN_(W_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_W_STR, stepperW.getMilliamps()));
    SERIAL_EOL();
  #endif

  #if X2_IS_TRINAMIC || Y2_IS_TRINAMIC || Z2_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOPGM(" I1");
    TERN_(X2_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_X_STR, stepperX2.getMilliamps()));
    TERN_(Y2_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_Y_STR, stepperY2.getMilliamps()));
    TERN_(Z2_IS_TRINAMIC, SERIAL_ECHOPGM_P(SP_Z_STR, stepperZ2.getMilliamps()));
    SERIAL_EOL();
  #endif

  #if Z3_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" I2 Z", stepperZ3.getMilliamps());
  #endif
  #if Z4_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" I3 Z", stepperZ4.getMilliamps());
  #endif

  #if E0_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T0 E", stepperE0.getMilliamps());
  #endif
  #if E1_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T1 E", stepperE1.getMilliamps());
  #endif
  #if E2_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T2 E", stepperE2.getMilliamps());
  #endif
  #if E3_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T3 E", stepperE3.getMilliamps());
  #endif
  #if E4_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T4 E", stepperE4.getMilliamps());
  #endif
  #if E5_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T5 E", stepperE5.getMilliamps());
  #endif
  #if E6_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T6 E", stepperE6.getMilliamps());
  #endif
  #if E7_IS_TRINAMIC
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T7 E", stepperE7.getMilliamps());
  #endif
}

#endif // HAS_TRINAMIC_CONFIG
