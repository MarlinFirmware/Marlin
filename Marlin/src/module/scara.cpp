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

#if IS_SCARA  //suggestion: Rename to IS_ROBOT

  #include "scara.h"
  #include "motion.h"
  #include "planner.h"

  #if EITHER(MORGAN_SCARA, MP_SCARA)
    float delta_segments_per_second = SCARA_SEGMENTS_PER_SECOND;  // suggestion rename to ROBOT_SEGMENTS_PER_SECOND
  #else // AXEL_TPARA
    float delta_segments_per_second = ROBOT_SEGMENTS_PER_SECOND;  // suggestion rename to ROBOT_SEGMENTS_PER_SECOND
  #endif
  

  void scara_set_axis_is_at_home(const AxisEnum axis) { // suggestion rename
    if (axis == Z_AXIS)
      current_position.z = Z_HOME_POS;
    else {

      /**
       * SCARA homes XY at the same time
       */
      xyz_pos_t homeposition;
      LOOP_XYZ(i) homeposition[i] = base_home_pos((AxisEnum)i);

      #if ENABLED(MORGAN_SCARA)
        // MORGAN_SCARA uses arm angles for AB home position
        // SERIAL_ECHOLNPAIR("homeposition A:", homeposition.a, " B:", homeposition.b);
        inverse_kinematics(homeposition);
        forward_kinematics_SCARA(delta.a, delta.b);
        current_position[axis] = cartes[axis];
      #elif ENABLED(MP_SCARA)
        // MP_SCARA uses a Cartesian XY home position
        // SERIAL_ECHOPGM("homeposition");
        // SERIAL_ECHOLNPAIR_P(SP_X_LBL, homeposition.x, SP_Y_LBL, homeposition.y);
        current_position[axis] = homeposition[axis];
      #else  // AXEL_TPARA, TODO, check this 
        inverse_kinematics(homeposition);
        forward_kinematics_SCARA(delta.a, delta.b);
        current_position[axis] = cartes[axis];      
      #endif

      // SERIAL_ECHOPGM("Cartesian");
      // SERIAL_ECHOLNPAIR_P(SP_X_LBL, current_position.x, SP_Y_LBL, current_position.y);
      update_software_endstops(axis);
    }
  }

  #if ENABLED(AXEL_TPARA)
    static constexpr xyz_pos_t robot_offset = { ROBOT_OFFSET_X, ROBOT_OFFSET_Y, ROBOT_OFFSET_Z };  
  #else
    static constexpr xy_pos_t scara_offset = { SCARA_OFFSET_X, SCARA_OFFSET_Y };
  #endif

#if ENABLED(AXEL_TPARA)
  void forward_kinematics_SCARA(const float &a, const float &b) {
    // Needs rework 
    const float a_sin = sin(RADIANS(a)) * L1,
                a_cos = cos(RADIANS(a)) * L1,
                b_sin = sin(RADIANS(b)) * L2,
                b_cos = cos(RADIANS(b)) * L2;

    cartes.set(a_cos + b_cos + robot_offset.x,  
              a_sin + b_sin + robot_offset.y, 
              robot_offset.z); 
  }
#else  
  /**
   * Morgan SCARA Forward Kinematics. Results in 'cartes'.
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   */
  void forward_kinematics_SCARA(const float &a, const float &b) {
  
    const float a_sin = sin(RADIANS(a)) * L1,
                a_cos = cos(RADIANS(a)) * L1,
                b_sin = sin(RADIANS(b)) * L2,
                b_cos = cos(RADIANS(b)) * L2;

    cartes.set(a_cos + b_cos + scara_offset.x,  // theta
              a_sin + b_sin + scara_offset.y); // theta+phi

    /*
      SERIAL_ECHOLNPAIR(
        "SCARA FK Angle a=", a,
        " b=", b,
        " a_sin=", a_sin,
        " a_cos=", a_cos,
        " b_sin=", b_sin,
        " b_cos=", b_cos
      );
      SERIAL_ECHOLNPAIR(" cartes (X,Y) = "(cartes.x, ", ", cartes.y, ")");
    //*/
  }
 #endif

  void inverse_kinematics(const xyz_pos_t &raw) {

    #if ENABLED(MORGAN_SCARA)
      /**
       * Morgan SCARA Inverse Kinematics. Results are stored in 'delta'.
       *
       * See https://reprap.org/forum/read.php?185,283327
       *
       * Maths and first version by QHARLEY.
       * Integrated into Marlin and slightly restructured by Joachim Cerny.
       */
      float C2, S2, SK1, SK2, THETA, PSI;

      // Translate SCARA to standard XY with scaling factor
      const xy_pos_t spos = raw - scara_offset;

      const float H2 = HYPOT2(spos.x, spos.y);
      if (L1 == L2)
        C2 = H2 / L1_2_2 - 1;
      else
        C2 = (H2 - (L1_2 + L2_2)) / (2.0f * L1 * L2);

      S2 = SQRT(1.0f - sq(C2));

      // Unrotated Arm1 plus rotated Arm2 gives the distance from Center to End
      SK1 = L1 + L2 * C2;

      // Rotated Arm2 gives the distance from Arm1 to Arm2
      SK2 = L2 * S2;

      // Angle of Arm1 is the difference between Center-to-End angle and the Center-to-Elbow
      THETA = ATAN2(SK1, SK2) - ATAN2(spos.x, spos.y);

      // Angle of Arm2
      PSI = ATAN2(S2, C2);

      delta.set(DEGREES(THETA), DEGREES(THETA + PSI), raw.z);

      /*
        DEBUG_POS("SCARA IK", raw);
        DEBUG_POS("SCARA IK", delta);
        SERIAL_ECHOLNPAIR("  SCARA (x,y) ", sx, ",", sy, " C2=", C2, " S2=", S2, " Theta=", THETA, " Phi=", PHI);
      //*/

    #elif ENABLED(MP_SCARA)// MP_SCARA

      const float x = raw.x, y = raw.y, c = HYPOT(x, y),
                  THETA3 = ATAN2(y, x),
                  THETA1 = THETA3 + ACOS((sq(c) + sq(L1) - sq(L2)) / (2.0f * c * L1)),
                  THETA2 = THETA3 - ACOS((sq(c) + sq(L2) - sq(L1)) / (2.0f * c * L2));

      delta.set(DEGREES(THETA1), DEGREES(THETA2), raw.z);

      /*
        DEBUG_POS("SCARA IK", raw);
        DEBUG_POS("SCARA IK", delta);
        SERIAL_ECHOLNPAIR("  SCARA (x,y) ", x, ",", y," Theta1=", THETA1, " Theta2=", THETA2);
      //*/

    #elif ENABLED(AXEL_TPARA)
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

          
        //SERIAL_ECHOLNPAIR(" SCARA (x,y,z) ", spos.x , ",", spos.y, ",", spos.z, " Rho=", RHO, " Rho2=", RHO2, " Theta=", THETA, " Phi=", PHI, " Psi=", PSI, " Gamma=", GAMMA);

    #endif // AXEL_TPARA
  }

  void scara_report_positions() {
    #if EITHER(MORGAN_SCARA, MP_SCARA)
      SERIAL_ECHOLNPAIR("SCARA Theta:", planner.get_axis_position_degrees(A_AXIS), "  Psi+Theta:", planner.get_axis_position_degrees(B_AXIS));
    #elif ENABLED(AXEL_TPARA)
      SERIAL_ECHOLNPAIR("SCARA Theta:", planner.get_axis_position_degrees(A_AXIS), "  Phi:", planner.get_axis_position_degrees(B_AXIS), "  Psi:", planner.get_axis_position_degrees(C_AXIS));
    #endif
    SERIAL_EOL();
  }

#endif // IS_SCARA
