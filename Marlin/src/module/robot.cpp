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

/**
 * scara.cpp
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(IS_ROBOT)

  #include "robot.h"
  #include "motion.h"
  #include "planner.h"

  float delta_segments_per_second = ROBOT_SEGMENTS_PER_SECOND;

  void robot_set_axis_is_at_home(const AxisEnum axis) {
    if (axis == Z_AXIS)
      current_position.z = Z_HOME_POS;
    else {

      /**
       * Robot home order, eg. XY at the same time
       */
      xyz_pos_t homeposition;
      LOOP_XYZ(i) homeposition[i] = base_home_pos((AxisEnum)i);

      #if ENABLED(MORGAN_SCARA)
        // MORGAN_SCARA uses arm angles for AB home position
        // SERIAL_ECHOLNPAIR("homeposition A:", homeposition.a, " B:", homeposition.b);
        inverse_kinematics(homeposition);
        forward_kinematics_robot(delta.a, delta.b);
        current_position[axis] = cartes[axis];
      #else
        // MP_SCARA uses a Cartesian XY home position
        // SERIAL_ECHOPGM("homeposition");
        // SERIAL_ECHOLNPAIR_P(SP_X_LBL, homeposition.x, SP_Y_LBL, homeposition.y);
        current_position[axis] = homeposition[axis];
      #endif

      // SERIAL_ECHOPGM("Cartesian");
      // SERIAL_ECHOLNPAIR_P(SP_X_LBL, current_position.x, SP_Y_LBL, current_position.y);
      update_software_endstops(axis);
    }
  }

  //static constexpr xy_pos_t scara_offset = { SCARA_OFFSET_X, SCARA_OFFSET_Y };
  static constexpr xyz_pos_t robot_offset = { ROBOT_OFFSET_X, ROBOT_OFFSET_Y, ROBOT_OFFSET_Z };

  /**
   * Morgan SCARA Forward Kinematics. Results in 'cartes'.
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   */
  void forward_kinematics_robot(const float &a, const float &b) {

    const float a_sin = sin(RADIANS(a)) * L1,
                a_cos = cos(RADIANS(a)) * L1,
                b_sin = sin(RADIANS(b)) * L2,
                b_cos = cos(RADIANS(b)) * L2;

    cartes.set(a_cos + b_cos + robot_offset.x,  // theta
              a_sin + b_sin + robot_offset.y); // theta+phi

    /* 
      SERIAL_ECHOLNPAIR(
        "SCARA FK Angle a=", a,
        " b=", b,
        " a_sin=", a_sin,
        " a_cos=", a_cos,
        " b_sin=", b_sin,
        " b_cos=", b_cos
      );
      SERIAL_ECHOLNPAIR(" cartes (X,Y) = (",cartes.x, ", ", cartes.y, ")");
     */
  }

  void inverse_kinematics(const xyz_pos_t &raw) {
    #if ENABLED(SCARA_KINEMATICS)
      // SCARA KINE should be added here...
      float a; 
      

    #elif ENABLED(TPARA_KINEMATICS)

      float CG, GAMMA, THETA, PHI, PSI;
      
      const xyz_pos_t spos = raw - robot_offset;

      const float RHO = SQRT(sq(spos.x) + sq(spos.y) + sq(spos.z) );
      const float RHO2 = sq(spos.x) + sq(spos.y) + sq(spos.z) ;

      const float LSS = L1_2 + L2_2 ;
      const float LM = 2.0f * L1 * L2 ;

      CG = (LSS - RHO2)/LM ;

      // Angle of Body Joint
      THETA = ATAN2(spos.y, spos.x);

      // Angle of Elbow Joint
      GAMMA = ACOS(CG);

      // Angle of Shoulder Joint
      PHI = asin (spos.z/RHO) +  asin (L2*sin(GAMMA)/ RHO )  ;   

      PSI = PHI + GAMMA; 

      delta.set(DEGREES(THETA), DEGREES(PHI), DEGREES(PSI));

      /**/
        //DEBUG_POS("SCARA IK", raw);
        //DEBUG_POS("SCARA IK", delta);
        
        //SERIAL_ECHOLNPAIR(" SCARA (x,y,z) ", spos.x , ",", spos.y, ",", spos.z, " Rho=", RHO, " Rho2=", RHO2, " Theta=", THETA, " Phi=", PHI, " Psi=", PSI, " Gamma=", GAMMA);
      ///**/

    #endif
  }

  void robot_report_positions() {
    #if ENABLED(SCARA_KINEMATICS)
      //SERIAL_ECHOLNPAIR("SCARA Theta:", planner.get_axis_position_degrees(A_AXIS), "  Psi+Theta:", planner.get_axis_position_degrees(B_AXIS));

    #elif ENABLED(TPARA_KINEMATICS)      
      SERIAL_ECHOLNPAIR("SCARA Theta:", planner.get_axis_position_degrees(A_AXIS), "  Phi:", planner.get_axis_position_degrees(B_AXIS), "  Psi:", planner.get_axis_position_degrees(C_AXIS));
    #endif
    SERIAL_EOL();
  }

#endif // IS_ROBOT
