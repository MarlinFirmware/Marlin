/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"

#if ENABLED(M114_DETAIL)

  #if HAS_DRIVER(L6470)
    //C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\Marlin\src\gcode\host\M114.cpp
    //C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\Marlin\src\module\bob_L6470.cpp
    #include "../../module/L6470/L6470_Marlin.h"
    #define DEBUG_OUT ENABLED(L6470_CHITCHAT)
    #include "../../core/debug_out.h"
  #endif

  void report_xyze(const float pos[], const uint8_t n = 4, const uint8_t precision = 3) {
    char str[12];
    for (uint8_t i = 0; i < n; i++) {
      SERIAL_CHAR(' ');
      SERIAL_CHAR(axis_codes[i]);
      SERIAL_CHAR(':');
      SERIAL_ECHO(dtostrf(pos[i], 8, precision, str));
    }
    SERIAL_EOL();
  }

  inline void report_xyz(const float pos[]) { report_xyze(pos, 3); }

  void report_current_position_detail() {

    SERIAL_ECHOPGM("\nLogical:");
    const float logical[XYZ] = {
      LOGICAL_X_POSITION(current_position[X_AXIS]),
      LOGICAL_Y_POSITION(current_position[Y_AXIS]),
      LOGICAL_Z_POSITION(current_position[Z_AXIS])
    };
    report_xyz(logical);

    SERIAL_ECHOPGM("Raw:    ");
    report_xyz(current_position);

    float leveled[XYZ] = { current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] };

    #if HAS_LEVELING
      SERIAL_ECHOPGM("Leveled:");
      planner.apply_leveling(leveled);
      report_xyz(leveled);

      SERIAL_ECHOPGM("UnLevel:");
      float unleveled[XYZ] = { leveled[X_AXIS], leveled[Y_AXIS], leveled[Z_AXIS] };
      planner.unapply_leveling(unleveled);
      report_xyz(unleveled);
    #endif

    #if IS_KINEMATIC
      #if IS_SCARA
        SERIAL_ECHOPGM("ScaraK: ");
      #else
        SERIAL_ECHOPGM("DeltaK: ");
      #endif
      inverse_kinematics(leveled);  // writes delta[]
      report_xyz(delta);
    #endif

    planner.synchronize();

    #if HAS_DRIVER(L6470)
      char temp_buf[80];
      int32_t temp;
      //#define ABS_POS_SIGN_MASK 0b1111 1111 1110 0000 0000 0000 0000 0000
      #define ABS_POS_SIGN_MASK 0b11111111111000000000000000000000
      #define REPORT_ABSOLUTE_POS(Q) do{                            \
          L6470.say_axis(Q, false);                                 \
          temp = L6470_GETPARAM(L6470_ABS_POS,Q);                   \
          if (temp & ABS_POS_SIGN_MASK) temp |= ABS_POS_SIGN_MASK;  \
          sprintf_P(temp_buf, PSTR(":%8ld   "), temp);              \
          DEBUG_ECHO(temp_buf);                                     \
        }while(0)

      DEBUG_ECHOPGM("\nL6470:");
      #if AXIS_DRIVER_TYPE_X(L6470)
        REPORT_ABSOLUTE_POS(X);
      #endif
      #if AXIS_DRIVER_TYPE_X2(L6470)
        REPORT_ABSOLUTE_POS(X2);
      #endif
      #if AXIS_DRIVER_TYPE_Y(L6470)
        REPORT_ABSOLUTE_POS(Y);
      #endif
      #if AXIS_DRIVER_TYPE_Y2(L6470)
        REPORT_ABSOLUTE_POS(Y2);
      #endif
      #if AXIS_DRIVER_TYPE_Z(L6470)
        REPORT_ABSOLUTE_POS(Z);
      #endif
      #if AXIS_DRIVER_TYPE_Z2(L6470)
        REPORT_ABSOLUTE_POS(Z2);
      #endif
      #if AXIS_DRIVER_TYPE_Z3(L6470)
        REPORT_ABSOLUTE_POS(Z3);
      #endif
      #if AXIS_DRIVER_TYPE_E0(L6470)
        REPORT_ABSOLUTE_POS(E0);
      #endif
      #if AXIS_DRIVER_TYPE_E1(L6470)
        REPORT_ABSOLUTE_POS(E1);
      #endif
      #if AXIS_DRIVER_TYPE_E2(L6470)
        REPORT_ABSOLUTE_POS(E2);
      #endif
      #if AXIS_DRIVER_TYPE_E3(L6470)
        REPORT_ABSOLUTE_POS(E3);
      #endif
      #if AXIS_DRIVER_TYPE_E4(L6470)
        REPORT_ABSOLUTE_POS(E4);
      #endif
      #if AXIS_DRIVER_TYPE_E5(L6470)
        REPORT_ABSOLUTE_POS(E5);
      #endif
      SERIAL_EOL();
    #endif // HAS_DRIVER(L6470)

    SERIAL_ECHOPGM("Stepper:");
    LOOP_XYZE(i) {
      SERIAL_CHAR(' ');
      SERIAL_CHAR(axis_codes[i]);
      SERIAL_CHAR(':');
      SERIAL_ECHO(stepper.position((AxisEnum)i));
    }
    SERIAL_EOL();

    #if IS_SCARA
      const float deg[XYZ] = {
        planner.get_axis_position_degrees(A_AXIS),
        planner.get_axis_position_degrees(B_AXIS)
      };
      SERIAL_ECHOPGM("Degrees:");
      report_xyze(deg, 2);
    #endif

    SERIAL_ECHOPGM("FromStp:");
    get_cartesian_from_steppers();  // writes cartes[XYZ] (with forward kinematics)
    const float from_steppers[XYZE] = { cartes[X_AXIS], cartes[Y_AXIS], cartes[Z_AXIS], planner.get_axis_position_mm(E_AXIS) };
    report_xyze(from_steppers);

    const float diff[XYZE] = {
      from_steppers[X_AXIS] - leveled[X_AXIS],
      from_steppers[Y_AXIS] - leveled[Y_AXIS],
      from_steppers[Z_AXIS] - leveled[Z_AXIS],
      from_steppers[E_AXIS] - current_position[E_AXIS]
    };
    SERIAL_ECHOPGM("Differ: ");
    report_xyze(diff);
  }

#endif // M114_DETAIL

/**
 * M114: Report current position to host
 */
void GcodeSuite::M114() {

  #if ENABLED(M114_DETAIL)
    if (parser.seen('D')) {
      report_current_position_detail();
      return;
    }
  #endif

  planner.synchronize();
  report_current_position();
}
