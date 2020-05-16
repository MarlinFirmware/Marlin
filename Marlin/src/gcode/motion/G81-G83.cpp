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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(DRILLING_CANNED_CYCLES)

//#define DEBUG_DRILLING_CANNED_CYCLES

#include "../../gcode/gcode.h"
#include "../../feature/bedlevel/bedlevel.h"

#include "../../MarlinCore.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/motion.h"
#include "../../module/tool_change.h"
#include "../../module/temperature.h"
#include "../../lcd/ultralcd.h"

#define DEBUG_OUT ENABLED(DEBUG_DRILLING_CANNED_CYCLES)
#include "../../core/debug_out.h"

bool fast_move; // = false

/**
 * G81 Peck Drill Canned Cycle
 *
 *  X Y - Hole position
 *  Z - Hole Depth - Relative to the Retraction plan R
 *  R - Position on Retraction plan
 *  Q - Max Depth for each Cut/Peck
 *  P - Dwell time on bottom of each Cut/Peck
 *  F - Cutting feedrate
 *
 */
void GcodeSuite::G81() {
  static float G81_Z, G81_R, G81_Q, G81_F;
  static millis_t G81_P;

  #ifdef G0_FEEDRATE
    float g0_feedrate_mm_s, saved_feedrate_mm_s = feedrate_mm_s;
  #endif

  if (parser.seenval('Z')) G81_Z = parser.value_linear();
  if (parser.seenval('R')) G81_R = parser.value_float();
  if (parser.seenval('Q')) G81_Q = parser.value_linear();
  if (parser.seenval('F')) {
    G81_F = parser.value_feedrate();
    feedrate_mm_s = G81_F;
  }

  DEBUG_ECHOLNPGM("Z", G81_Z);
  DEBUG_ECHOLNPGM("R", G81_R);
  DEBUG_ECHOLNPGM("Q", G81_Q);
  DEBUG_ECHOLNPGM("F", MMS_TO_MMM(G81_F));

  g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);

  xyz_pos_t sav_pos = current_position;

  const float z_zero_pos = sav_pos.z - G81_R,
              z_end_pos = z_zero_pos + G81_Z;

  destination.set(sav_pos.x, sav_pos.y, z_zero_pos + G81_R);
  prepare_internal_move_to_destination();
  planner.synchronize();

  float G81_N_D = G81_Q ? z_zero_pos + G81_R - G81_Q : z_end_pos;

  while (z_end_pos < G81_N_D) {

    destination.z = G81_N_D;
    prepare_internal_move_to_destination();
    planner.synchronize();

    dwell(G81_P);

    feedrate_mm_s = g0_feedrate_mm_s;

    destination.z = z_zero_pos + G81_R;
    prepare_internal_move_to_destination();
    planner.synchronize();

    feedrate_mm_s = G81_F;

    G81_N_D -= G81_Q;
  }

  destination.z = z_zero_pos + G81_Z;
  prepare_internal_move_to_destination();
  planner.synchronize();

  dwell(G81_P);

  feedrate_mm_s = g0_feedrate_mm_s;

  destination.z = z_zero_pos + G81_R;
  prepare_internal_move_to_destination();
  planner.synchronize();

  #ifdef G0_FEEDRATE
    // Restore the motion mode feedrate
    if (fast_move) feedrate_mm_s = saved_feedrate_mm_s;
  #endif
}

/**
 * G82 Peck Drill Canned Cycle
 *
 *  X Y - Hole position
 *  Z - Hole Depth - Relative to the Retraction plan R
 *  R - Position on Retraction plan
 *  Q - Max Depth for each Cut/Peck
 *  P - Dwell time on bottom of each Cut/Peck
 *  F - Cutting feedrate
 *
 */
void GcodeSuite::G82() {
  static float G82_Z, G82_R, G82_Q, G82_F;
  static millis_t G82_P;

  #ifdef G0_FEEDRATE
    float g0_feedrate_mm_s, saved_feedrate_mm_s = feedrate_mm_s;
  #endif

  if (parser.seenval('Z')) G82_Z = parser.value_linear();
  DEBUG_ECHOLNPGM("Z", G82_Z);

  if (parser.seenval('R')) G82_R = parser.value_float();
  DEBUG_ECHOLNPGM("R", G82_R);

  if (parser.seenval('P')) G82_P = parser.value_millis();
  DEBUG_ECHOLNPGM("P", G82_P);

  if (parser.seenval('Q')) G82_Q = parser.value_linear();
  DEBUG_ECHOLNPGM("Q", G82_Q);

  if (parser.seenval('F')) {
    G82_F = parser.value_feedrate();
    feedrate_mm_s = G82_F;
  }
  DEBUG_ECHOLNPGM("F", MMS_TO_MMM(G82_F));

  g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);

  xyz_pos_t sav_pos = current_position;

  const float z_zero_pos = sav_pos.z - G82_R,
              z_end_pos = z_zero_pos + G82_Z;

  destination.set(sav_pos.x, sav_pos.y, z_zero_pos + G82_R);
  prepare_internal_move_to_destination();
  planner.synchronize();

  float G82_N_D = G82_Q ? z_zero_pos + G82_R - G82_Q : z_end_pos;

  while (z_end_pos < G82_N_D) {

    destination.z = G82_N_D;
    prepare_internal_move_to_destination();
    planner.synchronize();

    dwell(G82_P);

    feedrate_mm_s = g0_feedrate_mm_s;

    destination.z = z_zero_pos + G82_R;
    prepare_internal_move_to_destination();
    planner.synchronize();

    feedrate_mm_s = G82_F;

    G82_N_D -= G82_Q;
  }

  destination.z = z_zero_pos + G82_Z;
  prepare_internal_move_to_destination();
  planner.synchronize();

  dwell(G82_P);

  feedrate_mm_s = g0_feedrate_mm_s;

  destination.z = z_zero_pos + G82_R;
  prepare_internal_move_to_destination();
  planner.synchronize();

  #ifdef G0_FEEDRATE
    // Restore the motion mode feedrate
    if (fast_move) feedrate_mm_s = saved_feedrate_mm_s;
  #endif
}

/**
 * G83 Peck Drill Canned Cycle
 *
 *  X Y - Hole position
 *  Z - Hole Depth - Relative to the Retraction plan R
 *  R - Position on Retraction plan
 *  Q - Max Depth for each Cut/Peck
 *  P - Dwell time on bottom of each Cut/Peck
 *  F - Cutting feedrate
 *
 */
void GcodeSuite::G83() {
  static float G83_Z, G83_R, G83_Q, G83_F;
  static millis_t G83_P;

  #ifdef G0_FEEDRATE
    float g0_feedrate_mm_s, saved_feedrate_mm_s = feedrate_mm_s;
  #endif

  if (parser.seenval('Z')) G83_Z = parser.value_linear();
  if (parser.seenval('R')) G83_R = parser.value_float();
  if (parser.seenval('Q')) G83_Q = parser.value_linear();
  if (parser.seenval('P')) G83_P = parser.value_millis();

  DEBUG_ECHOLNPGM("Z", G83_Z);
  DEBUG_ECHOLNPGM("R", G83_R);
  DEBUG_ECHOLNPGM("Q", G83_Q);
  DEBUG_ECHOLNPGM("P", G83_P);

  if (parser.seenval('F')) {
    G83_F = parser.value_feedrate();
    feedrate_mm_s = G83_F;
  }
  DEBUG_ECHOLNPGM("F", MMS_TO_MMM(G83_F));

  g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);

  xyz_pos_t sav_pos = current_position;

  const float z_zero_pos = sav_pos.z - G83_R,
              z_end_pos = z_zero_pos + G83_Z;

  destination.set(sav_pos.x, sav_pos.y, z_zero_pos + G83_R);
  prepare_internal_move_to_destination();
  planner.synchronize();

  float N_D = G83_Q ? z_zero_pos + G83_R - G83_Q : z_end_pos;

  while (z_end_pos < N_D) {

    destination.z = N_D;
    prepare_internal_move_to_destination();
    planner.synchronize();

    dwell(G83_P);

    feedrate_mm_s = g0_feedrate_mm_s;

    destination.z = z_zero_pos + G83_R;
    prepare_internal_move_to_destination();
    planner.synchronize();

    feedrate_mm_s = G83_F;

    N_D -= G83_Q;
  }

  destination.z = z_zero_pos + G83_Z;
  prepare_internal_move_to_destination();
  planner.synchronize();

  dwell(G83_P);

  feedrate_mm_s = g0_feedrate_mm_s;

  destination.z = z_zero_pos + G83_R;
  prepare_internal_move_to_destination();
  planner.synchronize();

  #ifdef G0_FEEDRATE
    // Restore the motion mode feedrate
    if (fast_move) feedrate_mm_s = saved_feedrate_mm_s;
  #endif
}

#endif // DRILLING_CANNED_CYCLES
