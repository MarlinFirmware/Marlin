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

#if IS_SCARA

#include "scara.h"
#include "motion.h"
#include "planner.h"

#if ENABLED(AXEL_TPARA)
  #include "endstops.h"
  #include "../MarlinCore.h"
#endif

float segments_per_second = DEFAULT_SEGMENTS_PER_SECOND;

#if ANY(MORGAN_SCARA, MP_SCARA)

  static constexpr xy_pos_t scara_offset = { SCARA_OFFSET_X, SCARA_OFFSET_Y };

  /**
   * Morgan SCARA Forward Kinematics. Results in 'cartes'.
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   */
  void forward_kinematics(const_float_t a, const_float_t b) {
    const float a_sin = sin(RADIANS(a)) * L1,
                a_cos = cos(RADIANS(a)) * L1,
                b_sin = sin(RADIANS(SUM_TERN(MP_SCARA, b, a))) * L2,
                b_cos = cos(RADIANS(SUM_TERN(MP_SCARA, b, a))) * L2;

    cartes.x = a_cos + b_cos + scara_offset.x;  // theta
    cartes.y = a_sin + b_sin + scara_offset.y;  // phi

    /*
      DEBUG_ECHOLNPGM(
        "SCARA FK Angle a=", a,
        " b=", b,
        " a_sin=", a_sin,
        " a_cos=", a_cos,
        " b_sin=", b_sin,
        " b_cos=", b_cos
      );
      DEBUG_ECHOLNPGM(" cartes (X,Y) = "(cartes.x, ", ", cartes.y, ")");
    //*/
  }

#endif

#if ENABLED(MORGAN_SCARA)

  void scara_set_axis_is_at_home(const AxisEnum axis) {
    if (axis == Z_AXIS)
      current_position.z = Z_HOME_POS;
    else {
      // MORGAN_SCARA uses a Cartesian XY home position
      xyz_pos_t homeposition = { X_HOME_POS, Y_HOME_POS, Z_HOME_POS };
      //DEBUG_ECHOLNPGM_P(PSTR("homeposition X"), homeposition.x, SP_Y_LBL, homeposition.y);

      delta = homeposition;
      forward_kinematics(delta.a, delta.b);
      current_position[axis] = cartes[axis];

      //DEBUG_ECHOLNPGM_P(PSTR("Cartesian X"), current_position.x, SP_Y_LBL, current_position.y);
      update_software_endstops(axis);
    }
  }

  /**
   * Morgan SCARA Inverse Kinematics. Results are stored in 'delta'.
   *
   * See https://reprap.org/forum/read.php?185,283327
   *
   * Maths and first version by QHARLEY.
   * Integrated into Marlin and slightly restructured by Joachim Cerny.
   */
  void inverse_kinematics(const xyz_pos_t &raw) {
    float C2, S2, SK1, SK2, THETA, PSI;

    // Translate SCARA to standard XY with scaling factor
    const xy_pos_t spos = raw - scara_offset;

    const float H2 = HYPOT2(spos.x, spos.y);
    if (L1 == L2)
      C2 = H2 / L1_2_2 - 1;
    else
      C2 = (H2 - (L1_2 + L2_2)) / (2.0f * L1 * L2);

    LIMIT(C2, -1, 1);

    S2 = SQRT(1.0f - sq(C2));

    // Unrotated Arm1 plus rotated Arm2 gives the distance from Center to End
    SK1 = L1 + L2 * C2;

    // Rotated Arm2 gives the distance from Arm1 to Arm2
    SK2 = L2 * S2;

    // Angle of Arm1 is the difference between Center-to-End angle and the Center-to-Elbow
    THETA = ATAN2(SK1, SK2) - ATAN2(spos.x, spos.y);

    // Angle of Arm2
    PSI = ATAN2(S2, C2);

    delta.set(DEGREES(THETA), DEGREES(SUM_TERN(MORGAN_SCARA, PSI, THETA)), raw.z);

    /*
      DEBUG_POS("SCARA IK", raw);
      DEBUG_POS("SCARA IK", delta);
      DEBUG_ECHOLNPGM("  SCARA (x,y) ", sx, ",", sy, " C2=", C2, " S2=", S2, " Theta=", THETA, " Psi=", PSI);
    //*/
  }

#elif ENABLED(MP_SCARA)

  void scara_set_axis_is_at_home(const AxisEnum axis) {
    if (axis == Z_AXIS)
      current_position.z = Z_HOME_POS;
    else {
      // MP_SCARA uses arm angles for AB home position
      #ifndef SCARA_OFFSET_THETA1
        #define SCARA_OFFSET_THETA1  12 // degrees
      #endif
      #ifndef SCARA_OFFSET_THETA2
        #define SCARA_OFFSET_THETA2 131 // degrees
      #endif
      ab_float_t homeposition = { SCARA_OFFSET_THETA1, SCARA_OFFSET_THETA2 };
      //DEBUG_ECHOLNPGM("homeposition A:", homeposition.a, " B:", homeposition.b);

      inverse_kinematics(homeposition);
      forward_kinematics(delta.a, delta.b);
      current_position[axis] = cartes[axis];

      //DEBUG_ECHOLNPGM_P(PSTR("Cartesian X"), current_position.x, SP_Y_LBL, current_position.y);
      update_software_endstops(axis);
    }
  }

  void inverse_kinematics(const xyz_pos_t &raw) {
    const float x = raw.x, y = raw.y, c = HYPOT(x, y),
                THETA3 = ATAN2(y, x),
                THETA1 = THETA3 + ACOS((sq(c) + sq(L1) - sq(L2)) / (2.0f * c * L1)),
                THETA2 = THETA3 - ACOS((sq(c) + sq(L2) - sq(L1)) / (2.0f * c * L2));

    delta.set(DEGREES(THETA1), DEGREES(THETA2), raw.z);

    /*
      DEBUG_POS("SCARA IK", raw);
      DEBUG_POS("SCARA IK", delta);
      SERIAL_ECHOLNPGM("  SCARA (x,y) ", x, ",", y," Theta1=", THETA1, " Theta2=", THETA2);
    //*/
  }

#elif ENABLED(AXEL_TPARA)
  // TPARA offset relative to the origin of the robot
  static constexpr xyz_pos_t robot_shoulder_offset = { 0, 0, TPARA_SHOULDER_AXIS_HEIGHT };
  // Workspace offset relative to the origin of the robot
  constexpr xyz_pos_t robot_workspace_offset = { TPARA_OFFSET_X, TPARA_OFFSET_Y, TPARA_OFFSET_Z };
  // Tool offset relative to the tool center point of the robot
  constexpr xyz_pos_t tool_offset = { TPARA_TCP_OFFSET_X, TPARA_TCP_OFFSET_Y, TPARA_TCP_OFFSET_Z };
  // Tool offset in cylindrical coordinates (r, phi, z)
  static const xyz_pos_t tool_offset_cyl = { SQRT(sq(TPARA_TCP_OFFSET_X) + sq(TPARA_TCP_OFFSET_Y)) , ATAN2(TPARA_TCP_OFFSET_Y, TPARA_TCP_OFFSET_X), TPARA_TCP_OFFSET_Z };

  //xyz_pos_t home_t_w_offset = tool_offset - robot_workspace_offset;

  // Remove offset for calculation with trigonometric
  // Tool offset coordinates are recalculated for each angle
  xyz_pos_t remove_W_T_offset(const xyz_pos_t &raw) {
  xyz_pos_t toolhead_absolute = raw + robot_workspace_offset; // Remove workspace offset first, so we can use trigonometrics relative to robot reference frame (otherwise a negative raw position would "mirror/invert" the tool offset)

  // We should apply a rotation matrix, but is too costly to calculate sin and cos
  const float r2 = HYPOT2(toolhead_absolute.x, toolhead_absolute.y);
  xyz_pos_t tool_offset_rotated;
  if (UNEAR_ZERO(r2)) {
    // avoid zero div
    tool_offset_rotated.x = tool_offset_cyl.x;
    tool_offset_rotated.y = 0.0f;
    tool_offset_rotated.z = tool_offset_cyl.z;
  }
  else {
    const float inv_r = 1.0f / sqrtf(r2);
    tool_offset_rotated.x = tool_offset_cyl.x * toolhead_absolute.x * inv_r;  // Equivalent to tool_offset_cyl.x * cos(atan2(toolhead_absolute.y, toolhead_absolute.x))
    tool_offset_rotated.y = tool_offset_cyl.x * toolhead_absolute.y * inv_r;  // Equivalent to tool_offset_cyl.x * sin(atan2(toolhead_absolute.y, toolhead_absolute.x))
    tool_offset_rotated.z = tool_offset_cyl.z;
  }

  return toolhead_absolute - tool_offset_rotated; // Returns the real robot pose without tool or workspace offset
}

// Apply tool and workspace offset to robot flange pose, accounting for the rotated tool offset
xyz_pos_t apply_T_W_offset(const xyz_pos_t &rpose) {
  // We should apply a rotation matrix, but it's too costly
  const float r2 = rpose.x * rpose.x + rpose.y * rpose.y;
  xyz_pos_t tool_offset_rotated;
  if (UNEAR_ZERO(r2)) {
    // avoid zero div
    tool_offset_rotated.x = tool_offset_cyl.x;
    tool_offset_rotated.y = 0.0f;
    tool_offset_rotated.z = tool_offset_cyl.z;
  }
  else {
    const float inv_r = 1.0f / sqrtf(r2);
    tool_offset_rotated.x = tool_offset_cyl.x * rpose.x * inv_r;
    tool_offset_rotated.y = tool_offset_cyl.x * rpose.y * inv_r;
    tool_offset_rotated.z = tool_offset_cyl.z;
  }
  // SERIAL_ECHOLNPGM(" Tool_offset_rotated(x,y,z) ", tool_offset_rotated.x, ",", tool_offset_rotated.y, ",", tool_offset_rotated.z );

  return rpose + tool_offset_rotated - robot_workspace_offset;
}

/**
 * Set an axis' current position to its home position (after homing).
 *
 * TPARA should wait until all YZ homing is done before setting the YZ
 * current_position to home, because neither Y nor Z is at home until
 * both are at home.
 *
 */
  void scara_set_axis_is_at_home(const AxisEnum axis) {
      // Home position should be arm end position -+ offsets (+ tool offset - workspace offset), measured at home robot pose
      xyz_pos_t homeposition = { X_HOME_POS , Y_HOME_POS , Z_HOME_POS };

      // SERIAL_ECHOLNPGM("TPARA Set axis is at home: ", axis );
      // DEBUG_ECHOLNPGM_P(PSTR("homeposition X"), homeposition.x, SP_Y_LBL, homeposition.y, SP_Z_LBL, homeposition.z);
      // SERIAL_ECHOLNPGM("Home: ", homeposition.x, ",", homeposition.y, ",", homeposition.z);
      // SERIAL_ECHOLNPGM("Pos before IK: ", current_position.x, ",", current_position.y, ",", current_position.z);
      // SERIAL_ECHOLNPGM("Angles Before: Theta: ", delta.a, " Phi: ", delta.b, " Psi: ", delta.c);
      inverse_kinematics(homeposition);
      // SERIAL_ECHOLNPGM("Angles After IK: Theta: ", delta.a, " Phi: ", delta.b, " Psi: ", delta.c);
      forward_kinematics(delta.a, delta.b, delta.c);
      current_position[axis] = cartes[axis];

      // SERIAL_ECHOLNPGM("Curr Pos after FK: ", current_position.x, ",", current_position.y, ",", current_position.z);
      // SERIAL_ECHOLNPGM("Cartes after FK: ", cartes.x, ",", cartes.y, ",", cartes.z);
      // DEBUG_ECHOLNPGM_P(PSTR("Cartesian X"), current_position.x, SP_Y_LBL, current_position.y);
      update_software_endstops(axis);

      // SERIAL_ECHOLNPGM("Final Angles: Theta: ", delta.a, " Phi: ", delta.b, " Psi: ", delta.c);
      // SERIAL_ECHOLNPGM("Final Pos: ", current_position.x, ",", current_position.y, ",", current_position.z);
      // SERIAL_ECHOLNPGM("Robot Offsets Shoulder:", robot_shoulder_offset.x, "," , robot_shoulder_offset.y,"," , robot_shoulder_offset.z);
      // SERIAL_ECHOLNPGM("Robot Offsets Tool:", tool_offset.x, "," , tool_offset.y,"," , tool_offset.z);
      // SERIAL_ECHOLNPGM("Robot Offsets Workspace:", robot_workspace_offset.x, "," , robot_workspace_offset.y,"," , robot_workspace_offset.z);
      // SERIAL_EOL();

  }

  // Convert ABC inputs in degrees to XYZ outputs in mm
  void forward_kinematics(const_float_t a, const_float_t b, const_float_t c) {
    const float w = c - b,
                r = L1 * cos(RADIANS(b)) + L2 * sin(RADIANS(w - (90 - b))),
                x = r  * cos(RADIANS(a)),
                y = r  * sin(RADIANS(a)),
                rho2 = L1_2 + L2_2 - 2.0f * L1 * L2 * cos(RADIANS(w));

    const xyz_pos_t calculated_fk = xyz_pos_t({ x, y, SQRT(rho2 - sq(x) - sq(y)) }) ;
    cartes = calculated_fk + robot_shoulder_offset + tool_offset - robot_workspace_offset;

    // SERIAL_ECHOLNPGM("TPARA FK Theta:", a, " Phi: ", b, " Psi: ", c , " Calculated X':", calculated_fk.x, " Y':", calculated_fk.y, " Z':", calculated_fk.z, " Workspace X:", cartes.x, " Y:", cartes.y, " Z:", cartes.z);
    
  }

  // Home YZ together, then X (or all at once). Based on quick_home_xy & home_delta
  void home_TPARA() {
    // First Init the current position of all carriages to 0,0,0
    current_position.reset();
    destination.reset();
    sync_plan_position();

    // SERIAL_ECHOLNPGM("Reset and sync position to the asumed start pose of the robot" );
    // Set the asumed start pose of the robot for homing, so it home ZY axis at same time preserving the B and C motor angle 
    const xyz_pos_t asumed_intial_pose = {L2, 0, 0}; 
    xyz_pos_t intial_pose_w_offset = apply_T_W_offset(asumed_intial_pose);
 
    current_position.set(intial_pose_w_offset.x, intial_pose_w_offset.y, intial_pose_w_offset.z);
    destination.set(intial_pose_w_offset.x, intial_pose_w_offset.y, intial_pose_w_offset.z);
    sync_plan_position();

    // Disable stealthChop if used. Enable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      TERN_(X_SENSORLESS, sensorless_t stealth_states_x = start_sensorless_homing_per_axis(X_AXIS));
      TERN_(Y_SENSORLESS, sensorless_t stealth_states_y = start_sensorless_homing_per_axis(Y_AXIS));
      TERN_(Z_SENSORLESS, sensorless_t stealth_states_z = start_sensorless_homing_per_axis(Z_AXIS));
    #endif

    // Set the homing current for all motors
    TERN_(HAS_HOMING_CURRENT, set_homing_current(Z_AXIS));

    // Move to home, should move Z, Y, then X. Move X to near 0 (to avoid div by zero
    // and sign/angle stability around 0 for trigonometric functions), Y to 0 and Z to max_length
    xyz_pos_t raw_homing_pose_dir = { 1, 0, max_length(Z_AXIS) };

    xyz_pos_t homing_pose_dir = apply_T_W_offset(raw_homing_pose_dir);
    current_position.set(homing_pose_dir.x, homing_pose_dir.y, homing_pose_dir.z);

    line_to_current_position(homing_feedrate(Z_AXIS));
    planner.synchronize();

    // Restore the homing current for all motors
    TERN_(HAS_HOMING_CURRENT, restore_homing_current(Z_AXIS));

    // Re-enable stealthChop if used. Disable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      TERN_(X_SENSORLESS, end_sensorless_homing_per_axis(X_AXIS, stealth_states_x));
      TERN_(Y_SENSORLESS, end_sensorless_homing_per_axis(Y_AXIS, stealth_states_y));
      TERN_(Z_SENSORLESS, end_sensorless_homing_per_axis(Z_AXIS, stealth_states_z));
    #endif

    endstops.validate_homing_move();

    // At least one motor has reached its endstop.
    // Now re-home each motor separately.
    #if ENABLED (INVERT_HOMING_AXIS_ORDER) 
      homeaxis(C_AXIS);
      homeaxis(B_AXIS);
      homeaxis(A_AXIS);
  
      // SERIAL_ECHOLNPGM("current_position After Homeaxis: ", current_position.x, ", ", current_position.y, ", ", current_position.z);     
    
    #else 
      homeaxis(A_AXIS);
      homeaxis(B_AXIS);
      homeaxis(C_AXIS);
    #endif

    // Set all carriages to their home positions
    // Do this here all at once for Delta, because
    // XYZ isn't ABC. Applying this per-tower would
    // give the impression that they are the same.
    LOOP_NUM_AXES(i) set_axis_is_at_home((AxisEnum)i);

    // SERIAL_ECHOLNPGM("Sync_plan_position after home");      
    sync_plan_position();
  }

  void inverse_kinematics(const xyz_pos_t &raw) {
    // Remove offsets to calculate with trigonometric
    const xyz_pos_t tpos = remove_W_T_offset(raw) - robot_shoulder_offset; //raw + robot_workspace_offset  - tool_offset_rotated - robot_shoulder_offset;
    // IK, Refer to TPARA analysis
    const float RXY = SQRT(HYPOT2(tpos.x, tpos.y)),
                RHO_2 = NORMSQ(tpos.x, tpos.y, tpos.z),
                //RHO = SQRT(RHO2),
                LSS = L1_2 + L2_2,  // L1^2 + L2^2 , LSS : Lenght square sum
                LM = 2.0f * L1 * L2, // Length multiplication and doubled

                // Method 2
                CG = (LSS - RHO_2) / LM, // cosine of gamma
                SG = SQRT(1 - POW(CG, 2)),  // sine of gamma
                K1 = L1 - L2 * CG,  // K1 projection
                K2 = L2 * SG,  // K2 projection

                // Angle of Body/base Joint
                THETA = ATAN2(tpos.y, tpos.x),

                // Angle of Elbow Joint, between L1 and L2
                //GAMMA = ACOS(CG),
                GAMMA = ATAN2(SG, CG), // Method 2

                // Angle of Shoulder Joint, elevation angle measured from horizontal plane XY (r+)
                //PHI = asin(tpos.z/RHO) + asin(L2 * sin(GAMMA) / RHO),
                PHI = ATAN2(tpos.z, RXY) + ATAN2(K2, K1),   // Method 2

                // Elbow motor angle measured from horizontal, same reference frame as shoulder angle (r+)
                PSI = PHI + GAMMA;

    delta.set(DEGREES(THETA), DEGREES(PHI), DEGREES(PSI));
    
    // SERIAL_ECHOLNPGM(" TPARA IK raw(x,y,z) ", raw.x, ",", raw.y, ",", raw.z, " Robot pose(x,y,z) ", tpos.x, ",", tpos.y, ",", tpos.z + robot_shoulder_offset.z, " Rho^2=", RHO_2, " Theta=", THETA*RAD_TO_DEG, " Phi=", PHI*RAD_TO_DEG, " Psi=", PSI*RAD_TO_DEG, " Gamma=", GAMMA*RAD_TO_DEG);
 
  }

#endif

void scara_report_positions() {
  SERIAL_ECHOLNPGM(
    #if ENABLED(AXEL_TPARA)
        "TPARA Theta: ", planner.get_axis_position_degrees(A_AXIS)
      , " Phi: ",         planner.get_axis_position_degrees(B_AXIS)
      , " Psi: ",         planner.get_axis_position_degrees(C_AXIS)
    #else
        "SCARA Theta:",                            planner.get_axis_position_degrees(A_AXIS)
      , "  Psi" TERN_(MORGAN_SCARA, "+Theta") ":", planner.get_axis_position_degrees(B_AXIS)
    #endif
  );
  SERIAL_EOL();
}

#endif // IS_SCARA
