/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if HAS_LCD_MENU && ENABLED(LCD_BED_LEVELING)

#include "menu.h"
#include "../../module/planner.h"
#include "../../feature/bedlevel/bedlevel.h"

#if ENABLED(PROBE_MANUALLY) || ENABLED(MESH_BED_LEVELING)

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

  bool lcd_wait_for_move;

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
    if (!lcd_wait_for_move) {
      #if MANUAL_PROBE_HEIGHT > 0 && DISABLED(MESH_BED_LEVELING)
        // Display "Done" screen and wait for moves to complete
        line_to_z(MANUAL_PROBE_HEIGHT);
        lcd_synchronize(PSTR(MSG_LEVEL_BED_DONE));
      #endif
      lcd_goto_previous_menu_no_defer();
      lcd_completion_feedback();
    }
    if (lcdDrawUpdate) lcd_implementation_drawmenu_static(LCD_HEIGHT >= 4 ? 1 : 0, PSTR(MSG_LEVEL_BED_DONE));
    lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
  }

  void _lcd_level_goto_next_point();

  //
  // Step 7: Get the Z coordinate, click goes to the next point or exits
  //
  void _lcd_level_bed_get_z() {
    ENCODER_DIRECTION_NORMAL();

    if (use_click()) {

      //
      // Save the current Z position and move
      //

      // If done...
      if (++manual_probe_index >= total_probe_points) {
        //
        // The last G29 records the point and enables bed leveling
        //
        lcd_wait_for_move = true;
        lcd_goto_screen(_lcd_level_bed_done);
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
    if (encoderPosition) {
      const float z = current_position[Z_AXIS] + float((int32_t)encoderPosition) * (MBL_Z_STEP);
      line_to_z(constrain(z, -(LCD_PROBE_Z_RANGE) * 0.5f, (LCD_PROBE_Z_RANGE) * 0.5f));
      lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
      encoderPosition = 0;
    }

    //
    // Draw on first display, then only on Z change
    //
    if (lcdDrawUpdate) {
      const float v = current_position[Z_AXIS];
      lcd_implementation_drawedit(PSTR(MSG_MOVE_Z), ftostr43sign(v + (v < 0 ? -0.0001f : 0.0001f), '+'));
    }
  }

  //
  // Step 6: Display "Next point: 1 / 9" while waiting for move to finish
  //
  void _lcd_level_bed_moving() {
    if (lcdDrawUpdate) {
      char msg[10];
      sprintf_P(msg, PSTR("%i / %u"), (int)(manual_probe_index + 1), total_probe_points);
      lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_NEXT_POINT), msg);
    }
    lcdDrawUpdate = LCDVIEW_CALL_NO_REDRAW;
    if (!lcd_wait_for_move) lcd_goto_screen(_lcd_level_bed_get_z);
  }

  //
  // Step 5: Initiate a move to the next point
  //
  void _lcd_level_goto_next_point() {
    lcd_goto_screen(_lcd_level_bed_moving);

    // G29 Records Z, moves, and signals when it pauses
    lcd_wait_for_move = true;
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
    if (lcdDrawUpdate) lcd_implementation_drawedit(PSTR(MSG_LEVEL_BED_WAITING));
    if (use_click()) {
      manual_probe_index = 0;
      _lcd_level_goto_next_point();
    }
  }

  //
  // Step 3: Display "Homing XYZ" - Wait for homing to finish
  //
  void _lcd_level_bed_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) lcd_goto_screen(_lcd_level_bed_homing_done);
  }

  #if ENABLED(PROBE_MANUALLY)
    extern bool g29_in_progress;
  #endif

  //
  // Step 2: Continue Bed Leveling...
  //
  void _lcd_level_bed_continue() {
    defer_return_to_status = true;
    axis_homed = 0;
    lcd_goto_screen(_lcd_level_bed_homing);
    enqueue_and_echo_commands_P(PSTR("G28"));
  }

#endif // PROBE_MANUALLY || MESH_BED_LEVELING

#if ENABLED(LEVEL_BED_CORNERS)

  /**
   * Level corners, starting in the front-left corner.
   */
  static int8_t bed_corner;
  void _lcd_goto_next_corner() {
    line_to_z(4.0);
    switch (bed_corner) {
      case 0:
        current_position[X_AXIS] = X_MIN_BED + LEVEL_CORNERS_INSET;
        current_position[Y_AXIS] = Y_MIN_BED + LEVEL_CORNERS_INSET;
        break;
      case 1:
        current_position[X_AXIS] = X_MAX_BED - LEVEL_CORNERS_INSET;
        break;
      case 2:
        current_position[Y_AXIS] = Y_MAX_BED - LEVEL_CORNERS_INSET;
        break;
      case 3:
        current_position[X_AXIS] = X_MIN_BED + LEVEL_CORNERS_INSET;
        break;
      #if ENABLED(LEVEL_CENTER_TOO)
        case 4:
          current_position[X_AXIS] = X_CENTER;
          current_position[Y_AXIS] = Y_CENTER;
          break;
      #endif
    }
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[X_AXIS]), active_extruder);
    line_to_z(0.0);
    if (++bed_corner > 3
      #if ENABLED(LEVEL_CENTER_TOO)
        + 1
      #endif
    ) bed_corner = 0;
  }

  void _lcd_corner_submenu() {
    START_MENU();
    MENU_ITEM(function,
      #if ENABLED(LEVEL_CENTER_TOO)
        MSG_LEVEL_BED_NEXT_POINT
      #else
        MSG_NEXT_CORNER
      #endif
      , _lcd_goto_next_corner);
    MENU_ITEM(function, MSG_BACK, lcd_goto_previous_menu_no_defer);
    END_MENU();
  }

  void _lcd_level_bed_corners_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) {
      bed_corner = 0;
      lcd_goto_screen(_lcd_corner_submenu);
      _lcd_goto_next_corner();
    }
  }

  void _lcd_level_bed_corners() {
    defer_return_to_status = true;
    if (!all_axes_known()) {
      axis_homed = 0;
      enqueue_and_echo_commands_P(PSTR("G28"));
    }
    lcd_goto_screen(_lcd_level_bed_corners_homing);
  }

#endif // LEVEL_BED_CORNERS

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
  #if DISABLED(PROBE_MANUALLY) && DISABLED(MESH_BED_LEVELING)
    if (!is_homed) MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
  #endif

  // Level Bed
  #if ENABLED(PROBE_MANUALLY) || ENABLED(MESH_BED_LEVELING)
    // Manual leveling uses a guided procedure
    MENU_ITEM(submenu, MSG_LEVEL_BED, _lcd_level_bed_continue);
  #else
    // Automatic leveling can just run the G-code
    MENU_ITEM(gcode, MSG_LEVEL_BED, is_homed ? PSTR("G29") : PSTR("G28\nG29"));
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
  // MBL Z Offset
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
    // Move to the next corner for leveling
    MENU_ITEM(submenu, MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
  #endif

  #if ENABLED(EEPROM_SETTINGS)
    MENU_ITEM(function, MSG_LOAD_EEPROM, lcd_load_settings);
    MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
  #endif
  END_MENU();
}

#endif // HAS_LCD_MENU && LCD_BED_LEVELING
