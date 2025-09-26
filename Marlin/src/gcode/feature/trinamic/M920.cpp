/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(EDITABLE_HOMING_CURRENT)

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"

#if AXIS_COLLISION('I')
  #define I_PARAM 'S'
  #define I_PARAM_STR "S"
  #warning "Use 'M920 S' instead of 'M920 I' for the stepper number."
#else
  #define I_PARAM 'I'
  #define I_PARAM_STR "I"
#endif

/**
 * M920: Set Homing Current for one or more axes
 *
 * Parameters:
 *   X<current>  - Homing Current to use for X axis stepper(s)
 *   Y<current>  - Homing Current to use for Y axis stepper(s)
 *   Z<current>  - Homing Current to use for Z axis stepper(s)
 *   A<current>  - Homing Current to use for A axis stepper(s)
 *   B<current>  - Homing Current to use for B axis stepper(s)
 *   C<current>  - Homing Current to use for C axis stepper(s)
 *   U<current>  - Homing Current to use for U axis stepper(s)
 *   V<current>  - Homing Current to use for V axis stepper(s)
 *   W<current>  - Homing Current to use for W axis stepper(s)
 *
 *   I<index>    - For multi-stepper axes, the zero-based index of the stepper to modify in each axis.
 *                 If omitted all steppers of each axis will be set to the given axis current.
 */
void GcodeSuite::M920() {
  bool report = true;
  const int8_t index = parser.intval(I_PARAM, -1);
  LOOP_NUM_AXES(i) if (parser.seen(AXIS_CHAR(i))) {
    const int16_t value = parser.value_int();
    report = false;
    switch (i) {
      #if X_HAS_HOME_CURRENT
        case X_AXIS:
          if (index <= 0) homing_current_mA.X = value;
          TERN_(X2_HAS_HOME_CURRENT, if (index < 0 || index == 1) homing_current_mA.X2 = value);
          break;
      #endif
      #if Y_HAS_HOME_CURRENT
        case Y_AXIS:
          if (index <= 0) homing_current_mA.Y = value;
          TERN_(Y2_HAS_HOME_CURRENT, if (index < 0 || index == 1) homing_current_mA.Y2 = value);
          break;
      #endif
      #if Z_HAS_HOME_CURRENT
        case Z_AXIS:
          if (index <= 0) homing_current_mA.Z = value;
          TERN_(Z2_HAS_HOME_CURRENT, if (index < 0 || index == 1) homing_current_mA.Z2 = value);
          TERN_(Z3_HAS_HOME_CURRENT, if (index < 0 || index == 2) homing_current_mA.Z3 = value);
          TERN_(Z4_HAS_HOME_CURRENT, if (index < 0 || index == 3) homing_current_mA.Z4 = value);
          break;
      #endif
      OPTCODE(I_HAS_HOME_CURRENT, case I_AXIS: homing_current_mA.I = value; break)
      OPTCODE(J_HAS_HOME_CURRENT, case J_AXIS: homing_current_mA.J = value; break)
      OPTCODE(K_HAS_HOME_CURRENT, case K_AXIS: homing_current_mA.K = value; break)
      OPTCODE(U_HAS_HOME_CURRENT, case U_AXIS: homing_current_mA.U = value; break)
      OPTCODE(V_HAS_HOME_CURRENT, case V_AXIS: homing_current_mA.V = value; break)
      OPTCODE(W_HAS_HOME_CURRENT, case W_AXIS: homing_current_mA.W = value; break)
    }
  }

  if (report) M920_report();
}

void GcodeSuite::M920_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  #if X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS

    report_heading(forReplay, F(STR_HOMING_CURRENT));

    auto say_M920 = [](const bool forReplay, int8_t index=-1) {
      report_echo_start(forReplay);
      SERIAL_ECHOPGM("  M920");
      if (index >= 0) SERIAL_ECHOPGM(" " I_PARAM_STR, index);
    };

    #if X2_SENSORLESS || Y2_SENSORLESS || Z2_SENSORLESS || Z3_SENSORLESS || Z4_SENSORLESS
      say_M920(forReplay, 0);
    #else
      say_M920(forReplay);
    #endif
    TERN_(X_SENSORLESS, SERIAL_ECHOPGM_P(SP_X_STR, homing_current_mA.X));
    TERN_(Y_SENSORLESS, SERIAL_ECHOPGM_P(SP_Y_STR, homing_current_mA.Y));
    TERN_(Z_SENSORLESS, SERIAL_ECHOPGM_P(SP_Z_STR, homing_current_mA.Z));
    #if X2_SENSORLESS || Y2_SENSORLESS || Z2_SENSORLESS || Z3_SENSORLESS || Z4_SENSORLESS
      SERIAL_EOL();
      say_M920(forReplay);
    #endif
    TERN_(I_SENSORLESS, SERIAL_ECHOPGM_P(SP_I_STR, homing_current_mA.I));
    TERN_(J_SENSORLESS, SERIAL_ECHOPGM_P(SP_J_STR, homing_current_mA.J));
    TERN_(K_SENSORLESS, SERIAL_ECHOPGM_P(SP_K_STR, homing_current_mA.K));
    TERN_(U_SENSORLESS, SERIAL_ECHOPGM_P(SP_U_STR, homing_current_mA.U));
    TERN_(V_SENSORLESS, SERIAL_ECHOPGM_P(SP_V_STR, homing_current_mA.V));
    TERN_(W_SENSORLESS, SERIAL_ECHOPGM_P(SP_W_STR, homing_current_mA.W));
    SERIAL_EOL();

    #if X2_SENSORLESS || Y2_SENSORLESS || Z2_SENSORLESS
      say_M920(forReplay, 1);
      TERN_(X2_SENSORLESS, SERIAL_ECHOPGM_P(SP_X_STR, homing_current_mA.X2));
      TERN_(Y2_SENSORLESS, SERIAL_ECHOPGM_P(SP_Y_STR, homing_current_mA.Y2));
      TERN_(Z2_SENSORLESS, SERIAL_ECHOPGM_P(SP_Z_STR, homing_current_mA.Z2));
      SERIAL_EOL();
    #endif

    #if Z3_SENSORLESS
      say_M920(forReplay, 2);
      SERIAL_ECHOLNPGM(" Z", homing_current_mA.Z3);
    #endif

    #if Z4_SENSORLESS
      say_M920(forReplay, 3);
      SERIAL_ECHOLNPGM(" Z", homing_current_mA.Z4);
    #endif

  #endif // X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS
}

#endif // EDITABLE_HOMING_CURRENT
