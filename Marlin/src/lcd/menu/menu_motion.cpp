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
// Motion Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu_item.h"
#include "menu_addon.h"

#include "../../module/motion.h"

#if ENABLED(DELTA)
  #include "../../module/delta.h"
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  #include "../../module/temperature.h"
#endif

#if HAS_LEVELING
  #include "../../module/planner.h"
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(MANUAL_E_MOVES_RELATIVE)
  float manual_move_e_origin = 0;
#endif

//
// "Motion" > "Move Axis" submenu
//

static void _lcd_move_xyz(PGM_P const name, const AxisEnum axis) {
  if (ui.use_click()) return ui.goto_previous_screen_no_defer();
  if (ui.encoderPosition && !ui.manual_move.processing) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits(axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS) {
        max = SQRT(sq((float)(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const float diff = float(int32_t(ui.encoderPosition)) * ui.manual_move.menu_scale;
    #if IS_KINEMATIC
      ui.manual_move.offset += diff;
      if (int32_t(ui.encoderPosition) < 0)
        NOLESS(ui.manual_move.offset, min - current_position[axis]);
      else
        NOMORE(ui.manual_move.offset, max - current_position[axis]);
    #else
      current_position[axis] += diff;
      if (int32_t(ui.encoderPosition) < 0)
        NOLESS(current_position[axis], min);
      else
        NOMORE(current_position[axis], max);
    #endif

    ui.manual_move.soon(axis);
    ui.refresh(LCDVIEW_REDRAW_NOW);
  }
  ui.encoderPosition = 0;
  if (ui.should_draw()) {
    const float pos = NATIVE_TO_LOGICAL(
      ui.manual_move.processing ? destination[axis] : current_position[axis] + TERN0(IS_KINEMATIC, ui.manual_move.offset),
      axis
    );
    MenuEditItemBase::draw_edit_screen(name, ui.manual_move.menu_scale >= 0.1f ? ftostr41sign(pos) : ftostr63(pos));
  }
}
void lcd_move_x() { _lcd_move_xyz(GET_TEXT(MSG_MOVE_X), X_AXIS); }
void lcd_move_y() { _lcd_move_xyz(GET_TEXT(MSG_MOVE_Y), Y_AXIS); }
void lcd_move_z() { _lcd_move_xyz(GET_TEXT(MSG_MOVE_Z), Z_AXIS); }

#if E_MANUAL

  static void lcd_move_e(TERN_(MULTI_MANUAL, const int8_t eindex=-1)) {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    if (ui.encoderPosition) {
      if (!ui.manual_move.processing) {
        const float diff = float(int32_t(ui.encoderPosition)) * ui.manual_move.menu_scale;
        TERN(IS_KINEMATIC, ui.manual_move.offset, current_position.e) += diff;
        ui.manual_move.soon(E_AXIS
          #if MULTI_MANUAL
            , eindex
          #endif
        );
        ui.refresh(LCDVIEW_REDRAW_NOW);
      }
      ui.encoderPosition = 0;
    }
    if (ui.should_draw()) {
      TERN_(MULTI_MANUAL, MenuItemBase::init(eindex));
      MenuEditItemBase::draw_edit_screen(
        GET_TEXT(TERN(MULTI_MANUAL, MSG_MOVE_EN, MSG_MOVE_E)),
        ftostr41sign(current_position.e
          + TERN0(IS_KINEMATIC, ui.manual_move.offset)
          - TERN0(MANUAL_E_MOVES_RELATIVE, manual_move_e_origin)
        )
      );
    } // should_draw
  }

#endif // E_MANUAL

//
// "Motion" > "Move Xmm" > "Move XYZ" submenu
//

#ifndef SHORT_MANUAL_Z_MOVE
  #define SHORT_MANUAL_Z_MOVE 0.025
#endif

screenFunc_t _manual_move_func_ptr;

void _goto_manual_move(const float scale) {
  ui.defer_status_screen();
  ui.manual_move.menu_scale = scale;
  ui.goto_screen(_manual_move_func_ptr);
}

void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=-1) {
  _manual_move_func_ptr = func;
  START_MENU();
  if (LCD_HEIGHT >= 4) {
    switch (axis) {
      case X_AXIS: STATIC_ITEM(MSG_MOVE_X, SS_DEFAULT|SS_INVERT); break;
      case Y_AXIS: STATIC_ITEM(MSG_MOVE_Y, SS_DEFAULT|SS_INVERT); break;
      case Z_AXIS: STATIC_ITEM(MSG_MOVE_Z, SS_DEFAULT|SS_INVERT); break;
      default:
        TERN_(MANUAL_E_MOVES_RELATIVE, manual_move_e_origin = current_position.e);
        STATIC_ITEM(MSG_MOVE_E, SS_DEFAULT|SS_INVERT);
        break;
    }
  }

  BACK_ITEM(MSG_MOVE_AXIS);
  SUBMENU(MSG_MOVE_10MM, []{ _goto_manual_move(10);    });
  SUBMENU(MSG_MOVE_1MM,  []{ _goto_manual_move( 1);    });
  SUBMENU(MSG_MOVE_01MM, []{ _goto_manual_move( 0.1f); });
  if (axis == Z_AXIS && (SHORT_MANUAL_Z_MOVE) > 0.0f && (SHORT_MANUAL_Z_MOVE) < 0.1f) {
    // Determine digits needed right of decimal
    constexpr uint8_t digs = !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) * 1000 - int((SHORT_MANUAL_Z_MOVE) * 1000)) ? 4 :
                              !UNEAR_ZERO((SHORT_MANUAL_Z_MOVE) *  100 - int((SHORT_MANUAL_Z_MOVE) *  100)) ? 3 : 2;
    PGM_P const label = GET_TEXT(MSG_MOVE_Z_DIST);
    char tmp[strlen_P(label) + 10 + 1], numstr[10];
    sprintf_P(tmp, label, dtostrf(SHORT_MANUAL_Z_MOVE, 1, digs, numstr));

    #if DISABLED(HAS_GRAPHICAL_TFT)
      extern const char NUL_STR[];
      SUBMENU_P(NUL_STR, []{ _goto_manual_move(float(SHORT_MANUAL_Z_MOVE)); });
      MENU_ITEM_ADDON_START(0 + ENABLED(HAS_MARLINUI_HD44780));
      lcd_put_u8str(tmp);
      MENU_ITEM_ADDON_END();
    #else
      SUBMENU_P(tmp, []{ _goto_manual_move(float(SHORT_MANUAL_Z_MOVE)); });
    #endif
  }
  END_MENU();
}

#if E_MANUAL

  inline void _goto_menu_move_distance_e() {
    ui.goto_screen([]{ _menu_move_distance(E_AXIS, []{ lcd_move_e(); }, -1); });
  }

  inline void _menu_move_distance_e_maybe() {
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      const bool too_cold = thermalManager.tooColdToExtrude(active_extruder);
      if (too_cold) {
        ui.goto_screen([]{
          MenuItem_confirm::select_screen(
            GET_TEXT(MSG_BUTTON_PROCEED), GET_TEXT(MSG_BACK),
            _goto_menu_move_distance_e, ui.goto_previous_screen,
            GET_TEXT(MSG_HOTEND_TOO_COLD), (const char *)nullptr, PSTR("!")
          );
        });
        return;
      }
    #endif
    _goto_menu_move_distance_e();
  }

#endif // E_MANUAL

void menu_move() {
  START_MENU();
  BACK_ITEM(MSG_MOTION);

  #if BOTH(HAS_SOFTWARE_ENDSTOPS, SOFT_ENDSTOPS_MENU_ITEM)
    EDIT_ITEM(bool, MSG_LCD_SOFT_ENDSTOPS, &soft_endstop._enabled);
  #endif

  if (NONE(IS_KINEMATIC, NO_MOTION_BEFORE_HOMING) || all_axes_homed()) {
    if (TERN1(DELTA, current_position.z <= delta_clip_start_height)) {
      SUBMENU(MSG_MOVE_X, []{ _menu_move_distance(X_AXIS, lcd_move_x); });
      SUBMENU(MSG_MOVE_Y, []{ _menu_move_distance(Y_AXIS, lcd_move_y); });
    }
    #if ENABLED(DELTA)
      else
        ACTION_ITEM(MSG_FREE_XY, []{ line_to_z(delta_clip_start_height); ui.synchronize(); });
    #endif

    SUBMENU(MSG_MOVE_Z, []{ _menu_move_distance(Z_AXIS, lcd_move_z); });
  }
  else
    GCODES_ITEM(MSG_AUTO_HOME, G28_STR);

  #if ANY(SWITCHING_EXTRUDER, SWITCHING_NOZZLE, MAGNETIC_SWITCHING_TOOLHEAD)

    #if EXTRUDERS >= 4
      switch (active_extruder) {
        case 0: GCODES_ITEM_N(1, MSG_SELECT_E, PSTR("T1")); break;
        case 1: GCODES_ITEM_N(0, MSG_SELECT_E, PSTR("T0")); break;
        case 2: GCODES_ITEM_N(3, MSG_SELECT_E, PSTR("T3")); break;
        case 3: GCODES_ITEM_N(2, MSG_SELECT_E, PSTR("T2")); break;
        #if EXTRUDERS == 6
          case 4: GCODES_ITEM_N(5, MSG_SELECT_E, PSTR("T5")); break;
          case 5: GCODES_ITEM_N(4, MSG_SELECT_E, PSTR("T4")); break;
        #endif
      }
    #elif EXTRUDERS == 3
      if (active_extruder < 2) {
        if (active_extruder)
          GCODES_ITEM_N(0, MSG_SELECT_E, PSTR("T0"));
        else
          GCODES_ITEM_N(1, MSG_SELECT_E, PSTR("T1"));
      }
    #else
      if (active_extruder)
        GCODES_ITEM_N(0, MSG_SELECT_E, PSTR("T0"));
      else
        GCODES_ITEM_N(1, MSG_SELECT_E, PSTR("T1"));
    #endif

  #elif ENABLED(DUAL_X_CARRIAGE)

    if (active_extruder)
      GCODES_ITEM_N(0, MSG_SELECT_E, PSTR("T0"));
    else
      GCODES_ITEM_N(1, MSG_SELECT_E, PSTR("T1"));

  #endif

  #if E_MANUAL

    // The current extruder
    SUBMENU(MSG_MOVE_E, []{ _menu_move_distance_e_maybe(); });

    #define SUBMENU_MOVE_E(N) SUBMENU_N(N, MSG_MOVE_EN, []{ _menu_move_distance(E_AXIS, []{ lcd_move_e(MenuItemBase::itemIndex); }, MenuItemBase::itemIndex); });

    #if EITHER(SWITCHING_EXTRUDER, SWITCHING_NOZZLE)

      // ...and the non-switching
      #if E_MANUAL == 7 || E_MANUAL == 5 || E_MANUAL == 3
        SUBMENU_MOVE_E(E_MANUAL - 1);
      #endif

    #elif MULTI_MANUAL

      // Independent extruders with one E-stepper per hotend
      LOOP_L_N(n, E_MANUAL) SUBMENU_MOVE_E(n);

    #endif

  #endif // E_MANUAL

  END_MENU();
}

#if ENABLED(AUTO_BED_LEVELING_UBL)
  void _lcd_ubl_level_bed();
#elif ENABLED(LCD_BED_LEVELING)
  void menu_bed_leveling();
#endif

void menu_motion() {
  START_MENU();

  //
  // ^ Main
  //
  BACK_ITEM(MSG_MAIN);

  //
  // Move Axis
  //
  if (TERN1(DELTA, all_axes_homed()))
    SUBMENU(MSG_MOVE_AXIS, menu_move);

  //
  // Auto Home
  //
  GCODES_ITEM(MSG_AUTO_HOME, G28_STR);
  #if ENABLED(INDIVIDUAL_AXIS_HOMING_MENU)
    GCODES_ITEM(MSG_AUTO_HOME_X, PSTR("G28X"));
    GCODES_ITEM(MSG_AUTO_HOME_Y, PSTR("G28Y"));
    GCODES_ITEM(MSG_AUTO_HOME_Z, PSTR("G28Z"));
  #endif

  //
  // Auto-calibration
  //
  #if ENABLED(CALIBRATION_GCODE)
    GCODES_ITEM(MSG_AUTO_CALIBRATE, PSTR("G425"));
  #endif

  //
  // Auto Z-Align
  //
  #if EITHER(Z_STEPPER_AUTO_ALIGN, MECHANICAL_GANTRY_CALIBRATION)
    GCODES_ITEM(MSG_AUTO_Z_ALIGN, PSTR("G34"));
  #endif

  //
  // Assisted Bed Tramming
  //
  #if ENABLED(ASSISTED_TRAMMING_MENU_ITEM)
    GCODES_ITEM(MSG_ASSISTED_TRAMMING, PSTR("G35"));
  #endif

  //
  // Level Bed
  //
  #if ENABLED(AUTO_BED_LEVELING_UBL)

    SUBMENU(MSG_UBL_LEVEL_BED, _lcd_ubl_level_bed);

  #elif ENABLED(LCD_BED_LEVELING)

    if (!g29_in_progress)
      SUBMENU(MSG_BED_LEVELING, menu_bed_leveling);

  #elif HAS_LEVELING && DISABLED(SLIM_LCD_MENUS)

    #if DISABLED(PROBE_MANUALLY)
      GCODES_ITEM(MSG_LEVEL_BED, PSTR("G28\nG29"));
    #endif

    if (all_axes_homed() && leveling_is_valid()) {
      bool show_state = planner.leveling_active;
      EDIT_ITEM(bool, MSG_BED_LEVELING, &show_state, _lcd_toggle_bed_leveling);
    }

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      editable.decimal = planner.z_fade_height;
      EDIT_ITEM_FAST(float3, MSG_Z_FADE_HEIGHT, &editable.decimal, 0, 100, []{ set_z_fade_height(editable.decimal); });
    #endif

  #endif

  #if ENABLED(LEVEL_BED_CORNERS) && DISABLED(LCD_BED_LEVELING)
    ACTION_ITEM(MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    GCODES_ITEM(MSG_M48_TEST, PSTR("G28 O\nM48 P10"));
  #endif

  //
  // Disable Steppers
  //
  GCODES_ITEM(MSG_DISABLE_STEPPERS, PSTR("M84"));

  END_MENU();
}

#endif // HAS_LCD_MENU
