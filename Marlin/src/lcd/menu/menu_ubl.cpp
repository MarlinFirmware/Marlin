/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// Unified Bed Leveling Menus
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(AUTO_BED_LEVELING_UBL)

#include "menu.h"
#include "../../module/planner.h"
#include "../../module/configuration_store.h"
#include "../../feature/bedlevel/bedlevel.h"

static int16_t ubl_storage_slot = 0,
               custom_hotend_temp = 190,
               side_points = 3,
               ubl_fillin_amount = 5,
               ubl_height_amount = 1;

static uint8_t n_edit_pts = 1, x_plot = 0, y_plot = 0;

#if HAS_HEATED_BED
  static int16_t custom_bed_temp = 50;
#endif

float mesh_edit_value, mesh_edit_accumulator; // We round mesh_edit_value to 2.5 decimal places. So we keep a
                                              // separate value that doesn't lose precision.
static int16_t ubl_encoderPosition = 0;

static void _lcd_mesh_fine_tune(PGM_P msg) {
  ui.defer_status_screen();
  if (ubl.encoder_diff) {
    ubl_encoderPosition = (ubl.encoder_diff > 0) ? 1 : -1;
    ubl.encoder_diff = 0;

    mesh_edit_accumulator += float(ubl_encoderPosition) * 0.005f * 0.5f;
    mesh_edit_value = mesh_edit_accumulator;
    ui.encoderPosition = 0;
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);

    const int32_t rounded = (int32_t)(mesh_edit_value * 1000);
    mesh_edit_value = float(rounded - (rounded % 5L)) / 1000;
  }

  if (ui.should_draw()) {
    MenuEditItemBase::draw_edit_screen(msg, ftostr43sign(mesh_edit_value));
    #if ENABLED(MESH_EDIT_GFX_OVERLAY)
      _lcd_zoffset_overlay_gfx(mesh_edit_value);
    #endif
  }
}

void _lcd_mesh_edit_NOP() {
  ui.defer_status_screen();
}

float lcd_mesh_edit() {
  ui.goto_screen(_lcd_mesh_edit_NOP);
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  _lcd_mesh_fine_tune(GET_TEXT(MSG_MESH_EDITOR));
  return mesh_edit_value;
}

void lcd_mesh_edit_setup(const float &initial) {
  mesh_edit_value = mesh_edit_accumulator = initial;
  ui.goto_screen(_lcd_mesh_edit_NOP);
}

void _lcd_z_offset_edit() {
  _lcd_mesh_fine_tune(GET_TEXT(MSG_UBL_Z_OFFSET));
}

float lcd_z_offset_edit() {
  ui.goto_screen(_lcd_z_offset_edit);
  return mesh_edit_value;
}

void lcd_z_offset_edit_setup(const float &initial) {
  mesh_edit_value = mesh_edit_accumulator = initial;
  ui.goto_screen(_lcd_z_offset_edit);
}

/**
 * UBL Build Custom Mesh Command
 */
void _lcd_ubl_build_custom_mesh() {
  char ubl_lcd_gcode[20];
  queue.inject_P(G28_STR);
  #if HAS_HEATED_BED
    sprintf_P(ubl_lcd_gcode, PSTR("M190 S%i"), custom_bed_temp);
    lcd_enqueue_one_now(ubl_lcd_gcode);
  #endif
  sprintf_P(ubl_lcd_gcode, PSTR("M109 S%i"), custom_hotend_temp);
  lcd_enqueue_one_now(ubl_lcd_gcode);
  queue.inject_P(PSTR("G29 P1"));
}

/**
 * UBL Custom Mesh submenu
 *
 * << Build Mesh
 *    Hotend Temp: ---
 *    Bed Temp: ---
 *    Build Custom Mesh
 */
void _lcd_ubl_custom_mesh() {
  START_MENU();
  BACK_ITEM(MSG_UBL_BUILD_MESH_MENU);
  EDIT_ITEM(int3, MSG_UBL_HOTEND_TEMP_CUSTOM, &custom_hotend_temp, EXTRUDE_MINTEMP, (HEATER_0_MAXTEMP - 10));
  #if HAS_HEATED_BED
    EDIT_ITEM(int3, MSG_UBL_BED_TEMP_CUSTOM, &custom_bed_temp, BED_MINTEMP, (BED_MAXTEMP - 10));
  #endif
  ACTION_ITEM(MSG_UBL_BUILD_CUSTOM_MESH, _lcd_ubl_build_custom_mesh);
  END_MENU();
}

/**
 * UBL Adjust Mesh Height Command
 */
void _lcd_ubl_adjust_height_cmd() {
  char ubl_lcd_gcode[16];
  const int ind = ubl_height_amount > 0 ? 9 : 10;
  strcpy_P(ubl_lcd_gcode, PSTR("G29 P6 C -"));
  sprintf_P(&ubl_lcd_gcode[ind], PSTR(".%i"), ABS(ubl_height_amount));
  lcd_enqueue_one_now(ubl_lcd_gcode);
}

/**
 * UBL Adjust Mesh Height submenu
 *
 * << Edit Mesh
 *    Height Amount: ---
 *    Adjust Mesh Height
 * << Info Screen
 */
void _menu_ubl_height_adjust() {
  START_MENU();
  BACK_ITEM(MSG_EDIT_MESH);
  EDIT_ITEM(int3, MSG_UBL_MESH_HEIGHT_AMOUNT, &ubl_height_amount, -9, 9, _lcd_ubl_adjust_height_cmd);
  ACTION_ITEM(MSG_INFO_SCREEN, ui.return_to_status);
  END_MENU();
}

/**
 * UBL Edit Mesh submenu
 *
 * << UBL Tools
 *    Fine Tune All
 *    Fine Tune Closest
 *  - Adjust Mesh Height >>
 * << Info Screen
 */
void _lcd_ubl_edit_mesh() {
  START_MENU();
  BACK_ITEM(MSG_UBL_TOOLS);
  GCODES_ITEM(MSG_UBL_FINE_TUNE_ALL, PSTR("G29 P4 R999 T"));
  GCODES_ITEM(MSG_UBL_FINE_TUNE_CLOSEST, PSTR("G29 P4 T"));
  SUBMENU(MSG_UBL_MESH_HEIGHT_ADJUST, _menu_ubl_height_adjust);
  ACTION_ITEM(MSG_INFO_SCREEN, ui.return_to_status);
  END_MENU();
}

/**
 * UBL Validate Custom Mesh Command
 */
void _lcd_ubl_validate_custom_mesh() {
  char ubl_lcd_gcode[24];
  const int temp =
    #if HAS_HEATED_BED
      custom_bed_temp
    #else
      0
    #endif
  ;
  sprintf_P(ubl_lcd_gcode, PSTR("G26 C B%i H%i P"), temp, custom_hotend_temp);
  lcd_enqueue_one_now_P(G28_STR);
  lcd_enqueue_one_now(ubl_lcd_gcode);
}

/**
 * UBL Validate Mesh submenu
 *
 * << UBL Tools
 *    Mesh Validation with Material 1
 *    Mesh Validation with Material 2
 *    Validate Custom Mesh
 * << Info Screen
 */
void _lcd_ubl_validate_mesh() {
  START_MENU();
  BACK_ITEM(MSG_UBL_TOOLS);
  #if HAS_HEATED_BED
    GCODES_ITEM(MSG_UBL_VALIDATE_MESH_M1, PSTR("G28\nG26 C B" STRINGIFY(PREHEAT_1_TEMP_BED) " H" STRINGIFY(PREHEAT_1_TEMP_HOTEND) " P"));
    GCODES_ITEM(MSG_UBL_VALIDATE_MESH_M2, PSTR("G28\nG26 C B" STRINGIFY(PREHEAT_2_TEMP_BED) " H" STRINGIFY(PREHEAT_2_TEMP_HOTEND) " P"));
  #else
    GCODES_ITEM(MSG_UBL_VALIDATE_MESH_M1, PSTR("G28\nG26 C B0 H" STRINGIFY(PREHEAT_1_TEMP_HOTEND) " P"));
    GCODES_ITEM(MSG_UBL_VALIDATE_MESH_M2, PSTR("G28\nG26 C B0 H" STRINGIFY(PREHEAT_2_TEMP_HOTEND) " P"));
  #endif
  ACTION_ITEM(MSG_UBL_VALIDATE_CUSTOM_MESH, _lcd_ubl_validate_custom_mesh);
  ACTION_ITEM(MSG_INFO_SCREEN, ui.return_to_status);
  END_MENU();
}

/**
 * UBL Grid Leveling submenu
 *
 * << UBL Tools
 *    Side points: ---
 *    Level Mesh
 */
void _lcd_ubl_grid_level() {
  START_MENU();
  BACK_ITEM(MSG_UBL_TOOLS);
  EDIT_ITEM(int3, MSG_UBL_SIDE_POINTS, &side_points, 2, 6);
  ACTION_ITEM(MSG_UBL_MESH_LEVEL, []{
    char ubl_lcd_gcode[12];
    sprintf_P(ubl_lcd_gcode, PSTR("G29 J%i"), side_points);
    lcd_enqueue_one_now(ubl_lcd_gcode);
  });
  END_MENU();
}

/**
 * UBL Mesh Leveling submenu
 *
 * << UBL Tools
 *    3-Point Mesh Leveling
 *  - Grid Mesh Leveling >>
 * << Info Screen
 */
void _lcd_ubl_mesh_leveling() {
  START_MENU();
  BACK_ITEM(MSG_UBL_TOOLS);
  GCODES_ITEM(MSG_UBL_3POINT_MESH_LEVELING, PSTR("G29 J0"));
  SUBMENU(MSG_UBL_GRID_MESH_LEVELING, _lcd_ubl_grid_level);
  ACTION_ITEM(MSG_INFO_SCREEN, ui.return_to_status);
  END_MENU();
}

/**
 * UBL Fill-in Amount Mesh Command
 */
void _lcd_ubl_fillin_amount_cmd() {
  char ubl_lcd_gcode[18];
  sprintf_P(ubl_lcd_gcode, PSTR("G29 P3 R C.%i"), ubl_fillin_amount);
  lcd_enqueue_one_now(ubl_lcd_gcode);
}

/**
 * UBL Fill-in Mesh submenu
 *
 * << Build Mesh
 *    Fill-in Amount: ---
 *    Fill-in Mesh
 *    Smart Fill-in
 *    Manual Fill-in
 * << Info Screen
 */
void _menu_ubl_fillin() {
  START_MENU();
  BACK_ITEM(MSG_UBL_BUILD_MESH_MENU);
  EDIT_ITEM(int3, MSG_UBL_FILLIN_AMOUNT, &ubl_fillin_amount, 0, 9, _lcd_ubl_fillin_amount_cmd);
  GCODES_ITEM(MSG_UBL_SMART_FILLIN, PSTR("G29 P3 T0"));
  GCODES_ITEM(MSG_UBL_MANUAL_FILLIN, PSTR("G29 P2 B T0"));
  ACTION_ITEM(MSG_INFO_SCREEN, ui.return_to_status);
  END_MENU();
}

void _lcd_ubl_invalidate() {
  ubl.invalidate();
  SERIAL_ECHOLNPGM("Mesh invalidated.");
}

/**
 * UBL Build Mesh submenu
 *
 * << UBL Tools
 *    Build Mesh with Material 1
 *    Build Mesh with Material 2
 *  - Build Custom Mesh >>
 *    Build Cold Mesh
 *  - Fill-in Mesh >>
 *    Continue Bed Mesh
 *    Invalidate All
 *    Invalidate Closest
 * << Info Screen
 */
void _lcd_ubl_build_mesh() {
  START_MENU();
  BACK_ITEM(MSG_UBL_TOOLS);
  #if HAS_HEATED_BED
    GCODES_ITEM(MSG_UBL_BUILD_MESH_M1, PSTR(
      "G28\n"
      "M190 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\n"
      "M109 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND) "\n"
      "G29 P1\n"
      "M104 S0\n"
      "M140 S0"
    ));
    GCODES_ITEM(MSG_UBL_BUILD_MESH_M2, PSTR(
      "G28\n"
      "M190 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\n"
      "M109 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND) "\n"
      "G29 P1\n"
      "M104 S0\n"
      "M140 S0"
    ));
  #else
    GCODES_ITEM(MSG_UBL_BUILD_MESH_M1, PSTR(
      "G28\n"
      "M109 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND) "\n"
      "G29 P1\n"
      "M104 S0"
    ));
    GCODES_ITEM(MSG_UBL_BUILD_MESH_M2, PSTR(
      "G28\n"
      "M109 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND) "\n"
      "G29 P1\n"
      "M104 S0"
    ));
  #endif
  SUBMENU(MSG_UBL_BUILD_CUSTOM_MESH, _lcd_ubl_custom_mesh);
  GCODES_ITEM(MSG_UBL_BUILD_COLD_MESH, PSTR("G28\nG29 P1"));
  SUBMENU(MSG_UBL_FILLIN_MESH, _menu_ubl_fillin);
  GCODES_ITEM(MSG_UBL_CONTINUE_MESH, PSTR("G29 P1 C"));
  ACTION_ITEM(MSG_UBL_INVALIDATE_ALL, _lcd_ubl_invalidate);
  GCODES_ITEM(MSG_UBL_INVALIDATE_CLOSEST, PSTR("G29 I"));
  ACTION_ITEM(MSG_INFO_SCREEN, ui.return_to_status);
  END_MENU();
}

/**
 * UBL Load Mesh Command
 */
void _lcd_ubl_load_mesh_cmd() {
  char ubl_lcd_gcode[25];
  sprintf_P(ubl_lcd_gcode, PSTR("G29 L%i"), ubl_storage_slot);
  lcd_enqueue_one_now(ubl_lcd_gcode);
  sprintf_P(ubl_lcd_gcode, GET_TEXT(MSG_MESH_LOADED), ubl_storage_slot);
  lcd_enqueue_one_now(ubl_lcd_gcode);
}

/**
 * UBL Save Mesh Command
 */
void _lcd_ubl_save_mesh_cmd() {
  char ubl_lcd_gcode[25];
  sprintf_P(ubl_lcd_gcode, PSTR("G29 S%i"), ubl_storage_slot);
  lcd_enqueue_one_now(ubl_lcd_gcode);
  sprintf_P(ubl_lcd_gcode, GET_TEXT(MSG_MESH_SAVED), ubl_storage_slot);
  lcd_enqueue_one_now(ubl_lcd_gcode);
}

/**
 * UBL Mesh Storage submenu
 *
 * << Unified Bed Leveling
 *    Memory Slot: ---
 *    Load Bed Mesh
 *    Save Bed Mesh
 */
void _lcd_ubl_storage_mesh() {
  int16_t a = settings.calc_num_meshes();
  START_MENU();
  BACK_ITEM(MSG_UBL_LEVEL_BED);
  if (!WITHIN(ubl_storage_slot, 0, a - 1)) {
    STATIC_ITEM(MSG_UBL_NO_STORAGE);
  }
  else {
    EDIT_ITEM(int3, MSG_UBL_STORAGE_SLOT, &ubl_storage_slot, 0, a - 1);
    ACTION_ITEM(MSG_UBL_LOAD_MESH, _lcd_ubl_load_mesh_cmd);
    ACTION_ITEM(MSG_UBL_SAVE_MESH, _lcd_ubl_save_mesh_cmd);
  }
  END_MENU();
}

/**
 * UBL LCD "radar" map homing
 */
void _lcd_ubl_output_map_lcd();

void _lcd_ubl_map_homing() {
  ui.defer_status_screen();
  _lcd_draw_homing();
  if (all_axes_homed()) {
    ubl.lcd_map_control = true; // Return to the map screen
    ui.goto_screen(_lcd_ubl_output_map_lcd);
  }
}

/**
 * UBL LCD "radar" map point editing
 */
void _lcd_ubl_map_lcd_edit_cmd() {
  char ubl_lcd_gcode[50], str[10], str2[10];
  dtostrf(ubl.mesh_index_to_xpos(x_plot), 0, 2, str);
  dtostrf(ubl.mesh_index_to_ypos(y_plot), 0, 2, str2);
  snprintf_P(ubl_lcd_gcode, sizeof(ubl_lcd_gcode), PSTR("G29 P4 X%s Y%s R%i"), str, str2, int(n_edit_pts));
  lcd_enqueue_one_now(ubl_lcd_gcode);
}

/**
 * UBL LCD Map Movement
 */
void ubl_map_move_to_xy() {
  const feedRate_t fr_mm_s = MMM_TO_MMS(XY_PROBE_SPEED);

  destination = current_position;          // sync destination at the start

  #if ENABLED(DELTA)
    if (current_position.z > delta_clip_start_height) {
      destination.z = delta_clip_start_height;
      prepare_internal_move_to_destination(fr_mm_s);
    }
  #endif

  destination.set(ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot));
  prepare_internal_move_to_destination(fr_mm_s);
}

/**
 * UBL LCD "radar" map
 */
void set_current_from_steppers_for_axis(const AxisEnum axis);
void sync_plan_position();

void _lcd_do_nothing() {}
void _lcd_hard_stop() {
  const screenFunc_t old_screen = ui.currentScreen;
  ui.currentScreen = _lcd_do_nothing;
  planner.quick_stop();
  ui.currentScreen = old_screen;
  set_current_from_steppers_for_axis(ALL_AXES);
  sync_plan_position();
}

void _lcd_ubl_output_map_lcd() {
  static int16_t step_scaler = 0;

  if (ui.use_click()) return _lcd_ubl_map_lcd_edit_cmd();

  if (ui.encoderPosition) {
    step_scaler += int32_t(ui.encoderPosition);
    x_plot += step_scaler / (ENCODER_STEPS_PER_MENU_ITEM);
    ui.encoderPosition = 0;
    ui.refresh(LCDVIEW_REDRAW_NOW);
  }

  #if IS_KINEMATIC
    #define KEEP_LOOPING true   // Loop until a valid point is found
  #else
    #define KEEP_LOOPING false
  #endif

  do {
    // Encoder to the right (++)
    if (x_plot >= GRID_MAX_POINTS_X) { x_plot = 0; y_plot++; }
    if (y_plot >= GRID_MAX_POINTS_Y) y_plot = 0;

    // Encoder to the left (--)
    if (x_plot < 0) { x_plot = GRID_MAX_POINTS_X - 1; y_plot--; }
    if (y_plot < 0) y_plot = GRID_MAX_POINTS_Y - 1;

    #if IS_KINEMATIC
      const xy_pos_t xy = { ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot) };
      if (position_is_reachable(xy)) break; // Found a valid point
      x_plot += (step_scaler < 0) ? -1 : 1;
    #endif

  } while(KEEP_LOOPING);

  // Determine number of points to edit
  #if IS_KINEMATIC
    n_edit_pts = 9; //TODO: Delta accessible edit points
  #else
    const bool xc = WITHIN(x_plot, 1, GRID_MAX_POINTS_X - 2),
               yc = WITHIN(y_plot, 1, GRID_MAX_POINTS_Y - 2);
    n_edit_pts = yc ? (xc ? 9 : 6) : (xc ? 6 : 4); // Corners
  #endif

  // Cleanup
  if (ABS(step_scaler) >= ENCODER_STEPS_PER_MENU_ITEM) step_scaler = 0;

  if (ui.should_draw()) {
    ui.ubl_plot(x_plot, y_plot);

    if (planner.movesplanned()) // If the nozzle is already moving, cancel the move.
      _lcd_hard_stop();

    ubl_map_move_to_xy();       // Move to new location
  }
}

/**
 * UBL Homing before LCD map
 */
void _lcd_ubl_output_map_lcd_cmd() {
  if (!all_axes_known()) {
    set_all_unhomed();
    queue.inject_P(G28_STR);
  }
  ui.goto_screen(_lcd_ubl_map_homing);
}

/**
 * UBL Output map submenu
 *
 * << Unified Bed Leveling
 *  Output for Host
 *  Output for CSV
 *  Off Printer Backup
 */
void _lcd_ubl_output_map() {
  START_MENU();
  BACK_ITEM(MSG_UBL_LEVEL_BED);
  GCODES_ITEM(MSG_UBL_OUTPUT_MAP_HOST, PSTR("G29 T0"));
  GCODES_ITEM(MSG_UBL_OUTPUT_MAP_CSV, PSTR("G29 T1"));
  GCODES_ITEM(MSG_UBL_OUTPUT_MAP_BACKUP, PSTR("G29 S-1"));
  END_MENU();
}

/**
 * UBL Tools submenu
 *
 * << Unified Bed Leveling
 *  - Build Mesh >>
 *  - Validate Mesh >>
 *  - Edit Mesh >>
 *  - Mesh Leveling >>
 */
void _menu_ubl_tools() {
  START_MENU();
  BACK_ITEM(MSG_UBL_LEVEL_BED);
  SUBMENU(MSG_UBL_BUILD_MESH_MENU, _lcd_ubl_build_mesh);
  GCODES_ITEM(MSG_UBL_MANUAL_MESH, PSTR("G29 I999\nG29 P2 B T0"));
  SUBMENU(MSG_UBL_VALIDATE_MESH_MENU, _lcd_ubl_validate_mesh);
  SUBMENU(MSG_EDIT_MESH, _lcd_ubl_edit_mesh);
  SUBMENU(MSG_UBL_MESH_LEVELING, _lcd_ubl_mesh_leveling);
  END_MENU();
}

/**
 * UBL Step-By-Step submenu
 *
 * << Unified Bed Leveling
 *    1 Build Cold Mesh
 *    2 Smart Fill-in
 *  - 3 Validate Mesh >>
 *    4 Fine Tune All
 *  - 5 Validate Mesh >>
 *    6 Fine Tune All
 *    7 Save Bed Mesh
 */
void _lcd_ubl_step_by_step() {
  START_MENU();
  BACK_ITEM(MSG_UBL_LEVEL_BED);
  GCODES_ITEM(MSG_UBL_1_BUILD_COLD_MESH, PSTR("G28\nG29 P1"));
  GCODES_ITEM(MSG_UBL_2_SMART_FILLIN, PSTR("G29 P3 T0"));
  SUBMENU(MSG_UBL_3_VALIDATE_MESH_MENU, _lcd_ubl_validate_mesh);
  GCODES_ITEM(MSG_UBL_4_FINE_TUNE_ALL, PSTR("G29 P4 R999 T"));
  SUBMENU(MSG_UBL_5_VALIDATE_MESH_MENU, _lcd_ubl_validate_mesh);
  GCODES_ITEM(MSG_UBL_6_FINE_TUNE_ALL, PSTR("G29 P4 R999 T"));
  ACTION_ITEM(MSG_UBL_7_SAVE_MESH, _lcd_ubl_save_mesh_cmd);
  END_MENU();
}

/**
 * UBL System submenu
 *
 * << Motion
 *  - Manually Build Mesh >>
 *  - Activate UBL >>
 *  - Deactivate UBL >>
 *  - Step-By-Step UBL >>
 *  - Mesh Storage >>
 *  - Output Map >>
 *  - UBL Tools >>
 *  - Output UBL Info >>
 */

void _lcd_ubl_level_bed() {
  START_MENU();
  BACK_ITEM(MSG_MOTION);
  if (planner.leveling_active)
    GCODES_ITEM(MSG_UBL_DEACTIVATE_MESH, PSTR("G29 D"));
  else
    GCODES_ITEM(MSG_UBL_ACTIVATE_MESH, PSTR("G29 A"));
  SUBMENU(MSG_UBL_STEP_BY_STEP_MENU, _lcd_ubl_step_by_step);
  ACTION_ITEM(MSG_UBL_MESH_EDIT, _lcd_ubl_output_map_lcd_cmd);
  SUBMENU(MSG_UBL_STORAGE_MESH_MENU, _lcd_ubl_storage_mesh);
  SUBMENU(MSG_UBL_OUTPUT_MAP, _lcd_ubl_output_map);
  SUBMENU(MSG_UBL_TOOLS, _menu_ubl_tools);
  GCODES_ITEM(MSG_UBL_INFO_UBL, PSTR("G29 W"));
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    editable.decimal = planner.z_fade_height;
    EDIT_ITEM_FAST(float3, MSG_Z_FADE_HEIGHT, &editable.decimal, 0, 100, []{ set_z_fade_height(editable.decimal); });
  #endif
  END_MENU();
}

#endif // HAS_LCD_MENU && AUTO_BED_LEVELING_UBL
