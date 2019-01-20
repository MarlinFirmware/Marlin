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
// Motion Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
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

extern millis_t manual_move_start_time;
extern int8_t manual_move_axis;
#if ENABLED(MANUAL_E_MOVES_RELATIVE)
  float manual_move_e_origin = 0;
#endif
#if IS_KINEMATIC
  extern float manual_move_offset;
#endif

//
// Tell ui.update() to start a move to current_position" after a short delay.
//
inline void manual_move_to_current(AxisEnum axis
  #if E_MANUAL > 1
    , const int8_t eindex=-1
  #endif
) {
  #if E_MANUAL > 1
    if (axis == E_AXIS) ui.manual_move_e_index = eindex >= 0 ? eindex : active_extruder;
  #endif
  manual_move_start_time = millis() + (move_menu_scale < 0.99f ? 0UL : 250UL); // delay for bigger moves
  manual_move_axis = (int8_t)axis;
}

//
// "Motion" > "Move Axis" submenu
//

static void _lcd_move_xyz(PGM_P name, AxisEnum axis) {
  if (ui.use_click()) return ui.goto_previous_screen_no_defer();
  ui.encoder_direction_normal();
  if (ui.encoderPosition && !ui.processing_manual_move) {

    // Start with no limits to movement
    float min = current_position[axis] - 1000,
          max = current_position[axis] + 1000;

    // Limit to software endstops, if enabled
    #if HAS_SOFTWARE_ENDSTOPS
      if (soft_endstops_enabled) switch (axis) {
        case X_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_X)
            min = soft_endstop_min[X_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_X)
            max = soft_endstop_max[X_AXIS];
          #endif
          break;
        case Y_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_Y)
            min = soft_endstop_min[Y_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_Y)
            max = soft_endstop_max[Y_AXIS];
          #endif
          break;
        case Z_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_Z)
            min = soft_endstop_min[Z_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_Z)
            max = soft_endstop_max[Z_AXIS];
          #endif
        default: break;
      }
    #endif // HAS_SOFTWARE_ENDSTOPS

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS) {
        max = SQRT(sq((float)(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const float diff = float((int32_t)ui.encoderPosition) * move_menu_scale;
    #if IS_KINEMATIC
      manual_move_offset += diff;
      if ((int32_t)ui.encoderPosition < 0)
        NOLESS(manual_move_offset, min - current_position[axis]);
      else
        NOMORE(manual_move_offset, max - current_position[axis]);
    #else
      current_position[axis] += diff;
      if ((int32_t)ui.encoderPosition < 0)
        NOLESS(current_position[axis], min);
      else
        NOMORE(current_position[axis], max);
    #endif

    manual_move_to_current(axis);
    ui.refresh(LCDVIEW_REDRAW_NOW);
  }
  ui.encoderPosition = 0;
  if (ui.should_draw()) {
    const float pos = NATIVE_TO_LOGICAL(ui.processing_manual_move ? destination[axis] : current_position[axis]
      #if IS_KINEMATIC
        + manual_move_offset
      #endif
    , axis);
    draw_edit_screen(name, move_menu_scale >= 0.1f ? ftostr41sign(pos) : ftostr43sign(pos));
  }
}
void lcd_move_x() { _lcd_move_xyz(PSTR(MSG_MOVE_X), X_AXIS); }
void lcd_move_y() { _lcd_move_xyz(PSTR(MSG_MOVE_Y), Y_AXIS); }
void lcd_move_z() { _lcd_move_xyz(PSTR(MSG_MOVE_Z), Z_AXIS); }
static void _lcd_move_e(
  #if E_MANUAL > 1
    const int8_t eindex=-1
  #endif
) {
  if (ui.use_click()) return ui.goto_previous_screen_no_defer();
  ui.encoder_direction_normal();
  if (ui.encoderPosition) {
    if (!ui.processing_manual_move) {
      const float diff = float((int32_t)ui.encoderPosition) * move_menu_scale;
      #if IS_KINEMATIC
        manual_move_offset += diff;
      #else
        current_position[E_AXIS] += diff;
      #endif
      manual_move_to_current(E_AXIS
        #if E_MANUAL > 1
          , eindex
        #endif
      );
      ui.refresh(LCDVIEW_REDRAW_NOW);
    }
    ui.encoderPosition = 0;
  }
  if (ui.should_draw()) {
    PGM_P pos_label;
    #if E_MANUAL == 1
      pos_label = PSTR(MSG_MOVE_E);
    #else
      switch (eindex) {
        default: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E1); break;
        case 1: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E2); break;
        #if E_MANUAL > 2
          case 2: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E3); break;
          #if E_MANUAL > 3
            case 3: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E4); break;
            #if E_MANUAL > 4
              case 4: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E5); break;
              #if E_MANUAL > 5
                case 5: pos_label = PSTR(MSG_MOVE_E MSG_MOVE_E6); break;
              #endif // E_MANUAL > 5
            #endif // E_MANUAL > 4
          #endif // E_MANUAL > 3
        #endif // E_MANUAL > 2
      }
    #endif // E_MANUAL > 1

    draw_edit_screen(pos_label, ftostr41sign(current_position[E_AXIS]
      #if IS_KINEMATIC
        + manual_move_offset
      #endif
      #if ENABLED(MANUAL_E_MOVES_RELATIVE)
        - manual_move_e_origin
      #endif
    ));
  }
}

inline void lcd_move_e() { _lcd_move_e(); }
#if E_MANUAL > 1
  inline void lcd_move_e0() { _lcd_move_e(0); }
  inline void lcd_move_e1() { _lcd_move_e(1); }
  #if E_MANUAL > 2
    inline void lcd_move_e2() { _lcd_move_e(2); }
    #if E_MANUAL > 3
      inline void lcd_move_e3() { _lcd_move_e(3); }
      #if E_MANUAL > 4
        inline void lcd_move_e4() { _lcd_move_e(4); }
        #if E_MANUAL > 5
          inline void lcd_move_e5() { _lcd_move_e(5); }
        #endif // E_MANUAL > 5
      #endif // E_MANUAL > 4
    #endif // E_MANUAL > 3
  #endif // E_MANUAL > 2
#endif // E_MANUAL > 1

//
// "Motion" > "Move Xmm" > "Move XYZ" submenu
//

screenFunc_t _manual_move_func_ptr;

void _goto_manual_move(const float scale) {
  ui.defer_status_screen(true);
  move_menu_scale = scale;
  ui.goto_screen(_manual_move_func_ptr);
}
void menu_move_10mm() { _goto_manual_move(10); }
void menu_move_1mm()  { _goto_manual_move( 1); }
void menu_move_01mm() { _goto_manual_move( 0.1f); }

void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=-1) {
  _manual_move_func_ptr = func;
  START_MENU();
  if (LCD_HEIGHT >= 4) {
    switch (axis) {
      case X_AXIS: STATIC_ITEM(MSG_MOVE_X, true, true); break;
      case Y_AXIS: STATIC_ITEM(MSG_MOVE_Y, true, true); break;
      case Z_AXIS: STATIC_ITEM(MSG_MOVE_Z, true, true); break;
      default:
        #if ENABLED(MANUAL_E_MOVES_RELATIVE)
          manual_move_e_origin = current_position[E_AXIS];
        #endif
        STATIC_ITEM(MSG_MOVE_E, true, true);
        break;
    }
  }
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(eindex >= 0 ? eindex : active_extruder))
      MENU_BACK(MSG_HOTEND_TOO_COLD);
    else
  #endif
  {
    MENU_BACK(MSG_MOVE_AXIS);
    MENU_ITEM(submenu, MSG_MOVE_10MM, menu_move_10mm);
    MENU_ITEM(submenu, MSG_MOVE_1MM, menu_move_1mm);
    MENU_ITEM(submenu, MSG_MOVE_01MM, menu_move_01mm);
  }
  END_MENU();
}
void lcd_move_get_x_amount() { _menu_move_distance(X_AXIS, lcd_move_x); }
void lcd_move_get_y_amount() { _menu_move_distance(Y_AXIS, lcd_move_y); }
void lcd_move_get_z_amount() { _menu_move_distance(Z_AXIS, lcd_move_z); }
void lcd_move_get_e_amount() { _menu_move_distance(E_AXIS, lcd_move_e, -1); }
#if E_MANUAL > 1
  void lcd_move_get_e0_amount()     { _menu_move_distance(E_AXIS, lcd_move_e0, 0); }
  void lcd_move_get_e1_amount()     { _menu_move_distance(E_AXIS, lcd_move_e1, 1); }
  #if E_MANUAL > 2
    void lcd_move_get_e2_amount()   { _menu_move_distance(E_AXIS, lcd_move_e2, 2); }
    #if E_MANUAL > 3
      void lcd_move_get_e3_amount() { _menu_move_distance(E_AXIS, lcd_move_e3, 3); }
      #if E_MANUAL > 4
        void lcd_move_get_e4_amount() { _menu_move_distance(E_AXIS, lcd_move_e4, 4); }
        #if E_MANUAL > 5
          void lcd_move_get_e5_amount() { _menu_move_distance(E_AXIS, lcd_move_e5, 5); }
        #endif // E_MANUAL > 5
      #endif // E_MANUAL > 4
    #endif // E_MANUAL > 3
  #endif // E_MANUAL > 2
#endif // E_MANUAL > 1

#if ENABLED(DELTA)
  void lcd_lower_z_to_clip_height() {
    line_to_z(delta_clip_start_height);
    ui.synchronize();
  }
#endif

void menu_move() {
  START_MENU();
  MENU_BACK(MSG_MOTION);

  #if HAS_SOFTWARE_ENDSTOPS && ENABLED(SOFT_ENDSTOPS_MENU_ITEM)
    MENU_ITEM_EDIT(bool, MSG_LCD_SOFT_ENDSTOPS, &soft_endstops_enabled);
  #endif

  if (
    #if IS_KINEMATIC || ENABLED(NO_MOTION_BEFORE_HOMING)
      all_axes_homed()
    #else
      true
    #endif
  ) {
    if (
      #if ENABLED(DELTA)
        current_position[Z_AXIS] <= delta_clip_start_height
      #else
        true
      #endif
    ) {
      MENU_ITEM(submenu, MSG_MOVE_X, lcd_move_get_x_amount);
      MENU_ITEM(submenu, MSG_MOVE_Y, lcd_move_get_y_amount);
    }
    #if ENABLED(DELTA)
      else
        MENU_ITEM(function, MSG_FREE_XY, lcd_lower_z_to_clip_height);
    #endif

    MENU_ITEM(submenu, MSG_MOVE_Z, lcd_move_get_z_amount);
  }
  else
    MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));

  #if ENABLED(SWITCHING_EXTRUDER) || ENABLED(SWITCHING_NOZZLE)

    #if EXTRUDERS == 6
      switch (active_extruder) {
        case 0: MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1")); break;
        case 1: MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0")); break;
        case 2: MENU_ITEM(gcode, MSG_SELECT " " MSG_E4, PSTR("T3")); break;
        case 3: MENU_ITEM(gcode, MSG_SELECT " " MSG_E3, PSTR("T2")); break;
        case 4: MENU_ITEM(gcode, MSG_SELECT " " MSG_E6, PSTR("T5")); break;
        case 5: MENU_ITEM(gcode, MSG_SELECT " " MSG_E5, PSTR("T4")); break;
      }
    #elif EXTRUDERS == 5 || EXTRUDERS == 4
      switch (active_extruder) {
        case 0: MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1")); break;
        case 1: MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0")); break;
        case 2: MENU_ITEM(gcode, MSG_SELECT " " MSG_E4, PSTR("T3")); break;
        case 3: MENU_ITEM(gcode, MSG_SELECT " " MSG_E3, PSTR("T2")); break;
      }
    #elif EXTRUDERS == 3
      if (active_extruder < 2) {
        if (active_extruder)
          MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
        else
          MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));
      }
    #else
      if (active_extruder)
        MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
      else
        MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));
    #endif

  #elif ENABLED(DUAL_X_CARRIAGE)

    if (active_extruder)
      MENU_ITEM(gcode, MSG_SELECT " " MSG_E1, PSTR("T0"));
    else
      MENU_ITEM(gcode, MSG_SELECT " " MSG_E2, PSTR("T1"));

  #endif

  #if ENABLED(SWITCHING_EXTRUDER) || ENABLED(SWITCHING_NOZZLE)

    // Only the current...
    MENU_ITEM(submenu, MSG_MOVE_E, lcd_move_get_e_amount);
    // ...and the non-switching
    #if E_MANUAL == 5
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E5, lcd_move_get_e4_amount);
    #elif E_MANUAL == 3
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E3, lcd_move_get_e2_amount);
    #endif

  #else

    // Independent extruders with one E-stepper per hotend
    MENU_ITEM(submenu, MSG_MOVE_E, lcd_move_get_e_amount);
    #if E_MANUAL > 1
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E1, lcd_move_get_e0_amount);
      MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E2, lcd_move_get_e1_amount);
      #if E_MANUAL > 2
        MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E3, lcd_move_get_e2_amount);
        #if E_MANUAL > 3
          MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E4, lcd_move_get_e3_amount);
          #if E_MANUAL > 4
            MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E5, lcd_move_get_e4_amount);
            #if E_MANUAL > 5
              MENU_ITEM(submenu, MSG_MOVE_E MSG_MOVE_E6, lcd_move_get_e5_amount);
            #endif // E_MANUAL > 5
          #endif // E_MANUAL > 4
        #endif // E_MANUAL > 3
      #endif // E_MANUAL > 2
    #endif // E_MANUAL > 1

  #endif

  END_MENU();
}

void _lcd_ubl_level_bed();
void menu_bed_leveling();

void menu_motion() {
  START_MENU();

  //
  // ^ Main
  //
  MENU_BACK(MSG_MAIN);

  //
  // Move Axis
  //
  #if ENABLED(DELTA)
    if (all_axes_homed())
  #endif
      MENU_ITEM(submenu, MSG_MOVE_AXIS, menu_move);

  //
  // Auto Home
  //
  MENU_ITEM(gcode, MSG_AUTO_HOME, PSTR("G28"));
  #if ENABLED(INDIVIDUAL_AXIS_HOMING_MENU)
    MENU_ITEM(gcode, MSG_AUTO_HOME_X, PSTR("G28 X"));
    MENU_ITEM(gcode, MSG_AUTO_HOME_Y, PSTR("G28 Y"));
    MENU_ITEM(gcode, MSG_AUTO_HOME_Z, PSTR("G28 Z"));
  #endif

  //
  // Auto Z-Align
  //
  #if ENABLED(Z_STEPPER_AUTO_ALIGN)
    MENU_ITEM(gcode, MSG_AUTO_Z_ALIGN, PSTR("G34"));
  #endif

  //
  // Level Bed
  //
  #if ENABLED(AUTO_BED_LEVELING_UBL)

    MENU_ITEM(submenu, MSG_UBL_LEVEL_BED, _lcd_ubl_level_bed);

  #elif ENABLED(LCD_BED_LEVELING)

    if (!g29_in_progress) MENU_ITEM(submenu, MSG_BED_LEVELING, menu_bed_leveling);

  #elif HAS_LEVELING && DISABLED(SLIM_LCD_MENUS)

    #if DISABLED(PROBE_MANUALLY)
      MENU_ITEM(gcode, MSG_LEVEL_BED, PSTR("G28\nG29"));
    #endif
    if (leveling_is_valid()) {
      bool new_level_state = planner.leveling_active;
      MENU_ITEM_EDIT_CALLBACK(bool, MSG_BED_LEVELING, &new_level_state, _lcd_toggle_bed_leveling);
    }
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(float3, MSG_Z_FADE_HEIGHT, &lcd_z_fade_height, 0, 100, _lcd_set_z_fade_height);
    #endif

  #endif

  #if ENABLED(LEVEL_BED_CORNERS) && DISABLED(LCD_BED_LEVELING)
    MENU_ITEM(function, MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
  #endif

  //
  // Disable Steppers
  //
  MENU_ITEM(gcode, MSG_DISABLE_STEPPERS, PSTR("M84"));

  END_MENU();
}

#endif // HAS_LCD_MENU
