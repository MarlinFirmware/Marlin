/**
 * Bed Level Tools for Pro UI
 * Extended by: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 5.1.0
 * Date: 2023/08/22
 *
 * Based on the original work of: Henri-J-Norden
 * https://github.com/Jyers/Marlin/pull/126
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

#include "../../inc/MarlinConfigPre.h"

#if HAS_LEVELING

#include "../../lcd/marlinui.h"
#include "../../core/types.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/probe.h"
#include "../../gcode/gcode.h"
#include "../../module/planner.h"
#include "../../gcode/queue.h"
#include "../../libs/least_squares_fit.h"
#include "../../libs/vector_3.h"

#if ENABLED(DWIN_LCD_PROUI)
  #include "../../lcd/e3v2/proui/dwin.h"
  #include "../../lcd/e3v2/proui/dwinui.h"
  #include "../../lcd/e3v2/proui/dwin_popup.h"
#endif

#include "bedlevel_tools.h"

BedLevelTools bedLevelTools;

bool BedLevelTools::goto_mesh_value = false;
uint8_t BedLevelTools::mesh_x = 0;
uint8_t BedLevelTools::mesh_y = 0;
uint8_t BedLevelTools::tilt_grid = 1;

bool drawing_mesh = false;

#if ENABLED(AUTO_BED_LEVELING_UBL)

  void BedLevelTools::manualValueUpdate(const uint8_t mesh_x, const uint8_t mesh_y, bool undefined/*=false*/) {
    MString<MAX_CMD_SIZE> cmd;
    cmd.set(F("M421 I"), mesh_x, 'J', mesh_y, 'Z', p_float_t(current_position.z, 3));
    if (undefined) cmd += F(" N");
    gcode.process_subcommands_now(cmd);
    planner.synchronize();
  }

  bool BedLevelTools::createPlaneFromMesh() {
    struct linear_fit_data lsf_results;
    incremental_LSF_reset(&lsf_results);
    GRID_LOOP(x, y) {
      if (!isnan(bedlevel.z_values[x][y])) {
        xy_pos_t rpos = { bedlevel.get_mesh_x(x), bedlevel.get_mesh_y(y) };
        incremental_LSF(&lsf_results, rpos, bedlevel.z_values[x][y]);
      }
    }

    if (finish_incremental_LSF(&lsf_results)) {
      SERIAL_ECHOPGM("Could not complete LSF!");
      return true;
    }

    bedlevel.set_all_mesh_points_to_value(0);

    matrix_3x3 rotation = matrix_3x3::create_look_at(vector_3(lsf_results.A, lsf_results.B, 1));
    GRID_LOOP(i, j) {
      float mx = bedlevel.get_mesh_x(i), my = bedlevel.get_mesh_y(j), mz = bedlevel.z_values[i][j];

      if (DEBUGGING(LEVELING)) {
        DEBUG_ECHOLN(F("before rotation = ["), p_float_t(mx, 7), C(','), p_float_t(my, 7), C(','), p_float_t(mz, 7), F("]   ---> "));
        DEBUG_DELAY(20);
      }

      rotation.apply_rotation_xyz(mx, my, mz);

      if (DEBUGGING(LEVELING)) {
        DEBUG_ECHOLN(F("after rotation = ["), p_float_t(mx, 7), C(','), p_float_t(my, 7), C(','), p_float_t(mz, 7), F("]   ---> "));
        DEBUG_DELAY(20);
      }

      bedlevel.z_values[i][j] = mz - lsf_results.D;
    }
    return false;
  }

#else

  void BedLevelTools::manualValueUpdate(const uint8_t mesh_x, const uint8_t mesh_y) {
    gcode.process_subcommands_now(
      TS(F("G29 I"), mesh_x, 'J', mesh_y, 'Z', p_float_t(current_position.z, 3))
    );
    planner.synchronize();
  }

#endif

void BedLevelTools::manualMove(const uint8_t mesh_x, const uint8_t mesh_y, bool zmove/*=false*/) {
  gcode.process_subcommands_now(F("G28O"));
  if (zmove) {
    planner.synchronize();
    current_position.z = goto_mesh_value ? bedlevel.z_values[mesh_x][mesh_y] : Z_CLEARANCE_BETWEEN_PROBES;
    planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
    planner.synchronize();
  }
  else {
    #if ENABLED(DWIN_LCD_PROUI)
      dwinShowPopup(ICON_BLTouch, F("Moving to Point"), F("Please wait until done."));
      hmiSaveProcessID(ID_NothingToDo);
    #endif
    gcode.process_subcommands_now(TS(F("G0 F300 Z"), p_float_t(Z_CLEARANCE_BETWEEN_PROBES, 3)));
    gcode.process_subcommands_now(TS(F("G42 F4000 I"), mesh_x, F(" J"), mesh_y));
    planner.synchronize();
    current_position.z = goto_mesh_value ? bedlevel.z_values[mesh_x][mesh_y] : Z_CLEARANCE_BETWEEN_PROBES;
    planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
    planner.synchronize();
    TERN_(DWIN_LCD_PROUI, hmiReturnScreen());
  }
}

// Move / Probe methods. As examples, not yet used.
void BedLevelTools::moveToXYZ() {
  goto_mesh_value = true;
  manualMove(mesh_x, mesh_y, false);
}

void BedLevelTools::moveToXY() {
  goto_mesh_value = false;
  manualMove(mesh_x, mesh_y, false);
}

void BedLevelTools::moveToZ() {
  goto_mesh_value = true;
  manualMove(mesh_x, mesh_y, true);
}

void BedLevelTools::probeXY() {
  gcode.process_subcommands_now(
    MString<MAX_CMD_SIZE>(
      F("G0Z"), uint16_t(Z_CLEARANCE_DEPLOY_PROBE),
      F("\nG30X"), p_float_t(bedlevel.get_mesh_x(mesh_x), 2),
      F("Y"), p_float_t(bedlevel.get_mesh_y(mesh_y), 2)
    )
  );
}

void BedLevelTools::meshReset() {
  ZERO(bedlevel.z_values);
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    bedlevel.refresh_bed_level();
  #endif
}

// Return 'true' if mesh is good and within limits
bool BedLevelTools::meshValidate() {
  if (MESH_MAX_X <= MESH_MIN_X || MESH_MAX_Y <= MESH_MIN_Y) return false;
  GRID_LOOP(x, y) {
    const float v = bedlevel.z_values[x][y];
    if (isnan(v) || !WITHIN(v, UBL_Z_OFFSET_MIN, UBL_Z_OFFSET_MAX)) return false;
  }
  return true;
}

#endif // HAS_LEVELING
