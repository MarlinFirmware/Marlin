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

/**
 * M906: Set motor current in milliamps.
 *
 * Parameters:
 *   X[current]  - Set mA current for X driver(s)
 *   Y[current]  - Set mA current for Y driver(s)
 *   Z[current]  - Set mA current for Z driver(s)
 *   E[current]  - Set mA current for E driver(s)
 *
 *   I[index]    - Axis sub-index (Omit or 0 for X, Y, Z; 1 for X2, Y2, Z2; 2 for Z3; 3 for Z4.)
 *   T[index]    - Extruder index (Zero-based. Omit for E0 only.)
 *
 * With no parameters report driver currents.
 */
void GcodeSuite::M906() {
  #define TMC_SAY_CURRENT(Q) tmc_print_current(stepper##Q)
  #define TMC_SET_CURRENT(Q) stepper##Q.rms_current(value)

  bool report = true;

  #if AXIS_IS_TMC(X) || AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4) || AXIS_IS_TMC(I) || AXIS_IS_TMC(J) || AXIS_IS_TMC(K)
    const uint8_t index = parser.byteval('I');
  #endif

  LOOP_LOGICAL_AXES(i) if (uint16_t value = parser.intval(axis_codes[i])) {
    report = false;
    switch (i) {
      case X_AXIS:
        #if AXIS_IS_TMC(X)
          if (index == 0) TMC_SET_CURRENT(X);
        #endif
        #if AXIS_IS_TMC(X2)
          if (index == 1) TMC_SET_CURRENT(X2);
        #endif
        break;

      #if HAS_Y_AXIS
        case Y_AXIS:
          #if AXIS_IS_TMC(Y)
            if (index == 0) TMC_SET_CURRENT(Y);
          #endif
          #if AXIS_IS_TMC(Y2)
            if (index == 1) TMC_SET_CURRENT(Y2);
          #endif
          break;
      #endif

      #if HAS_Z_AXIS
        case Z_AXIS:
          #if AXIS_IS_TMC(Z)
            if (index == 0) TMC_SET_CURRENT(Z);
          #endif
          #if AXIS_IS_TMC(Z2)
            if (index == 1) TMC_SET_CURRENT(Z2);
          #endif
          #if AXIS_IS_TMC(Z3)
            if (index == 2) TMC_SET_CURRENT(Z3);
          #endif
          #if AXIS_IS_TMC(Z4)
            if (index == 3) TMC_SET_CURRENT(Z4);
          #endif
          break;
      #endif

      #if AXIS_IS_TMC(I)
        case I_AXIS: TMC_SET_CURRENT(I); break;
      #endif
      #if AXIS_IS_TMC(J)
        case J_AXIS: TMC_SET_CURRENT(J); break;
      #endif
      #if AXIS_IS_TMC(K)
        case K_AXIS: TMC_SET_CURRENT(K); break;
      #endif

      #if E_STEPPERS
        case E_AXIS: {
          const int8_t target_e_stepper = get_target_e_stepper_from_command();
          if (target_e_stepper < 0) return;
          switch (target_e_stepper) {
            #if AXIS_IS_TMC(E0)
              case 0: TMC_SET_CURRENT(E0); break;
            #endif
            #if AXIS_IS_TMC(E1)
              case 1: TMC_SET_CURRENT(E1); break;
            #endif
            #if AXIS_IS_TMC(E2)
              case 2: TMC_SET_CURRENT(E2); break;
            #endif
            #if AXIS_IS_TMC(E3)
              case 3: TMC_SET_CURRENT(E3); break;
            #endif
            #if AXIS_IS_TMC(E4)
              case 4: TMC_SET_CURRENT(E4); break;
            #endif
            #if AXIS_IS_TMC(E5)
              case 5: TMC_SET_CURRENT(E5); break;
            #endif
            #if AXIS_IS_TMC(E6)
              case 6: TMC_SET_CURRENT(E6); break;
            #endif
            #if AXIS_IS_TMC(E7)
              case 7: TMC_SET_CURRENT(E7); break;
            #endif
          }
        } break;
      #endif
    }
  }

  if (report) {
    #if AXIS_IS_TMC(X)
      TMC_SAY_CURRENT(X);
    #endif
    #if AXIS_IS_TMC(X2)
      TMC_SAY_CURRENT(X2);
    #endif
    #if AXIS_IS_TMC(Y)
      TMC_SAY_CURRENT(Y);
    #endif
    #if AXIS_IS_TMC(Y2)
      TMC_SAY_CURRENT(Y2);
    #endif
    #if AXIS_IS_TMC(Z)
      TMC_SAY_CURRENT(Z);
    #endif
    #if AXIS_IS_TMC(Z2)
      TMC_SAY_CURRENT(Z2);
    #endif
    #if AXIS_IS_TMC(Z3)
      TMC_SAY_CURRENT(Z3);
    #endif
    #if AXIS_IS_TMC(Z4)
      TMC_SAY_CURRENT(Z4);
    #endif
    #if AXIS_IS_TMC(I)
      TMC_SAY_CURRENT(I);
    #endif
    #if AXIS_IS_TMC(J)
      TMC_SAY_CURRENT(J);
    #endif
    #if AXIS_IS_TMC(K)
      TMC_SAY_CURRENT(K);
    #endif
    #if AXIS_IS_TMC(E0)
      TMC_SAY_CURRENT(E0);
    #endif
    #if AXIS_IS_TMC(E1)
      TMC_SAY_CURRENT(E1);
    #endif
    #if AXIS_IS_TMC(E2)
      TMC_SAY_CURRENT(E2);
    #endif
    #if AXIS_IS_TMC(E3)
      TMC_SAY_CURRENT(E3);
    #endif
    #if AXIS_IS_TMC(E4)
      TMC_SAY_CURRENT(E4);
    #endif
    #if AXIS_IS_TMC(E5)
      TMC_SAY_CURRENT(E5);
    #endif
    #if AXIS_IS_TMC(E6)
      TMC_SAY_CURRENT(E6);
    #endif
    #if AXIS_IS_TMC(E7)
      TMC_SAY_CURRENT(E7);
    #endif
  }
}

void GcodeSuite::M906_report(const bool forReplay/*=true*/) {
  report_heading(forReplay, F(STR_STEPPER_DRIVER_CURRENT));

  auto say_M906 = [](const bool forReplay) {
    report_echo_start(forReplay);
    SERIAL_ECHOPGM("  M906");
  };

  #if  AXIS_IS_TMC(X) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Z) \
    || AXIS_IS_TMC(I) || AXIS_IS_TMC(J) || AXIS_IS_TMC(K)
    say_M906(forReplay);
    #if AXIS_IS_TMC(X)
      SERIAL_ECHOPGM_P(SP_X_STR, stepperX.getMilliamps());
    #endif
    #if AXIS_IS_TMC(Y)
      SERIAL_ECHOPGM_P(SP_Y_STR, stepperY.getMilliamps());
    #endif
    #if AXIS_IS_TMC(Z)
      SERIAL_ECHOPGM_P(SP_Z_STR, stepperZ.getMilliamps());
    #endif
    #if AXIS_IS_TMC(I)
      SERIAL_ECHOPGM_P(SP_I_STR, stepperI.getMilliamps());
    #endif
    #if AXIS_IS_TMC(J)
      SERIAL_ECHOPGM_P(SP_J_STR, stepperJ.getMilliamps());
    #endif
    #if AXIS_IS_TMC(K)
      SERIAL_ECHOPGM_P(SP_K_STR, stepperK.getMilliamps());
    #endif
    SERIAL_EOL();
  #endif

  #if AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z2)
    say_M906(forReplay);
    SERIAL_ECHOPGM(" I1");
    #if AXIS_IS_TMC(X2)
      SERIAL_ECHOPGM_P(SP_X_STR, stepperX2.getMilliamps());
    #endif
    #if AXIS_IS_TMC(Y2)
      SERIAL_ECHOPGM_P(SP_Y_STR, stepperY2.getMilliamps());
    #endif
    #if AXIS_IS_TMC(Z2)
      SERIAL_ECHOPGM_P(SP_Z_STR, stepperZ2.getMilliamps());
    #endif
    SERIAL_EOL();
  #endif

  #if AXIS_IS_TMC(Z3)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" I2 Z", stepperZ3.getMilliamps());
  #endif
  #if AXIS_IS_TMC(Z4)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" I3 Z", stepperZ4.getMilliamps());
  #endif

  #if AXIS_IS_TMC(E0)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T0 E", stepperE0.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E1)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T1 E", stepperE1.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E2)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T2 E", stepperE2.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E3)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T3 E", stepperE3.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E4)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T4 E", stepperE4.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E5)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T5 E", stepperE5.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E6)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T6 E", stepperE6.getMilliamps());
  #endif
  #if AXIS_IS_TMC(E7)
    say_M906(forReplay);
    SERIAL_ECHOLNPGM(" T7 E", stepperE7.getMilliamps());
  #endif
  SERIAL_EOL();
}

#endif // HAS_TRINAMIC_CONFIG
