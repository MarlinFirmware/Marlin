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

//
// Calibrate Probe offset menu.
//

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(PROBE_OFFSET_MESH)

#include "menu_item.h"
#include "../../module/planner.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/motion.h"
#include "../../gcode/queue.h"
#include "../../module/probe.h"

bed_mesh_t z_offset_values;

//
// Motion > Level Bed handlers
//

static uint8_t manual_probe_index;
static xy_int8_t meshCount;
xy_pos_t probePos;
static constexpr xy_uint8_t grid_points = {GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y};
bool zig;

#if ABL_USES_GRID
#if ENABLED(PROBE_Y_FIRST)
#define PR_OUTER_VAR meshCount.x
#define PR_OUTER_SIZE grid_points.x
#define PR_INNER_VAR meshCount.y
#define PR_INNER_SIZE grid_points.y
#else
#define PR_OUTER_VAR meshCount.y
#define PR_OUTER_SIZE grid_points.y
#define PR_INNER_VAR meshCount.x
#define PR_INNER_SIZE grid_points.x
#endif
#endif

// LCD probed points are from defaults
constexpr uint8_t total_probe_points = TERN(AUTO_BED_LEVELING_3POINT, 3, GRID_MAX_POINTS);

//
// Bed leveling is done. Wait for G29 to complete.
// A flag is used so that this can release control
// and allow the command queue to be processed.
//
// When G29 finishes the last move:
// - Raise Z to the "Z after probing" height
// - Don't return until done.
//
// ** This blocks the command queue! **
//
void _lcd_probe_offset_mesh_done()
{
  if (!ui.wait_for_move)
  {
    ui.goto_previous_screen_no_defer();
    ui.completion_feedback();
  }
  if (ui.should_draw())
    MenuItem_static::draw(LCD_HEIGHT >= 4, GET_TEXT(MSG_PROBE_MESH_DONE));
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
}

void _lcd_level_goto_next_point();

//
// Step 7: Get the Z coordinate, click goes to the next point or exits
//
void _lcd_probe_offset_mesh_get_z()
{

  if (ui.use_click())
  {

    //
    // Save the current Z position and move
    //

    // If done...
    if (++manual_probe_index >= total_probe_points)
    {
      //
      // The last G29 records the point and enables bed leveling
      //
      ui.wait_for_move = true;
      ui.goto_screen(_lcd_probe_offset_mesh_done);
#if ENABLED(MESH_BED_LEVELING)
      queue.inject_P(PSTR("G29S2"));
#elif ENABLED(PROBE_MANUALLY)
      queue.inject_P(PSTR("G29V1"));
#endif
    }
    else
      _lcd_level_goto_next_point();

    return;
  }

  //
  // Encoder knob or keypad buttons adjust the Z position
  //
  if (ui.encoderPosition)
  {
    const float z = current_position.z + float(int32_t(ui.encoderPosition)) * (MESH_EDIT_Z_STEP);
    line_to_z(constrain(z, -(LCD_PROBE_Z_RANGE)*0.5f, (LCD_PROBE_Z_RANGE)*0.5f));
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
    ui.encoderPosition = 0;
  }

  //
  // Draw on first display, then only on Z change
  //
  if (ui.should_draw())
  {
    const float v = current_position.z;
    MenuEditItemBase::draw_edit_screen(GET_TEXT(MSG_MOVE_Z), ftostr43sign(v + (v < 0 ? -0.0001f : 0.0001f), '+'));
  }
}

//
// Step 6: Display "Next point: 1 / 9" while waiting for move to finish
//
void _lcd_probe_offset_mesh_moving()
{
  if (ui.should_draw())
  {
    char msg[10];
    sprintf_P(msg, PSTR("%i / %u"), int(manual_probe_index + 1), total_probe_points);
    MenuEditItemBase::draw_edit_screen(GET_TEXT(MSG_LEVEL_BED_NEXT_POINT), msg);
  }
  ui.refresh(LCDVIEW_CALL_NO_REDRAW);
  if (!ui.wait_for_move)
    ui.goto_screen(_lcd_probe_offset_mesh_get_z);
}

//
// Step 5: Initiate a move to the next point
//
void _lcd_level_goto_next_point()
{
  ui.goto_screen(_lcd_probe_offset_mesh_moving);

  if (manual_probe_index < total_probe_points)
  {

    // Set meshCount.x, meshCount.y based on manual_probe_index, with zig-zag
    PR_OUTER_VAR = manual_probe_index / PR_INNER_SIZE;
    PR_INNER_VAR = manual_probe_index - (PR_OUTER_VAR * PR_INNER_SIZE);

    // Probe in reverse order for every other row/column
    const bool zig = (PR_OUTER_VAR & 1); // != ((PR_OUTER_SIZE) & 1);
    if (zig)
      PR_INNER_VAR = (PR_INNER_SIZE - 1) - PR_INNER_VAR;

    probePos = bilinear_start + bilinear_grid_spacing * meshCount.asFloat();

    // Move Nozzle to Probing/Homing Position
    ui.wait_for_move = true;
    current_position = probePos;
    line_to_current_position(MMM_TO_MMS(XY_PROBE_FEEDRATE));
    probe.probe_at_point(probePos, PROBE_PT_RAISE);
    //ui.synchronize(GET_TEXT(MSG_PROBE_WIZARD_MOVING));
    ui.wait_for_move = false;
  }

#if ENABLED(MESH_BED_LEVELING)
  queue.inject_P(manual_probe_index ? PSTR("G29S2") : PSTR("G29S1"));
#elif ENABLED(PROBE_MANUALLY)
  queue.inject_P(PSTR("G29V1"));
#endif
}

//
// Step 4: Display "Click to Begin", wait for click
//         Move to the first probe position
//
void _lcd_probe_offset_mesh_homing_done()
{
  if (ui.should_draw())
  {
    MenuItem_static::draw(1, GET_TEXT(MSG_LEVEL_BED_WAITING));
// Color UI needs a control to detect a touch
#if BOTH(TOUCH_SCREEN, HAS_GRAPHICAL_TFT)
    touch.add_control(CLICK, 0, 0, TFT_WIDTH, TFT_HEIGHT);
#endif
  }
  if (ui.use_click())
  {
    
    // Initialize grid variables in case they are not initialized yet
    if (bilinear_grid_spacing.x == 0 && bilinear_grid_spacing.y == 0){
      bilinear_grid_spacing.set((probe.max_x() - probe.min_x()) / (grid_points.x - 1),
                                (probe.max_y() - probe.min_y()) / (grid_points.y - 1));
      bilinear_start.set(probe.min_x(), probe.min_y());
    }

    zig = PR_OUTER_SIZE & 1; // Always end at RIGHT and BACK_PROBE_BED_POSITION
    manual_probe_index = 0;
    _lcd_level_goto_next_point();
  }
}

//
// Step 3: Display "Homing XYZ" - Wait for homing to finish
//
void _lcd_probe_offset_mesh_homing()
{
  _lcd_draw_homing();
  if (all_axes_homed())
    ui.goto_screen(_lcd_probe_offset_mesh_homing_done);
}

//
// Step 2: Continue Z-Offset Mesh Creation...
//
void _lcd_probe_offset_mesh_continue()
{
  ui.defer_status_screen();
  set_all_unhomed();
  ui.goto_screen(_lcd_probe_offset_mesh_homing);
  queue.inject_P(G28_STR);
}

#endif // PROBE_OFFSET_MESH
