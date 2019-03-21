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

//
// Bed Leveling Menus
//

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(LCD_BED_LEVELING)

#include "menu.h"
#include "../../module/planner.h"
#include "../../feature/bedlevel/bedlevel.h"

#if HAS_BED_PROBE && DISABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../module/probe.h"
#endif

#if EITHER(PROBE_MANUALLY, MESH_BED_LEVELING)

  #include "../../module/motion.h"
  #include "../../gcode/queue.h"

  //
  // Motion > Level Bed handlers
  //

  static uint8_t manual_probe_index;

  // LCD probed points are from defaults
  constexpr uint8_t total_probe_points = (
    #if ENABLED(AUTO_BED_LEVELING_3POINT)
      3
    #elif ABL_GRID || ENABLED(MESH_BED_LEVELING)
      GRID_MAX_POINTS
    #endif
  );

  bool MarlinUI::wait_for_bl_move; // = false

  //
  // Bed leveling is done. Wait for G29 to complete.
  // A flag is used so that this can release control
  // and allow the command queue to be processed.
  //
  // When G29 finishes the last move:
  // - Raise Z to the "manual probe height"
  // - Don't return until done.
  //
  // ** This blocks the command queue! **
  //
  void _lcd_level_bed_done() {
    if (!ui.wait_for_bl_move) {
      #if MANUAL_PROBE_HEIGHT > 0 && DISABLED(MESH_BED_LEVELING)
        // Display "Done" screen and wait for moves to complete
        line_to_z(MANUAL_PROBE_HEIGHT);
        ui.synchronize(PSTR(MSG_LEVEL_BED_DONE));
      #endif
      ui.goto_previous_screen_no_defer();
      ui.completion_feedback();
    }
    if (ui.should_draw()) draw_menu_item_static(LCD_HEIGHT >= 4 ? 1 : 0, PSTR(MSG_LEVEL_BED_DONE));
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  }

  void _lcd_level_goto_next_point();

  //
  // Step 7: Get the Z coordinate, click goes to the next point or exits
  //
  void _lcd_level_bed_get_z() {
    ui.encoder_direction_normal();

    if (ui.use_click()) {

      //
      // Save the current Z position and move
      //

      // If done...
      if (++manual_probe_index >= total_probe_points) {
        //
        // The last G29 records the point and enables bed leveling
        //
        ui.wait_for_bl_move = true;
        ui.goto_screen(_lcd_level_bed_done);
        #if ENABLED(MESH_BED_LEVELING)
          enqueue_and_echo_commands_P(PSTR("G29 S2"));
        #elif ENABLED(PROBE_MANUALLY)
          enqueue_and_echo_commands_P(PSTR("G29 V1"));
        #endif
      }
      else
        _lcd_level_goto_next_point();

      return;
    }

    //
    // Encoder knob or keypad buttons adjust the Z position
    //
    if (ui.encoderPosition) {
      const float z = current_position[Z_AXIS] + float((int32_t)ui.encoderPosition) * (MESH_EDIT_Z_STEP);
      line_to_z(constrain(z, -(LCD_PROBE_Z_RANGE) * 0.5f, (LCD_PROBE_Z_RANGE) * 0.5f));
      ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
      ui.encoderPosition = 0;
    }

    //
    // Draw on first display, then only on Z change
    //
    if (ui.should_draw()) {
      const float v = current_position[Z_AXIS];
      draw_edit_screen(PSTR(MSG_MOVE_Z), ftostr43sign(v + (v < 0 ? -0.0001f : 0.0001f), '+'));
    }
  }

  //
  // Step 6: Display "Next point: 1 / 9" while waiting for move to finish
  //
  void _lcd_level_bed_moving() {
    if (ui.should_draw()) {
      char msg[10];
      sprintf_P(msg, PSTR("%i / %u"), (int)(manual_probe_index + 1), total_probe_points);
      draw_edit_screen(PSTR(MSG_LEVEL_BED_NEXT_POINT), msg);
    }
    ui.refresh(LCDVIEW_CALL_NO_REDRAW);
    if (!ui.wait_for_bl_move) ui.goto_screen(_lcd_level_bed_get_z);
  }

  //
  // Step 5: Initiate a move to the next point
  //
  void _lcd_level_goto_next_point() {
    ui.goto_screen(_lcd_level_bed_moving);

    // G29 Records Z, moves, and signals when it pauses
    ui.wait_for_bl_move = true;
    #if ENABLED(MESH_BED_LEVELING)
      enqueue_and_echo_commands_P(manual_probe_index ? PSTR("G29 S2") : PSTR("G29 S1"));
    #elif ENABLED(PROBE_MANUALLY)
      enqueue_and_echo_commands_P(PSTR("G29 V1"));
    #endif
  }

  //
  // Step 4: Display "Click to Begin", wait for click
  //         Move to the first probe position
  //
  void _lcd_level_bed_homing_done() {
    if (ui.should_draw()) draw_edit_screen(PSTR(MSG_LEVEL_BED_WAITING));
    if (ui.use_click()) {
      manual_probe_index = 0;
      _lcd_level_goto_next_point();
    }
  }

  //
  // Step 3: Display "Homing XYZ" - Wait for homing to finish
  //
  void _lcd_level_bed_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) ui.goto_screen(_lcd_level_bed_homing_done);
  }

  #if ENABLED(PROBE_MANUALLY)
    extern bool g29_in_progress;
  #endif

  //
  // Step 2: Continue Bed Leveling...
  //
  void _lcd_level_bed_continue() {
    ui.defer_status_screen(true);
    set_all_unhomed();
    ui.goto_screen(_lcd_level_bed_homing);
    enqueue_and_echo_commands_P(PSTR("G28"));
  }

#endif // PROBE_MANUALLY || MESH_BED_LEVELING

#if ENABLED(MESH_EDIT_MENU)

  inline void refresh_planner() {
    set_current_from_steppers_for_axis(ALL_AXES);
    sync_plan_position();
  }

  void menu_edit_mesh() {
    static uint8_t xind, yind; // =0
    START_MENU();
    MENU_BACK(MSG_BED_LEVELING);
    MENU_ITEM_EDIT(uint8, MSG_MESH_X, &xind, 0, GRID_MAX_POINTS_X - 1);
    MENU_ITEM_EDIT(uint8, MSG_MESH_Y, &yind, 0, GRID_MAX_POINTS_Y - 1);
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float43, MSG_MESH_EDIT_Z, &Z_VALUES(xind, yind), -(LCD_PROBE_Z_RANGE) * 0.5, (LCD_PROBE_Z_RANGE) * 0.5, refresh_planner);
    END_MENU();
  }

#endif // MESH_EDIT_MENU

/**
 * Step 1: Bed Level entry-point
 *
 * << Motion
 *    Auto Home           (if homing needed)
 *    Leveling On/Off     (if data exists, and homed)
 *    Fade Height: ---    (Req: ENABLE_LEVELING_FADE_HEIGHT)
 *    Mesh Z Offset: ---  (Req: MESH_BED_LEVELING)
 *    Z Probe Offset: --- (Req: HAS_BED_PROBE, Opt: BABYSTEP_ZPROBE_OFFSET)
 *    Level Bed >
 *    Level Corners >     (if homed)
 *    Load Settings       (Req: EEPROM_SETTINGS)
 *    Save Settings       (Req: EEPROM_SETTINGS)
 */
void menu_bed_leveling() {
  START_MENU();
  MENU_BACK(MSG_MOTION);

  const bool is_homed = all_axes_known();

  // Auto Home if not using manual probing
  #if DISABLED(PROBE_MANUALLY, MESH_BED_LEVELING)
    if (!is_homed) MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
  #endif

  // Level Bed
  #if EITHER(PROBE_MANUALLY, MESH_BED_LEVELING)
    // Manual leveling uses a guided procedure
    MENU_ITEM(submenu, MSG_LEVEL_BED, _lcd_level_bed_continue);
  #else
    // Automatic leveling can just run the G-code
    MENU_ITEM(gcode, MSG_LEVEL_BED, is_homed ? PSTR("G29") : PSTR("G28\nG29"));
  #endif

  #if ENABLED(MESH_EDIT_MENU)
    if (leveling_is_valid())
      MENU_ITEM(submenu, MSG_EDIT_MESH, menu_edit_mesh);
  #endif

  // Homed and leveling is valid? Then leveling can be toggled.
  if (is_homed && leveling_is_valid()) {
    bool new_level_state = planner.leveling_active;
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_BED_LEVELING, &new_level_state, _lcd_toggle_bed_leveling);
  }

  // Z Fade Height
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float3, MSG_Z_FADE_HEIGHT, &lcd_z_fade_height, 0, 100, _lcd_set_z_fade_height);
  #endif

  //
  // Mesh Bed Leveling Z-Offset
  //
  #if ENABLED(MESH_BED_LEVELING)
    MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
  #endif

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
  #elif HAS_BED_PROBE
    MENU_ITEM_EDIT(float52, MSG_ZPROBE_ZOFFSET, &zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX);
  #endif

  #if ENABLED(LEVEL_BED_CORNERS)
    MENU_ITEM(submenu, MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    MENU_ITEM(function, MSG_LOAD_EEPROM, lcd_load_settings);
    MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
  #endif
  END_MENU();
}

#endif // LCD_BED_LEVELING
