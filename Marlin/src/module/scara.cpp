/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * scara.cpp
 */

#include "../inc/MarlinConfig.h"

#if IS_SCARA

#include "scara.h"
#include "motion.h"
#include "planner.h"

float delta_segments_per_second = SCARA_SEGMENTS_PER_SECOND;

void scara_set_axis_is_at_home(const AxisEnum axis) {
  if (axis == Z_AXIS)
    current_position[Z_AXIS] = Z_HOME_POS;
  else {

    /**
     * SCARA homes XY at the same time
     */
    float homeposition[XYZ];
    LOOP_XYZ(i) homeposition[i] = base_home_pos((AxisEnum)i);

    // SERIAL_ECHOPAIR("homeposition X:", homeposition[X_AXIS]);
    // SERIAL_ECHOLNPAIR(" Y:", homeposition[Y_AXIS]);

    /**
     * Get Home position SCARA arm angles using inverse kinematics,
     * and calculate homing offset using forward kinematics
     */
    inverse_kinematics(homeposition);
    forward_kinematics_SCARA(delta[A_AXIS], delta[B_AXIS]);

    // SERIAL_ECHOPAIR("Cartesian X:", cartes[X_AXIS]);
    // SERIAL_ECHOLNPAIR(" Y:", cartes[Y_AXIS]);

    current_position[axis] = cartes[axis];

    update_software_endstops(axis);
  }
}

/**
 * Morgan SCARA Forward Kinematics. Results in cartes[].
 * Maths and first version by QHARLEY.
 * Integrated into Marlin and slightly restructured by Joachim Cerny.
 */
void forward_kinematics_SCARA(const float &a, const float &b) {

  const float a_sin = sin(RADIANS(a)) * L1,
              a_cos = cos(RADIANS(a)) * L1,
              b_sin = sin(RADIANS(b)) * L2,
              b_cos = cos(RADIANS(b)) * L2;

  cartes[X_AXIS] = a_cos + b_cos + SCARA_OFFSET_X;  //theta
  cartes[Y_AXIS] = a_sin + b_sin + SCARA_OFFSET_Y;  //theta+phi

  /*
    SERIAL_ECHOPAIR("SCARA FK Angle a=", a);
    SERIAL_ECHOPAIR(" b=", b);
    SERIAL_ECHOPAIR(" a_sin=", a_sin);
    SERIAL_ECHOPAIR(" a_cos=", a_cos);
    SERIAL_ECHOPAIR(" b_sin=", b_sin);
    SERIAL_ECHOLNPAIR(" b_cos=", b_cos);
    SERIAL_ECHOPAIR(" cartes[X_AXIS]=", cartes[X_AXIS]);
    SERIAL_ECHOLNPAIR(" cartes[Y_AXIS]=", cartes[Y_AXIS]);
  //*/
}

/**
 * Morgan SCARA Inverse Kinematics. Results in delta[].
 *
 * See http://forums.reprap.org/read.php?185,283327
 *
 * Maths and first version by QHARLEY.
 * Integrated into Marlin and slightly restructured by Joachim Cerny.
 */
void inverse_kinematics(const float (&raw)[XYZ]) {

  static float C2, S2, SK1, SK2, THETA, PSI;

  float sx = raw[X_AXIS] - SCARA_OFFSET_X,  // Translate SCARA to standard X Y
        sy = raw[Y_AXIS] - SCARA_OFFSET_Y;  // With scaling factor.

  if (L1 == L2)
    C2 = HYPOT2(sx, sy) / L1_2_2 - 1;
  else
    C2 = (HYPOT2(sx, sy) - (L1_2 + L2_2)) / (2.0 * L1 * L2);

  S2 = SQRT(1 - sq(C2));

  // Unrotated Arm1 plus rotated Arm2 gives the distance from Center to End
  SK1 = L1 + L2 * C2;

  // Rotated Arm2 gives the distance from Arm1 to Arm2
  SK2 = L2 * S2;

  // Angle of Arm1 is the difference between Center-to-End angle and the Center-to-Elbow
  THETA = ATAN2(SK1, SK2) - ATAN2(sx, sy);

  // Angle of Arm2
  PSI = ATAN2(S2, C2);

  delta[A_AXIS] = DEGREES(THETA);        // theta is support arm angle
  delta[B_AXIS] = DEGREES(THETA + PSI);  // equal to sub arm angle (inverted motor)
  delta[C_AXIS] = raw[Z_AXIS];

  /*
    DEBUG_POS("SCARA IK", raw);
    DEBUG_POS("SCARA IK", delta);
    SERIAL_ECHOPAIR("  SCARA (x,y) ", sx);
    SERIAL_ECHOPAIR(",", sy);
    SERIAL_ECHOPAIR(" C2=", C2);
    SERIAL_ECHOPAIR(" S2=", S2);
    SERIAL_ECHOPAIR(" Theta=", THETA);
    SERIAL_ECHOLNPAIR(" Phi=", PHI);
  //*/
}

void scara_report_positions() {
  SERIAL_ECHOPAIR("SCARA Theta:", planner.get_axis_position_degrees(A_AXIS));
  SERIAL_ECHOLNPAIR("   Psi+Theta:", planner.get_axis_position_degrees(B_AXIS));
  SERIAL_EOL();
}

#endif // IS_SCARA
