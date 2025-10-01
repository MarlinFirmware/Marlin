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

#if HAS_MARLINUI_MENU

#include "menu_item.h"
#include "menu_addon.h"

#include "../../module/motion.h"
#include "../../gcode/parser.h" // for inch support
#include "../../module/temperature.h"

#if ENABLED(DELTA)
  #include "../../module/delta.h"
#endif

// Always show configurable options regardless of FT Motion active
//#define FT_MOTION_NO_MENU_TOGGLE

constexpr bool has_large_area() {
  return TERN0(HAS_X_AXIS, (X_BED_SIZE) >= 1000) || TERN0(HAS_Y_AXIS, (Y_BED_SIZE) >= 1000) || TERN0(HAS_Z_AXIS, (Z_MAX_POS) >= 1000);
}

//
// "Motion" > "Move Axis" submenu
//

void lcd_move_axis(const AxisEnum axis) {
  if (ui.use_click()) return ui.goto_previous_screen_no_defer();
  if (ui.encoderPosition && !ui.manual_move.processing) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits(axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS) {
        max = SQRT(FLOAT_SQ(PRINTABLE_RADIUS) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const float diff = float(int32_t(ui.encoderPosition)) * ui.manual_move.menu_scale;
    (void)ui.manual_move.apply_diff(axis, diff, min, max);
    ui.manual_move.soon(axis);
    ui.refresh(LCDVIEW_REDRAW_NOW);
  }
  ui.encoderPosition = 0;
  if (ui.should_draw()) {
    MenuEditItemBase::itemIndex = axis;
    const float pos = ui.manual_move.axis_value(axis);
    if (parser.using_inch_units() && !parser.axis_is_rotational(axis)) {
      const float imp_pos = parser.per_axis_value(axis, pos);
      MenuEditItemBase::draw_edit_screen(GET_TEXT_F(MSG_MOVE_N), ftostr63(imp_pos));
    }
    else
      MenuEditItemBase::draw_edit_screen(GET_TEXT_F(MSG_MOVE_N), ui.manual_move.menu_scale >= 0.1f ? (has_large_area() ? ftostr51sign(pos) : ftostr41sign(pos)) : ftostr63(pos));
  }
}

#if E_MANUAL

  static void lcd_move_e(TERN_(MULTI_E_MANUAL, const int8_t eindex=active_extruder)) {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    if (ui.encoderPosition) {
      if (!ui.manual_move.processing) {
        const float diff = float(int32_t(ui.encoderPosition)) * ui.manual_move.menu_scale;
        TERN(IS_KINEMATIC, ui.manual_move.offset, current_position.e) += diff;
        ui.manual_move.soon(E_AXIS OPTARG(MULTI_E_MANUAL, eindex));
        ui.refresh(LCDVIEW_REDRAW_NOW);
      }
      ui.encoderPosition = 0;
    }
    if (ui.should_draw()) {
      TERN_(MULTI_E_MANUAL, MenuItemBase::init(eindex));
      MenuEditItemBase::draw_edit_screen(
        GET_TEXT_F(TERN(MULTI_E_MANUAL, MSG_MOVE_EN, MSG_MOVE_E)),
        ftostr41sign(current_position.e
          PLUS_TERN0(IS_KINEMATIC, ui.manual_move.offset)
          MINUS_TERN0(MANUAL_E_MOVES_RELATIVE, ui.manual_move.e_origin)
        )
      );
    } // should_draw
  }

#endif // E_MANUAL

#if ANY(PROBE_OFFSET_WIZARD, X_AXIS_TWIST_COMPENSATION)

  void _goto_manual_move_z(const float scale) {
    ui.manual_move.menu_scale = scale;
    ui.goto_screen([]{ lcd_move_axis(Z_AXIS); });
  }

#endif

//
// "Motion" > "Move Xmm" > "Move XYZ" submenu
//

#ifndef FINE_MANUAL_MOVE
  #define FINE_MANUAL_MOVE 0.025
#endif

void _goto_manual_move(const float scale) {
  ui.defer_status_screen();
  ui.manual_move.menu_scale = scale;
  ui.goto_screen(ui.manual_move.screen_ptr);
  thermalManager.set_menu_cold_override(true);
}

void _menu_move_distance(const AxisEnum axis, const screenFunc_t func, const int8_t eindex=active_extruder) {
  ui.manual_move.screen_ptr = func;
  START_MENU();
  if (LCD_HEIGHT >= 4) {
    if (axis < NUM_AXES)
      STATIC_ITEM_N(axis, MSG_MOVE_N, SS_DEFAULT|SS_INVERT);
    else {
      TERN_(MANUAL_E_MOVES_RELATIVE, ui.manual_move.e_origin = current_position.e);
      STATIC_ITEM_N(eindex, MSG_MOVE_EN, SS_DEFAULT|SS_INVERT);
    }
  }

  BACK_ITEM(MSG_MOVE_AXIS);

  #define __LINEAR_LIMIT(D) ((D) < max_length(axis) / 2 + 1)
  #if HAS_EXTRUDERS
    #ifndef EXTRUDE_MAXLENGTH
      #define EXTRUDE_MAXLENGTH 50
    #endif
    #define _LINEAR_LIMIT(D) ((axis < E_AXIS) ? __LINEAR_LIMIT(D) : ((D) < (EXTRUDE_MAXLENGTH) / 2 + 1))
  #else
    #define _LINEAR_LIMIT __LINEAR_LIMIT
  #endif
  #define __MOVE_SUB(L,T,D) if (rotational[axis] || _LINEAR_LIMIT(D)) SUBMENU_S(F(T), L, []{ _goto_manual_move(D); })

  if (rotational[axis]) {
    #ifdef MANUAL_MOVE_DISTANCE_DEG
      #define _MOVE_DEG(D) __MOVE_SUB(MSG_MOVE_N_DEG, STRINGIFY(D), D);
      MAP(_MOVE_DEG, MANUAL_MOVE_DISTANCE_DEG)
    #endif
  }
  else if (parser.using_inch_units()) {
    #ifdef MANUAL_MOVE_DISTANCE_IN
      #define _MOVE_IN(I) __MOVE_SUB(MSG_MOVE_N_MM, STRINGIFY(I), IN_TO_MM(I));
      MAP(_MOVE_IN, MANUAL_MOVE_DISTANCE_IN)
    #endif
  }
  else {
    #ifdef MANUAL_MOVE_DISTANCE_MM
      #define _MOVE_MM(M) __MOVE_SUB(MSG_MOVE_N_MM, STRINGIFY(M), M);
      MAP(_MOVE_MM, MANUAL_MOVE_DISTANCE_MM)
    #endif
    #if HAS_Z_AXIS
      if (axis == Z_AXIS && (FINE_MANUAL_MOVE) > 0.0f && (FINE_MANUAL_MOVE) < 0.1f)
        SUBMENU_f(F(STRINGIFY(FINE_MANUAL_MOVE)), MSG_MOVE_N_MM, []{ _goto_manual_move(float(FINE_MANUAL_MOVE)); });
    #endif
  }
  END_MENU();
}

#if E_MANUAL

  inline void _goto_menu_move_distance_e() {
    ui.goto_screen([]{ _menu_move_distance(E_AXIS, []{ lcd_move_e(); }); });
  }

  inline void _menu_move_distance_e_maybe() {
    if (thermalManager.tooColdToExtrude(active_extruder)) {
      ui.goto_screen([]{
        MenuItem_confirm::select_screen(
          GET_TEXT_F(MSG_BUTTON_PROCEED), GET_TEXT_F(MSG_BACK),
          _goto_menu_move_distance_e, nullptr,
          GET_TEXT_F(MSG_HOTEND_TOO_COLD), (const char *)nullptr, F("!")
        );
      });
    }
    else
      _goto_menu_move_distance_e();
  }

#endif

void menu_move() {
  START_MENU();
  BACK_ITEM(MSG_MOTION);

  #if ALL(HAS_SOFTWARE_ENDSTOPS, SOFT_ENDSTOPS_MENU_ITEM)
    EDIT_ITEM(bool, MSG_LCD_SOFT_ENDSTOPS, &soft_endstop._enabled);
  #endif

  // Move submenu for each axis
  if (NONE(IS_KINEMATIC, NO_MOTION_BEFORE_HOMING) || all_axes_homed()) {
    if (TERN1(DELTA, current_position.z <= delta_clip_start_height)) {
      #if HAS_X_AXIS
        SUBMENU_N(X_AXIS, MSG_MOVE_N, []{ _menu_move_distance(X_AXIS, []{ lcd_move_axis(X_AXIS); }); });
      #endif
      #if HAS_Y_AXIS
        SUBMENU_N(Y_AXIS, MSG_MOVE_N, []{ _menu_move_distance(Y_AXIS, []{ lcd_move_axis(Y_AXIS); }); });
      #endif
    }
    else {
      #if ENABLED(DELTA)
        ACTION_ITEM(MSG_FREE_XY, []{ line_to_z(delta_clip_start_height); ui.synchronize(); });
      #endif
    }
    #if HAS_Z_AXIS
      #define _AXIS_MOVE(N) SUBMENU_N(N, MSG_MOVE_N, []{ _menu_move_distance(AxisEnum(N), []{ lcd_move_axis(AxisEnum(N)); }); });
      REPEAT_S(2, NUM_AXES, _AXIS_MOVE);
    #endif
  }
  else
    GCODES_ITEM(MSG_AUTO_HOME, FPSTR(G28_STR));

  #if ANY(HAS_SWITCHING_EXTRUDER, HAS_SWITCHING_NOZZLE, MAGNETIC_SWITCHING_TOOLHEAD)

    #if EXTRUDERS >= 4
      switch (active_extruder) {
        case 0: GCODES_ITEM_N(1, MSG_SELECT_E, F("T1")); break;
        case 1: GCODES_ITEM_N(0, MSG_SELECT_E, F("T0")); break;
        case 2: GCODES_ITEM_N(3, MSG_SELECT_E, F("T3")); break;
        case 3: GCODES_ITEM_N(2, MSG_SELECT_E, F("T2")); break;
        #if EXTRUDERS == 6
          case 4: GCODES_ITEM_N(5, MSG_SELECT_E, F("T5")); break;
          case 5: GCODES_ITEM_N(4, MSG_SELECT_E, F("T4")); break;
        #endif
      }
    #elif EXTRUDERS == 3
      if (active_extruder < 2)
        GCODES_ITEM_N(1 - active_extruder, MSG_SELECT_E, active_extruder ? F("T0") : F("T1"));
    #else
      GCODES_ITEM_N(1 - active_extruder, MSG_SELECT_E, active_extruder ? F("T0") : F("T1"));
    #endif

  #elif ENABLED(DUAL_X_CARRIAGE)

    GCODES_ITEM_N(1 - active_extruder, MSG_SELECT_E, active_extruder ? F("T0") : F("T1"));

  #endif

  #if E_MANUAL

    // The current extruder
    SUBMENU(MSG_MOVE_E, _menu_move_distance_e_maybe);

    #define SUBMENU_MOVE_E(N) SUBMENU_N(N, MSG_MOVE_EN, []{ _menu_move_distance(E_AXIS, []{ lcd_move_e(N); }, N); });

    #if HAS_SWITCHING_EXTRUDER || HAS_SWITCHING_NOZZLE

      // ...and the non-switching
      #if E_MANUAL == 7 || E_MANUAL == 5 || E_MANUAL == 3
        SUBMENU_MOVE_E(E_MANUAL - 1);
      #endif

    #elif MULTI_E_MANUAL

      // Independent extruders with one E stepper per hotend
      REPEAT(E_MANUAL, SUBMENU_MOVE_E);

    #endif

  #endif // E_MANUAL

  END_MENU();
}

#define _HOME_ITEM(N) GCODES_ITEM_N(N##_AXIS, MSG_AUTO_HOME_N, F("G28" STR_##N));

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  //
  // "Motion" > "Homing" submenu
  //
  void menu_home() {
    START_MENU();
    BACK_ITEM(MSG_MOTION);

    GCODES_ITEM(MSG_AUTO_HOME, FPSTR(G28_STR));
    MAIN_AXIS_MAP(_HOME_ITEM);

    END_MENU();
  }
#endif

#if ENABLED(FT_MOTION_MENU)

  #include "../../module/ft_motion.h"

  FSTR_P get_shaper_name(const AxisEnum axis) {
    switch (ftMotion.cfg.shaper[axis]) {
      default: return nullptr;
      case ftMotionShaper_NONE:  return GET_TEXT_F(MSG_LCD_OFF);
      case ftMotionShaper_ZV:    return GET_TEXT_F(MSG_FTM_ZV);
      case ftMotionShaper_ZVD:   return GET_TEXT_F(MSG_FTM_ZVD);
      case ftMotionShaper_ZVDD:  return GET_TEXT_F(MSG_FTM_ZVDD);
      case ftMotionShaper_ZVDDD: return GET_TEXT_F(MSG_FTM_ZVDDD);
      case ftMotionShaper_EI:    return GET_TEXT_F(MSG_FTM_EI);
      case ftMotionShaper_2HEI:  return GET_TEXT_F(MSG_FTM_2HEI);
      case ftMotionShaper_3HEI:  return GET_TEXT_F(MSG_FTM_3HEI);
      case ftMotionShaper_MZV:   return GET_TEXT_F(MSG_FTM_MZV);
    }
  }

  FSTR_P get_trajectory_name() {
    switch (ftMotion.getTrajectoryType()) {
      default:
      case TrajectoryType::TRAPEZOIDAL: return GET_TEXT_F(MSG_FTM_TRAPEZOIDAL);
      case TrajectoryType::POLY5:       return GET_TEXT_F(MSG_FTM_POLY5);
      case TrajectoryType::POLY6:       return GET_TEXT_F(MSG_FTM_POLY6);
    }
  }

  #if HAS_DYNAMIC_FREQ
    FSTR_P get_dyn_freq_mode_name() {
      switch (ftMotion.cfg.dynFreqMode) {
        default:
        case dynFreqMode_DISABLED:   return GET_TEXT_F(MSG_LCD_OFF);
        case dynFreqMode_Z_BASED:    return GET_TEXT_F(MSG_FTM_Z_BASED);
        case dynFreqMode_MASS_BASED: return GET_TEXT_F(MSG_FTM_MASS_BASED);
      }
    }
  #endif

  void ftm_menu_set_shaper(ftMotionShaper_t &outShaper, const ftMotionShaper_t s) {
    outShaper = s;
    ftMotion.update_shaping_params();
    ui.go_back();
  }

  #define MENU_FTM_SHAPER(A) \
    inline void menu_ftm_shaper_##A() { \
      const ftMotionShaper_t shaper = ftMotion.cfg.shaper.A; \
      START_MENU(); \
      BACK_ITEM(MSG_FIXED_TIME_MOTION); \
      if (shaper != ftMotionShaper_NONE)  ACTION_ITEM(MSG_LCD_OFF,  []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_NONE  ); }); \
      if (shaper != ftMotionShaper_ZV)    ACTION_ITEM(MSG_FTM_ZV,   []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_ZV    ); }); \
      if (shaper != ftMotionShaper_ZVD)   ACTION_ITEM(MSG_FTM_ZVD,  []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_ZVD   ); }); \
      if (shaper != ftMotionShaper_ZVDD)  ACTION_ITEM(MSG_FTM_ZVDD, []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_ZVDD  ); }); \
      if (shaper != ftMotionShaper_ZVDDD) ACTION_ITEM(MSG_FTM_ZVDDD,[]{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_ZVDDD ); }); \
      if (shaper != ftMotionShaper_EI)    ACTION_ITEM(MSG_FTM_EI,   []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_EI    ); }); \
      if (shaper != ftMotionShaper_2HEI)  ACTION_ITEM(MSG_FTM_2HEI, []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_2HEI  ); }); \
      if (shaper != ftMotionShaper_3HEI)  ACTION_ITEM(MSG_FTM_3HEI, []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_3HEI  ); }); \
      if (shaper != ftMotionShaper_MZV)   ACTION_ITEM(MSG_FTM_MZV,  []{ ftm_menu_set_shaper(ftMotion.cfg.shaper.A, ftMotionShaper_MZV   ); }); \
      END_MENU(); \
    }

  SHAPED_MAP(MENU_FTM_SHAPER);

  void menu_ftm_trajectory_generator() {
    const TrajectoryType current_type = ftMotion.getTrajectoryType();
    START_MENU();
    BACK_ITEM(MSG_FIXED_TIME_MOTION);
    if (current_type != TrajectoryType::TRAPEZOIDAL) ACTION_ITEM(MSG_FTM_TRAPEZOIDAL, []{ planner.synchronize(); ftMotion.setTrajectoryType(TrajectoryType::TRAPEZOIDAL);  ui.go_back(); });
    if (current_type != TrajectoryType::POLY5)       ACTION_ITEM(MSG_FTM_POLY5,       []{ planner.synchronize(); ftMotion.setTrajectoryType(TrajectoryType::POLY5);        ui.go_back(); });
    if (current_type != TrajectoryType::POLY6)       ACTION_ITEM(MSG_FTM_POLY6,       []{ planner.synchronize(); ftMotion.setTrajectoryType(TrajectoryType::POLY6);        ui.go_back(); });
    END_MENU();
  }

  #if HAS_DYNAMIC_FREQ

    void menu_ftm_dyn_mode() {
      const dynFreqMode_t dmode = ftMotion.cfg.dynFreqMode;

      START_MENU();
      BACK_ITEM(MSG_FIXED_TIME_MOTION);

      if (dmode != dynFreqMode_DISABLED) ACTION_ITEM(MSG_LCD_OFF, []{ ftMotion.cfg.dynFreqMode = dynFreqMode_DISABLED; ui.go_back(); });
      #if HAS_DYNAMIC_FREQ_MM
        if (dmode != dynFreqMode_Z_BASED) ACTION_ITEM(MSG_FTM_Z_BASED, []{ ftMotion.cfg.dynFreqMode = dynFreqMode_Z_BASED; ui.go_back(); });
      #endif
      #if HAS_DYNAMIC_FREQ_G
        if (dmode != dynFreqMode_MASS_BASED) ACTION_ITEM(MSG_FTM_MASS_BASED, []{ ftMotion.cfg.dynFreqMode = dynFreqMode_MASS_BASED; ui.go_back(); });
      #endif

      END_MENU();
    }

  #endif // HAS_DYNAMIC_FREQ

  // Suppress warning about storing a stack address in a static string pointer
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wdangling-pointer"

  #if ALL(__AVR__, HAS_MARLINUI_U8GLIB) && DISABLED(OPTIMIZE_FT_MOTION_FOR_SIZE)
    #define CACHE_FOR_SPEED 1
  #endif

  void menu_ft_motion() {
    // Define stuff ahead of the menu loop
    ft_config_t &c = ftMotion.cfg;

    #ifdef __AVR__
      // Copy Flash strings to RAM for C-string substitution
      // For U8G paged rendering check and skip extra string copy
      #if HAS_X_AXIS
        MString<20> shaper_name;
        #if CACHE_FOR_SPEED
          int8_t prev_a = -1;
        #endif
        auto _shaper_name = [&](const AxisEnum a) {
          if (TERN1(CACHE_FOR_SPEED, a != prev_a)) {
            TERN_(CACHE_FOR_SPEED, prev_a = a);
            shaper_name = get_shaper_name(a);
          }
          return shaper_name;
        };
      #endif
      #if HAS_DYNAMIC_FREQ
        MString<20> dmode;
        #if CACHE_FOR_SPEED
          bool got_d = false;
        #endif
        auto _dmode = [&]{
          if (TERN1(CACHE_FOR_SPEED, !got_d)) {
            TERN_(CACHE_FOR_SPEED, got_d = true);
            dmode = get_dyn_freq_mode_name();
          }
          return dmode;
        };
      #endif
      MString<20> traj_name;
      #if CACHE_FOR_SPEED
        bool got_t = false;
      #endif
      auto _traj_name = [&]{
        if (TERN1(CACHE_FOR_SPEED, !got_t)) {
          TERN_(CACHE_FOR_SPEED, got_t = true);
          traj_name = get_trajectory_name();
        }
        return traj_name;
      };
    #else
      auto _shaper_name = [](const AxisEnum a) { return get_shaper_name(a); };
      auto _dmode = []{ return get_dyn_freq_mode_name(); };
      auto _traj_name = []{ return get_trajectory_name(); };
    #endif

    START_MENU();
    BACK_ITEM(MSG_MOTION);

    bool show_state = c.active;
    EDIT_ITEM(bool, MSG_FIXED_TIME_MOTION, &show_state, []{
      FLIP(ftMotion.cfg.active);
      ftMotion.update_shaping_params();
    });

    // Show only when FT Motion is active (or optionally always show)
    if (c.active || ENABLED(FT_MOTION_NO_MENU_TOGGLE)) {
      #define SHAPER_MENU_ITEM(A) \
        SUBMENU_N_S(_AXIS(A), _shaper_name(_AXIS(A)), MSG_FTM_CMPN_MODE, menu_ftm_shaper_##A); \
        if (AXIS_IS_SHAPING(A)) { \
          EDIT_ITEM_FAST_N(float42_52, _AXIS(A), MSG_FTM_BASE_FREQ_N, &c.baseFreq.A, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2, ftMotion.update_shaping_params); \
          EDIT_ITEM_FAST_N(float42_52, _AXIS(A), MSG_FTM_ZETA_N, &c.zeta.A, 0.0f, 1.0f, ftMotion.update_shaping_params); \
          if (AXIS_IS_EISHAPING(A)) \
            EDIT_ITEM_FAST_N(float42_52, _AXIS(A), MSG_FTM_VTOL_N, &c.vtol.A, 0.0f, 1.0f, ftMotion.update_shaping_params); \
        }
      SUBMENU_S(_traj_name(), MSG_FTM_TRAJECTORY, menu_ftm_trajectory_generator);

      if (ftMotion.getTrajectoryType() == TrajectoryType::POLY6)
        EDIT_ITEM(float42_52, MSG_FTM_POLY6_OVERSHOOT, &c.poly6_acceleration_overshoot, 1.25f, 1.875f);

      SHAPED_MAP(SHAPER_MENU_ITEM);

      #if HAS_DYNAMIC_FREQ
        SUBMENU_S(_dmode(), MSG_FTM_DYN_MODE, menu_ftm_dyn_mode);
        if (c.dynFreqMode != dynFreqMode_DISABLED) {
          #define _DYN_MENU_ITEM(A) EDIT_ITEM_FAST_N(float42_52, _AXIS(A), MSG_FTM_DFREQ_K_N, &c.dynFreqK.A, 0.0f, 20.0f);
          SHAPED_MAP(_DYN_MENU_ITEM);
        }
      #endif

      #if HAS_EXTRUDERS
        EDIT_ITEM(bool, MSG_LINEAR_ADVANCE, &c.linearAdvEna);
        if (c.linearAdvEna || ENABLED(FT_MOTION_NO_MENU_TOGGLE))
          EDIT_ITEM(float42_52, MSG_ADVANCE_K, &c.linearAdvK, 0.0f, 10.0f);
      #endif

      EDIT_ITEM(bool, MSG_FTM_AXIS_SYNC, &c.axis_sync_enabled);
      #if ENABLED(FTM_SMOOTHING)
        #if HAS_X_AXIS
          editable.decimal = c.smoothingTime.X;
          EDIT_ITEM_FAST_N(float43, X_AXIS, MSG_FTM_SMOOTH_TIME_N, &editable.decimal, 0.0f, FTM_MAX_SMOOTHING_TIME, []{ ftMotion.set_smoothing_time(X_AXIS, editable.decimal); });
        #endif
        #if HAS_Y_AXIS
          editable.decimal = c.smoothingTime.Y;
          EDIT_ITEM_FAST_N(float43, Y_AXIS, MSG_FTM_SMOOTH_TIME_N, &editable.decimal, 0.0f, FTM_MAX_SMOOTHING_TIME, []{ ftMotion.set_smoothing_time(Y_AXIS, editable.decimal); });
        #endif
        #if HAS_Z_AXIS
          editable.decimal = c.smoothingTime.Z;
          EDIT_ITEM_FAST_N(float43, Z_AXIS, MSG_FTM_SMOOTH_TIME_N, &editable.decimal, 0.0f, FTM_MAX_SMOOTHING_TIME, []{ ftMotion.set_smoothing_time(Z_AXIS, editable.decimal); });
        #endif
        #if HAS_EXTRUDERS
          editable.decimal = c.smoothingTime.E;
          EDIT_ITEM_FAST_N(float43, E_AXIS, MSG_FTM_SMOOTH_TIME_N, &editable.decimal, 0.0f, FTM_MAX_SMOOTHING_TIME, []{ ftMotion.set_smoothing_time(E_AXIS, editable.decimal); });
        #endif
      #endif
    }
    END_MENU();
  } // menu_ft_motion

  void menu_tune_ft_motion() {
    // Define stuff ahead of the menu loop
    ft_config_t &c = ftMotion.cfg;
    #ifdef __AVR__
      // Copy Flash strings to RAM for C-string substitution
      // For U8G paged rendering check and skip extra string copy
      #if HAS_X_AXIS
        MString<20> shaper_name;
        #if CACHE_FOR_SPEED
          int8_t prev_a = -1;
        #endif
        auto _shaper_name = [&](const AxisEnum a) {
          if (TERN1(CACHE_FOR_SPEED, a != prev_a)) {
            TERN_(CACHE_FOR_SPEED, prev_a = a);
            shaper_name = get_shaper_name(a);
          }
          return shaper_name;
        };
      #endif
      #if HAS_DYNAMIC_FREQ
        MString<20> dmode;
        #if CACHE_FOR_SPEED
          bool got_d = false;
        #endif
        auto _dmode = [&]{
          if (TERN1(CACHE_FOR_SPEED, !got_d)) {
            TERN_(CACHE_FOR_SPEED, got_d = true);
            dmode = get_dyn_freq_mode_name();
          }
          return dmode;
        };
      #endif
      MString<20> traj_name;
      #if CACHE_FOR_SPEED
        bool got_t = false;
      #endif
      auto _traj_name = [&]{
        if (TERN1(CACHE_FOR_SPEED, !got_t)) {
          TERN_(CACHE_FOR_SPEED, got_t = true);
          traj_name = get_trajectory_name();
        }
        return traj_name;
      };
    #else // !__AVR__
      auto _shaper_name = [](const AxisEnum a) { return get_shaper_name(a); };
      auto _dmode = []{ return get_dyn_freq_mode_name(); };
      auto _traj_name = []{ return get_trajectory_name(); };
    #endif

    START_MENU();
    BACK_ITEM(MSG_TUNE);

    SUBMENU_S(_traj_name(), MSG_FTM_TRAJECTORY, menu_ftm_trajectory_generator);

    if (ftMotion.getTrajectoryType() == TrajectoryType::POLY6)
      EDIT_ITEM(float42_52, MSG_FTM_POLY6_OVERSHOOT, &c.poly6_acceleration_overshoot, 1.25f, 1.875f);

    #define _CMPM_MENU_ITEM(A) SUBMENU_N_S(_AXIS(A), _shaper_name(_AXIS(A)), MSG_FTM_CMPN_MODE, menu_ftm_shaper_##A);
    SHAPED_MAP(_CMPM_MENU_ITEM);

    #if HAS_DYNAMIC_FREQ
      SUBMENU_S(_dmode(), MSG_FTM_DYN_MODE, menu_ftm_dyn_mode);
    #endif

    #if HAS_EXTRUDERS
      EDIT_ITEM(bool, MSG_LINEAR_ADVANCE, &c.linearAdvEna);
      if (c.linearAdvEna || ENABLED(FT_MOTION_NO_MENU_TOGGLE))
        EDIT_ITEM(float42_52, MSG_ADVANCE_K, &c.linearAdvK, 0.0f, 10.0f);
    #endif

    #if ENABLED(FTM_SMOOTHING)
      #define _SMOO_MENU_ITEM(A) do{ \
        editable.decimal = c.smoothingTime.A; \
        EDIT_ITEM_FAST_N(float43, _AXIS(A), MSG_FTM_SMOOTH_TIME_N, &editable.decimal, 0.0f, FTM_MAX_SMOOTHING_TIME, []{ ftMotion.set_smoothing_time(_AXIS(A), editable.decimal); }); \
      }while(0);
      CARTES_MAP(_SMOO_MENU_ITEM);
    #endif

    END_MENU();
  } // menu_tune_ft_motion

  #pragma GCC diagnostic pop

#endif // FT_MOTION_MENU

void menu_motion() {

  START_MENU();

  //
  // ^ Main
  //
  BACK_ITEM(MSG_MAIN_MENU);

  //
  // Move Axis
  //
  if (TERN1(DELTA, all_axes_homed()))
    SUBMENU(MSG_MOVE_AXIS, menu_move);

  //
  // Auto Home
  //
  #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
    SUBMENU(MSG_HOMING, menu_home);
  #else
    GCODES_ITEM(MSG_AUTO_HOME, FPSTR(G28_STR));
    #if ENABLED(INDIVIDUAL_AXIS_HOMING_MENU)
      MAIN_AXIS_MAP(_HOME_ITEM);
    #endif
  #endif

  //
  // M493 - Fixed-Time Motion
  //
  #if ENABLED(FT_MOTION_MENU)
    SUBMENU(MSG_FIXED_TIME_MOTION, menu_ft_motion);
  #endif

  //
  // Pen up/down menu
  //
  #if ENABLED(PEN_UP_DOWN_MENU)
    GCODES_ITEM(MSG_MANUAL_PENUP, F("M280 P0 S90"));
    GCODES_ITEM(MSG_MANUAL_PENDOWN, F("M280 P0 S50"));
  #endif

  //
  // Auto-calibration with Object
  //
  #if ENABLED(CALIBRATION_GCODE)
    GCODES_ITEM(MSG_AUTO_CALIBRATE, F("G425"));
  #endif

  //
  // Disable Steppers
  //
  GCODES_ITEM(MSG_DISABLE_STEPPERS, F("M84"));

  END_MENU();
} // menu_motion

#endif // HAS_MARLINUI_MENU
