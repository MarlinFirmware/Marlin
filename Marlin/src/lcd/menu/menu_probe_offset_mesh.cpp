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
#include "menu_addon.h"
#include "../../module/planner.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../feature/bedlevel/abl/probe_offset_mesh.h"
#include "../../module/motion.h"
#include "../../gcode/queue.h"
#include "../../module/probe.h"

//
// Motion > Level Bed handlers
//

static uint8_t manual_probe_index;
static xy_int8_t meshCount;
xy_pos_t probePos;
static constexpr xy_uint8_t grid_points = {GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y};
bool zig;
int8_t inStart, inStop, inInc;
float measured_z;
bool was_leveling_active;

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
    print_z_offset_grid();
    set_bed_leveling_enabled(was_leveling_active);
    SET_SOFT_ENDSTOP_LOOSE(false);
    ui.goto_screen(menu_advanced_settings);
  }
  if (ui.should_draw())
    MenuItem_static::draw(LCD_HEIGHT >= 4, GET_TEXT(MSG_PROBE_MESH_DONE));
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
}

void _lcd_level_goto_next_point_inner();
void _lcd_level_goto_next_point_outer();

#if ENABLED(PROBE_OFFSET_WIZARD)
  void _goto_manual_move_z(const_float_t);
#else
  void _goto_manual_move_z(const_float_t scale) {
    ui.manual_move.menu_scale = scale;
    ui.goto_screen(lcd_move_z);
  }
#endif

void set_offset_and_go_to_next_point() {
  //Set Z offset at probed point
  z_offset_mesh[meshCount.x][meshCount.y] = current_position.z - measured_z;

  //Go to next point
  PR_INNER_VAR += inInc;
  manual_probe_index++;
  ui.goto_screen(_lcd_level_goto_next_point_inner);
}

void probe_offset_mesh_wizard_menu() {
  START_MENU();
  float calculated_z_offset = probe.offset.z + current_position.z - measured_z;

  if (LCD_HEIGHT >= 4)
    STATIC_ITEM(MSG_MOVE_NOZZLE_TO_BED, SS_CENTER|SS_INVERT);

  STATIC_ITEM_P(PSTR("Z="), SS_CENTER, ftostr42_52(current_position.z));
  STATIC_ITEM(MSG_ZPROBE_ZOFFSET, SS_LEFT, ftostr42_52(calculated_z_offset));

  SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move_z( 1);    });
  SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move_z( 0.1f); });

  if ((FINE_MANUAL_MOVE) > 0.0f && (FINE_MANUAL_MOVE) < 0.1f) {
    char tmp[20], numstr[10];
    // Determine digits needed right of decimal
    const uint8_t digs = !UNEAR_ZERO((FINE_MANUAL_MOVE) * 1000 - int((FINE_MANUAL_MOVE) * 1000)) ? 4 :
                         !UNEAR_ZERO((FINE_MANUAL_MOVE) *  100 - int((FINE_MANUAL_MOVE) *  100)) ? 3 : 2;
    sprintf_P(tmp, GET_TEXT(MSG_MOVE_N_MM), dtostrf(FINE_MANUAL_MOVE, 1, digs, numstr));
    #if DISABLED(HAS_GRAPHICAL_TFT)
      SUBMENU_P(NUL_STR, []{ _goto_manual_move_z(float(FINE_MANUAL_MOVE)); });
      MENU_ITEM_ADDON_START(0 + ENABLED(HAS_MARLINUI_HD44780));
      lcd_put_u8str(tmp);
      MENU_ITEM_ADDON_END();
    #else
      SUBMENU_P(tmp, []{ _goto_manual_move_z(float(FINE_MANUAL_MOVE)); });
    #endif
  }

  ACTION_ITEM(MSG_BUTTON_DONE, set_offset_and_go_to_next_point);

  END_MENU();
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
  if (!ui.wait_for_move){
    ui.goto_screen(probe_offset_mesh_wizard_menu);
  }
}

//
// Step 5: Initiate a move to the next point
//
void _lcd_level_goto_next_point_inner()
{

  if (PR_INNER_VAR != inStop) {
    // Avoid probing outside the round or hexagonal area
    if (!TERN0(IS_KINEMATIC, !probe.can_reach(abl.probePos))){
      ui.wait_for_move = true;
      ui.goto_screen(_lcd_probe_offset_mesh_moving);

      // Deploy certain probes before starting probing
      #if HAS_BED_PROBE
        if (ENABLED(BLTOUCH))
          do_z_clearance(Z_CLEARANCE_DEPLOY_PROBE);
      #endif

      probePos = bilinear_start + bilinear_grid_spacing * meshCount.asFloat();
      measured_z = probe.probe_at_point(probePos, PROBE_PT_STOW);
      current_position += probe.offset_xy;
      line_to_current_position(MMM_TO_MMS(XY_PROBE_FEEDRATE));
      ui.wait_for_move = false;
    } else {
      //Go to next point
      PR_INNER_VAR += inInc;
      manual_probe_index++;
      //ui.goto_screen(_lcd_level_goto_next_point_inner);
    }
  } else {
    PR_OUTER_VAR++;
    ui.goto_screen(_lcd_level_goto_next_point_outer);
  }
}

//
// Step 5: Initiate a move to the next point
//
void _lcd_level_goto_next_point_outer()
{
  if (PR_OUTER_VAR < PR_OUTER_SIZE) {

    if (zig) {                      // Zig away from origin
        inStart = 0;                  // Left or front
        inStop = PR_INNER_SIZE;       // Right or back
        inInc = 1;                    // Zig right
      }
      else {                          // Zag towards origin
        inStart = PR_INNER_SIZE - 1;  // Right or back
        inStop = -1;                  // Left or front
        inInc = -1;                   // Zag left
      }

      zig ^= true; // zag

      PR_INNER_VAR = inStart;
      ui.goto_screen(_lcd_level_goto_next_point_inner);
  } else {
    ui.goto_screen(_lcd_probe_offset_mesh_done);
  }
}

//
// Step 4: Display "Click to Begin", wait for click
//         Move to the first probe position
//
void _lcd_probe_offset_mesh_homing_done() {
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
    PR_OUTER_VAR = 0;
    manual_probe_index = 0;

    SET_SOFT_ENDSTOP_LOOSE(true); // Disable soft endstops for free Z movement

    ui.goto_screen(_lcd_level_goto_next_point_outer);
  }
}

//
// Step 3: Display "Homing XYZ" - Wait for homing to finish
//
void _lcd_probe_offset_mesh_homing() {
  _lcd_draw_homing();
  if (all_axes_homed())
    ui.goto_screen(_lcd_probe_offset_mesh_homing_done);
}

//
// Step 2: Continue Z-Offset Mesh Creation...
//
void _lcd_probe_offset_mesh_continue() {

  // Store Bed-Leveling-State and disable
  #if HAS_LEVELING
    was_leveling_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  // Home all axes
  ui.defer_status_screen();
  set_all_unhomed();
  ui.goto_screen(_lcd_probe_offset_mesh_homing);
  queue.inject_P(G28_STR);
}

#endif // PROBE_OFFSET_MESH
