/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if IS_DWIN_MARLINUI

#include "marlinui_dwin.h"
#include "dwin_lcd.h"
#include "dwin_string.h"
#include "lcdprint_dwin.h"

#include "../../fontutils.h"
#include "../../../libs/numtostr.h"
#include "../../marlinui.h"

#include "../../../sd/cardreader.h"
#include "../../../module/motion.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/planner.h"

#if ENABLED(SDSUPPORT)
  #include "../../../libs/duration_t.h"
#endif

#if ENABLED(LCD_SHOW_E_TOTAL)
  #include "../../../MarlinCore.h" // for printingIsActive
#endif

#if ENABLED(DWIN_MARLINUI_PORTRAIT)
  #define STATUS_HEATERS_X    15
  #define STATUS_HEATERS_Y    56
#else
  #define STATUS_HEATERS_X   154
  #define STATUS_HEATERS_Y    10
#endif
#define STATUS_HEATERS_XSPACE 64
#define STATUS_FAN_WIDTH      48
#define STATUS_FAN_HEIGHT     48
#define STATUS_FAN_Y          STATUS_HEATERS_Y + 22
#define STATUS_CHR_WIDTH      14
#define STATUS_CHR_HEIGHT     28

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink, const uint16_t x, const uint16_t y) {

  #if ENABLED(DWIN_MARLINUI_PORTRAIT)

    uint8_t vallen = utf8_strlen(value);
    if (!ui.did_first_redraw) {
      dwin_string.set();
      dwin_string.add('X' + axis);
      DWIN_Draw_String(true, font16x32, Color_IconBlue, Color_Bg_Black, x + (vallen * 14 - 14) / 2, y + 2, S(dwin_string.string()));
    }

    dwin_string.set();
    if (blink)
      dwin_string.add(value);
    else if (!TEST(axis_homed, axis))
      while (const char c = *value++) dwin_string.add(c <= '.' ? c : '?');
    else if (NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING) && !TEST(axis_trusted, axis))
      dwin_string.add(TERN1(DWIN_MARLINUI_PORTRAIT, axis == Z_AXIS) ? PSTR("       ") : PSTR("    "));
    else
      dwin_string.add(value);

    // For E_TOTAL there may be some characters to cover up
    if (BOTH(DWIN_MARLINUI_PORTRAIT, LCD_SHOW_E_TOTAL) && axis == X_AXIS)
      dwin_string.add("   ");

    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x, y + 32, S(dwin_string.string()));

  #else // !DWIN_MARLINUI_PORTRAIT

    if (!ui.did_first_redraw || ui.old_is_printing != print_job_timer.isRunning()) {
      dwin_string.set();
      dwin_string.add('X' + axis);
      DWIN_Draw_String(true, font16x32, Color_IconBlue, Color_Bg_Black, x, y, S(dwin_string.string()));
    }

    dwin_string.set();
    if (blink)
      dwin_string.add(value);
    else {
      if (!TEST(axis_homed, axis))
        while (const char c = *value++) dwin_string.add(c <= '.' ? c : '?');
      else {
        #if NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING)
          if (!TEST(axis_trusted, axis))
            dwin_string.add(TERN1(DWIN_MARLINUI_PORTRAIT, axis == Z_AXIS) ? PSTR("       ") : PSTR("    "));
          else
        #endif
            dwin_string.add(value);
      }
    }

    // For E_TOTAL there may be some characters to cover up
    if (ENABLED(LCD_SHOW_E_TOTAL) && (!ui.did_first_redraw  || ui.old_is_printing != print_job_timer.isRunning()) && axis == X_AXIS)
      dwin_string.add("   ");

    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x + 32, y + 4, S(dwin_string.string()));

  #endif // !DWIN_MARLINUI_PORTRAIT
}

#if ENABLED(LCD_SHOW_E_TOTAL)

  FORCE_INLINE void _draw_e_value(const_float_t value, const uint16_t x, const uint16_t y) {
    const uint8_t scale = value >= 100000.0f ? 10 : 1; // show cm after 99,999mm

    #if ENABLED(DWIN_MARLINUI_PORTRAIT)

      if (!ui.did_first_redraw) {
        // Extra spaces to erase previous value
        dwin_string.set("E         ");
        DWIN_Draw_String(true, font16x32, Color_IconBlue, Color_Bg_Black, x + (4 * 14 / 2) - 7, y + 2, S(dwin_string.string()));
      }

      dwin_string.set(ui16tostr5rj(value / scale));
      DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x, y + 32, S(dwin_string.string()));

      // Extra spaces to erase previous value
      DWIN_Draw_String(true, font14x28, Color_IconBlue, Color_Bg_Black, x + (5 * 14), y + 32, S(scale == 1 ? "mm      " : "cm      "));

    #else // !DWIN_MARLINUI_PORTRAIT

      if (!ui.did_first_redraw || ui.old_is_printing != print_job_timer.isRunning()) {
        dwin_string.set("E ");
        DWIN_Draw_String(true, font16x32, Color_IconBlue, Color_Bg_Black, x, y, S(dwin_string.string()));
      }

      dwin_string.set(ui16tostr5rj(value / scale));
      DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x + 32, y + 4, S(dwin_string.string()));

      DWIN_Draw_String(true, font14x28, Color_IconBlue, Color_Bg_Black, x + (32 + 70), y + 4, S(scale == 1 ? "mm      " : "cm      "));

    #endif // !DWIN_MARLINUI_PORTRAIT
  }

#endif // LCD_SHOW_E_TOTAL

//
// Fan Icon and Percentage
//
FORCE_INLINE void _draw_fan_status(const uint16_t x, const uint16_t y) {
  const uint16_t fanx = (4 * STATUS_CHR_WIDTH - STATUS_FAN_WIDTH) / 2;
  const uint8_t fan_pct = thermalManager.scaledFanSpeedPercent(0);
  const bool fan_on = !!fan_pct;
  if (fan_on) {
    DWIN_ICON_Animation(0, fan_on, ICON, ICON_Fan0, ICON_Fan3, x + fanx, y, 25);
    dwin_string.set(i8tostr3rj(fan_pct));
    dwin_string.add('%');
    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x, y + STATUS_FAN_HEIGHT, S(dwin_string.string()));
  }
  else {
    DWIN_ICON_AnimationControl(0x0000); // disable all icon animations (this is the only one)
    DWIN_ICON_Show(ICON, ICON_Fan0, x + fanx, y);
    dwin_string.set(PSTR("    "));
    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x, y + STATUS_FAN_HEIGHT, S(dwin_string.string()));
  }
}

/**
 * Draw a single heater icon with current and target temperature, at the given XY
 */
FORCE_INLINE void _draw_heater_status(const heater_id_t heater, const uint16_t x, const uint16_t y) {

  #if HAS_HOTEND
    #if HOTENDS > 2
      #define HOTEND_STATS 3
    #elif HOTENDS > 1
      #define HOTEND_STATS 2
    #else
      #define HOTEND_STATS 1
    #endif
    static celsius_t old_temp[HOTEND_STATS] = ARRAY_N_1(HOTEND_STATS, 500),
                   old_target[HOTEND_STATS] = ARRAY_N_1(HOTEND_STATS, 500);
    static bool old_on[HOTEND_STATS] = ARRAY_N_1(HOTEND_STATS, false);
  #endif

  #if HAS_HEATED_BED
    static celsius_t old_bed_temp = 500, old_bed_target = 500;
    static bool old_bed_on = false;
    #if HAS_LEVELING
      static bool old_leveling_on = false;
    #endif
  #endif

  #if HAS_HOTEND && HAS_HEATED_BED
    float tc, tt;
    bool c_draw, t_draw, i_draw, ta;
    const bool isBed = heater < 0;
    if (isBed) {
      tc = thermalManager.degBed();
      tt = thermalManager.degTargetBed();
      ta = thermalManager.isHeatingBed();
      c_draw = tc != old_bed_temp;
      t_draw = tt != old_bed_target;
      i_draw = ta != old_bed_on;
      old_bed_temp = tc;
      old_bed_target = tt;
      old_bed_on = ta;
    }
    else {
      tc = thermalManager.degHotend(heater);
      tt = thermalManager.degTargetHotend(heater);
      ta = thermalManager.isHeatingHotend(heater);
      c_draw = tc != old_temp[heater];
      t_draw = tt != old_target[heater];
      i_draw = ta != old_on[heater];
      old_temp[heater] = tc;
      old_target[heater] = tt;
      old_on[heater] = ta;
    }
  #elif HAS_HOTEND
    constexpr bool isBed = false;
    const float tc = thermalManager.degHotend(heater), tt = thermalManager.degTargetHotend(heater);
    const uint8_t ta = thermalManager.isHeatingHotend(heater);
    bool c_draw = tc != old_temp[heater], t_draw = tt != old_target[heater], i_draw = ta != old_on[heater];
    old_temp[heater] = tc; old_target[heater] = tt; old_on[heater] = ta;
  #elif HAS_HEATED_BED
    constexpr bool isBed = true;
    const float tc = thermalManager.degBed(), tt = thermalManager.degTargetBed();
    const uint8_t ta = thermalManager.isHeatingBed();
    bool c_draw = tc != old_bed_temp, t_draw = tt != old_bed_target, i_draw = ta != old_bed_on;
    old_bed_temp = tc; old_bed_target = tt; old_bed_on = ta;
  #else
    bool c_draw = false, t_draw = false, i_draw = false;
    constexpr float tc = 0, tt = 0;
    constexpr uint8_t ta = 0;
  #endif

  #if HAS_HEATED_BED && HAS_LEVELING
    if (isBed) {
      i_draw |= (planner.leveling_active != old_leveling_on);
      old_leveling_on = planner.leveling_active;
    }
  #endif

  // Draw target temperature, if needed
  if (!ui.did_first_redraw || t_draw) {
    dwin_string.set(i16tostr3rj(tt + 0.5));
    dwin_string.add(LCD_STR_DEGREE);
    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x, y, S(dwin_string.string()));
  }

  // Draw heater icon with on / off / leveled states
  if (!ui.did_first_redraw || i_draw) {
    const uint8_t ico = isBed ? (TERN0(HAS_LEVELING, planner.leveling_active) ? ICON_BedLevelOff : ICON_BedOff) : ICON_HotendOff;
    DWIN_ICON_Show(ICON, ico + ta, x, y + STATUS_CHR_HEIGHT + 2);
  }

  // Draw current temperature, if needed
  if (!ui.did_first_redraw || c_draw) {
    dwin_string.set(i16tostr3rj(tc + 0.5));
    dwin_string.add(LCD_STR_DEGREE);
    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x, y + 70, S(dwin_string.string()));
  }
}

/**
 * Draw the current "feed rate" percentage preceded by the >> character
 */
FORCE_INLINE void _draw_feedrate_status(const char *value, uint16_t x, uint16_t y) {
  if (!ui.did_first_redraw) {
    dwin_string.set(LCD_STR_FEEDRATE);
    DWIN_Draw_String(true, font14x28, Color_IconBlue, Color_Bg_Black, x, y, S(dwin_string.string()));
  }

  dwin_string.set(value);
  dwin_string.add(PSTR("%"));
  DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, x + 14, y, S(dwin_string.string()));
}

/**
 * Draw the MarlinUI Status Screen for Ender 3 V2
 */
void MarlinUI::draw_status_screen() {
  const bool blink = get_blink();

  // Draw elements that never change
  if (!ui.did_first_redraw) {
    // Logo/Status Icon
    #define STATUS_LOGO_WIDTH  128
    #define STATUS_LOGO_HEIGHT  40
    DWIN_ICON_Show(ICON, ICON_LOGO_Marlin,
      #if ENABLED(DWIN_MARLINUI_PORTRAIT)
        (LCD_PIXEL_WIDTH - (STATUS_LOGO_WIDTH)) / 2, ((STATUS_HEATERS_Y - 4) - (STATUS_LOGO_HEIGHT)) / 2
      #else
        5, 42
      #endif
    );

    // Draw a frame around the x/y/z values
    DWIN_Draw_Rectangle(0, Select_Color,
      #if ENABLED(DWIN_MARLINUI_PORTRAIT)
        0, 193, LCD_PIXEL_WIDTH, 260
      #else
        0, 115, LCD_PIXEL_WIDTH - 1, 152
      #endif
    );
  }

  uint16_t hx = STATUS_HEATERS_X;
  #if HAS_HOTEND
    _draw_heater_status(H_E0, hx, STATUS_HEATERS_Y);
    hx += STATUS_HEATERS_XSPACE;
  #endif
  #if HAS_MULTI_HOTEND
    _draw_heater_status(H_E1, hx, STATUS_HEATERS_Y);
    hx += STATUS_HEATERS_XSPACE;
  #endif
  #if HAS_HEATED_BED
    _draw_heater_status(H_BED, hx, STATUS_HEATERS_Y);
  #endif

  #if HAS_FAN
    _draw_fan_status(LCD_PIXEL_WIDTH - STATUS_CHR_WIDTH * 5, STATUS_FAN_Y);
  #endif

  // Axis values
  const xyz_pos_t lpos = current_position.asLogical();
  const bool show_e_total = TERN0(LCD_SHOW_E_TOTAL, printingIsActive()); UNUSED(show_e_total);

  constexpr int16_t cpy = TERN(DWIN_MARLINUI_PORTRAIT, 195, 117);
  if (show_e_total) {
    TERN_(LCD_SHOW_E_TOTAL, _draw_e_value(e_move_accumulator, TERN(DWIN_MARLINUI_PORTRAIT, 6, 75), cpy));
  }
  else {
                      _draw_axis_value(X_AXIS, ftostr4sign(lpos.x), blink, TERN(DWIN_MARLINUI_PORTRAIT,  6,  75), cpy);
    TERN_(HAS_Y_AXIS, _draw_axis_value(Y_AXIS, ftostr4sign(lpos.y), blink, TERN(DWIN_MARLINUI_PORTRAIT, 95, 184), cpy));
  }
  TERN_(HAS_Z_AXIS, _draw_axis_value(Z_AXIS, ftostr52sp(lpos.z), blink, TERN(DWIN_MARLINUI_PORTRAIT, 165, 300), cpy));

  // Feedrate
  static uint16_t old_fp = 0;
  if (!ui.did_first_redraw || old_fp != feedrate_percentage) {
    old_fp = feedrate_percentage;
    _draw_feedrate_status(i16tostr3rj(feedrate_percentage),
      #if ENABLED(DWIN_MARLINUI_PORTRAIT)
         5, 290
      #else
        14, 195
      #endif
    );
  }

  //
  // Elapsed time
  //
  char buffer[14];
  duration_t time;

  #if ENABLED(DWIN_MARLINUI_PORTRAIT)

    // Portrait mode only shows one value at a time, and will rotate if ROTATE_PROGRESS_DISPLAY
    dwin_string.set();
    char prefix = ' ';
    #if ENABLED(SHOW_REMAINING_TIME)
      if (TERN1(ROTATE_PROGRESS_DISPLAY, blink) && print_job_timer.isRunning()) {
        time = get_remaining_time();
        prefix = 'R';
      }
      else
    #endif
        time = print_job_timer.duration();

    time.toDigital(buffer);
    dwin_string.add(prefix);
    dwin_string.add(buffer);
    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, (LCD_PIXEL_WIDTH - ((dwin_string.length() + 1) * 14)), 290, S(dwin_string.string()));

  #else

    // landscape mode shows both elapsed and remaining (if SHOW_REMAINING_TIME)
    time = print_job_timer.duration();
    time.toDigital(buffer);
    dwin_string.set(" ");
    dwin_string.add(buffer);
    DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, 230, 170, S(dwin_string.string()));

    #if ENABLED(SHOW_REMAINING_TIME)
      if (print_job_timer.isRunning()) {
        time = get_remaining_time();
        DWIN_Draw_String(true, font14x28, Color_IconBlue, Color_Bg_Black, 336, 170, S(" R "));
        if (print_job_timer.isPaused() && blink)
          dwin_string.set("     ");
        else {
          time.toDigital(buffer);
          dwin_string.set(buffer);
        }
        DWIN_Draw_String(true, font14x28, Color_White, Color_Bg_Black, 378, 170, S(dwin_string.string()));
      }
      else if (!ui.did_first_redraw || ui.old_is_printing != print_job_timer.isRunning()) {
        dwin_string.set("        ");
        DWIN_Draw_String(true, font14x28, Color_IconBlue, Color_Bg_Black, 336, 170, S(dwin_string.string()));
      }
    #endif
  #endif

  //
  // Progress Bar
  //
  #if HAS_PRINT_PROGRESS
    constexpr int16_t pb_margin = 5,
                      pb_left = pb_margin + TERN(DWIN_MARLINUI_PORTRAIT, 0, 90),
                      pb_height = TERN(DWIN_MARLINUI_PORTRAIT, 60, 20),
                      pb_right = LCD_PIXEL_WIDTH - pb_margin,
                      pb_bottom = TERN(DWIN_MARLINUI_PORTRAIT, 410, 220),
                      pb_top = pb_bottom - pb_height,
                      pb_width = pb_right - pb_left;

    const progress_t progress = TERN(HAS_PRINT_PROGRESS_PERMYRIAD, get_progress_permyriad, get_progress_percent)();

    if (!ui.did_first_redraw)
      DWIN_Draw_Rectangle(0, Select_Color, pb_left, pb_top, pb_right, pb_bottom);   // Outline

    static uint16_t old_solid = 50;
    const uint16_t pb_solid = (pb_width - 2) * (progress / (PROGRESS_SCALE)) * 0.01f;
    const bool p_draw = !ui.did_first_redraw || old_solid != pb_solid;

    if (p_draw) {
      //if (pb_solid)
        DWIN_Draw_Rectangle(1, Select_Color, pb_left + 1, pb_top + 1, pb_left + pb_solid, pb_bottom - 1); // Fill the solid part

      //if (pb_solid < old_solid)
        DWIN_Draw_Rectangle(1, Color_Bg_Black, pb_left + 1 + pb_solid, pb_top + 1, pb_right - 1, pb_bottom - 1); // Erase the rest

      #if ENABLED(SHOW_SD_PERCENT)
        dwin_string.set(TERN(PRINT_PROGRESS_SHOW_DECIMALS, permyriadtostr4(progress), ui8tostr3rj(progress / (PROGRESS_SCALE))));
        dwin_string.add(PSTR("%"));
        DWIN_Draw_String(
          false, font16x32, Percent_Color, Color_Bg_Black,
          pb_left + (pb_width - dwin_string.length() * 16) / 2,
          pb_top + (pb_height - 32) / 2,
          S(dwin_string.string())
        );
      #endif

      old_solid = pb_solid;
    }
  #endif // HAS_PRINT_PROGRESS

  //
  // Status Message
  //
  draw_status_message(blink);

  ui.did_first_redraw = true;
  ui.old_is_printing = print_job_timer.isRunning();
}

#endif // IS_DWIN_MARLINUI
