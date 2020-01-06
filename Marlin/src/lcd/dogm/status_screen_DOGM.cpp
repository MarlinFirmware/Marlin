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
// status_screen_DOGM.cpp
// Standard Status Screen for Graphical Display
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_LCD && DISABLED(LIGHTWEIGHT_UI)

#include "dogm_Statusscreen.h"
#include "ultralcd_DOGM.h"
#include "../ultralcd.h"
#include "../lcdprint.h"
#include "../../libs/numtostr.h"

#include "../../module/motion.h"
#include "../../module/temperature.h"

#if ENABLED(FILAMENT_LCD_DISPLAY)
  #include "../../feature/filwidth.h"
  #include "../../module/planner.h"
  #include "../../gcode/parser.h"
#endif

#if HAS_CUTTER
  #include "../../feature/spindle_laser.h"
#endif

#if ENABLED(SDSUPPORT)
  #include "../../sd/cardreader.h"
#endif

#if HAS_PRINT_PROGRESS
  #include "../../module/printcounter.h"
#endif

#if DUAL_MIXING_EXTRUDER
  #include "../../feature/mixing.h"
#endif

#define X_LABEL_POS      3
#define X_VALUE_POS     11
#define XYZ_SPACING     37
#define XYZ_BASELINE    (30 + INFO_FONT_ASCENT)
#define EXTRAS_BASELINE (40 + INFO_FONT_ASCENT)
#define STATUS_BASELINE (LCD_PIXEL_HEIGHT - INFO_FONT_DESCENT)

#if ANIM_HBCC
  enum HeatBits : uint8_t {
    HEATBIT_HOTEND,
    HEATBIT_BED = HOTENDS,
    HEATBIT_CHAMBER,
    HEATBIT_CUTTER
  };
  IF<(HEATBIT_CUTTER > 7), uint16_t, uint8_t>::type heat_bits;
#endif

#if ANIM_HOTEND
  #define HOTEND_ALT(N) TEST(heat_bits, HEATBIT_HOTEND + N)
#else
  #define HOTEND_ALT(N) false
#endif
#if ANIM_BED
  #define BED_ALT() TEST(heat_bits, HEATBIT_BED)
#else
  #define BED_ALT() false
#endif
#if ANIM_CHAMBER
  #define CHAMBER_ALT() TEST(heat_bits, HEATBIT_CHAMBER)
#else
  #define CHAMBER_ALT() false
#endif
#if ANIM_CUTTER
  #define CUTTER_ALT(N) TEST(heat_bits, HEATBIT_CUTTER)
#else
  #define CUTTER_ALT() false
#endif

#if DO_DRAW_HOTENDS
  #define MAX_HOTEND_DRAW _MIN(HOTENDS, ((LCD_PIXEL_WIDTH - (STATUS_LOGO_BYTEWIDTH + STATUS_FAN_BYTEWIDTH) * 8) / (STATUS_HEATERS_XSPACE)))
  #define STATUS_HEATERS_BOT (STATUS_HEATERS_Y + STATUS_HEATERS_HEIGHT - 1)
#endif

#define PROGRESS_BAR_X 54
#define PROGRESS_BAR_WIDTH (LCD_PIXEL_WIDTH - PROGRESS_BAR_X)

FORCE_INLINE void _draw_centered_temp(const int16_t temp, const uint8_t tx, const uint8_t ty) {
  const char *str = i16tostr3(temp);
  const uint8_t len = str[0] != ' ' ? 3 : str[1] != ' ' ? 2 : 1;
  lcd_put_u8str(tx - len * (INFO_FONT_WIDTH) / 2 + 1, ty, &str[3-len]);
  lcd_put_wchar(LCD_STR_DEGREE[0]);
}

#if DO_DRAW_HOTENDS

  // Draw hotend bitmap with current and target temperatures
  FORCE_INLINE void _draw_hotend_status(const heater_ind_t heater, const bool blink) {
    #if !HEATER_IDLE_HANDLER
      UNUSED(blink);
    #endif

    const bool isHeat = HOTEND_ALT(heater);

    const uint8_t tx = STATUS_HOTEND_TEXT_X(heater);

    const float temp = thermalManager.degHotend(heater),
              target = thermalManager.degTargetHotend(heater);

    #if DISABLED(STATUS_HOTEND_ANIM)
      #define STATIC_HOTEND true
      #define HOTEND_DOT    isHeat
    #else
      #define STATIC_HOTEND false
      #define HOTEND_DOT    false
    #endif

    #if ANIM_HOTEND && BOTH(STATUS_HOTEND_INVERTED, STATUS_HOTEND_NUMBERLESS)
      #define OFF_BMP(N) status_hotend_b_bmp
      #define ON_BMP(N)  status_hotend_a_bmp
    #elif ANIM_HOTEND && DISABLED(STATUS_HOTEND_INVERTED) && ENABLED(STATUS_HOTEND_NUMBERLESS)
      #define OFF_BMP(N) status_hotend_a_bmp
      #define ON_BMP(N)  status_hotend_b_bmp
    #elif ANIM_HOTEND && ENABLED(STATUS_HOTEND_INVERTED)
      #define OFF_BMP(N) status_hotend##N##_b_bmp
      #define ON_BMP(N)  status_hotend##N##_a_bmp
    #else
      #define OFF_BMP(N) status_hotend##N##_a_bmp
      #define ON_BMP(N)  status_hotend##N##_b_bmp
    #endif

    #if STATUS_HOTEND_BITMAPS > 1
      static const unsigned char* const status_hotend_gfx[STATUS_HOTEND_BITMAPS] PROGMEM = ARRAY_N(STATUS_HOTEND_BITMAPS, OFF_BMP(1), OFF_BMP(2), OFF_BMP(3), OFF_BMP(4), OFF_BMP(5), OFF_BMP(6));
      #if ANIM_HOTEND
        static const unsigned char* const status_hotend_on_gfx[STATUS_HOTEND_BITMAPS] PROGMEM = ARRAY_N(STATUS_HOTEND_BITMAPS, ON_BMP(1), ON_BMP(2), ON_BMP(3), ON_BMP(4), ON_BMP(5), ON_BMP(6));
        #define HOTEND_BITMAP(N,S) (unsigned char*)pgm_read_ptr((S) ? &status_hotend_on_gfx[(N) % (STATUS_HOTEND_BITMAPS)] : &status_hotend_gfx[(N) % (STATUS_HOTEND_BITMAPS)])
      #else
        #define HOTEND_BITMAP(N,S) (unsigned char*)pgm_read_ptr(&status_hotend_gfx[(N) % (STATUS_HOTEND_BITMAPS)])
      #endif
    #elif ANIM_HOTEND
      #define HOTEND_BITMAP(N,S) ((S) ? ON_BMP() : OFF_BMP())
    #else
      #define HOTEND_BITMAP(N,S) status_hotend_a_bmp
    #endif

    if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_BOT)) {

      #define BAR_TALL (STATUS_HEATERS_HEIGHT - 2)

      const float prop = target - 20,
                  perc = prop > 0 && temp >= 20 ? (temp - 20) / prop : 0;
      uint8_t tall = uint8_t(perc * BAR_TALL + 0.5f);
      NOMORE(tall, BAR_TALL);

      #if ANIM_HOTEND
        // Draw hotend bitmap, either whole or split by the heating percent
        const uint8_t hx = STATUS_HOTEND_X(heater),
                      bw = STATUS_HOTEND_BYTEWIDTH(heater);
        #if ENABLED(STATUS_HEAT_PERCENT)
          if (isHeat && tall <= BAR_TALL) {
            const uint8_t ph = STATUS_HEATERS_HEIGHT - 1 - tall;
            u8g.drawBitmapP(hx, STATUS_HEATERS_Y, bw, ph, HOTEND_BITMAP(heater, false));
            u8g.drawBitmapP(hx, STATUS_HEATERS_Y + ph, bw, tall + 1, HOTEND_BITMAP(heater, true) + ph * bw);
          }
          else
        #endif
            u8g.drawBitmapP(hx, STATUS_HEATERS_Y, bw, STATUS_HEATERS_HEIGHT, HOTEND_BITMAP(heater, isHeat));
      #endif

      // Draw a heating progress bar, if specified
      #if DO_DRAW_BED && ENABLED(STATUS_HEAT_PERCENT)

        if (STATIC_HOTEND && isHeat) {
          const uint8_t bx = STATUS_HOTEND_X(heater) + STATUS_HOTEND_WIDTH(heater) + 1;
          u8g.drawFrame(bx, STATUS_HEATERS_Y, 3, STATUS_HEATERS_HEIGHT);
          if (tall) {
            const uint8_t ph = STATUS_HEATERS_HEIGHT - 1 - tall;
            if (PAGE_OVER(STATUS_HEATERS_Y + ph))
              u8g.drawVLine(bx + 1, STATUS_HEATERS_Y + ph, tall);
          }
        }

      #endif

    } // PAGE_CONTAINS

    if (PAGE_UNDER(7)) {
      #if HEATER_IDLE_HANDLER
        const bool dodraw = (blink || !thermalManager.hotend_idle[heater].timed_out);
      #else
        constexpr bool dodraw = true;
      #endif
      if (dodraw) _draw_centered_temp(target + 0.5, tx, 7);
    }

    if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
      _draw_centered_temp(temp + 0.5f, tx, 28);

    if (STATIC_HOTEND && HOTEND_DOT && PAGE_CONTAINS(17, 19)) {
      u8g.setColorIndex(0); // set to white on black
      u8g.drawBox(tx, 20 - 3, 2, 2);
      u8g.setColorIndex(1); // restore black on white
    }

  }

#endif // DO_DRAW_HOTENDS

#if DO_DRAW_BED

  // Draw bed bitmap with current and target temperatures
  FORCE_INLINE void _draw_bed_status(const bool blink) {
    #if !HEATER_IDLE_HANDLER
      UNUSED(blink);
    #endif

    const uint8_t tx = STATUS_BED_TEXT_X;

    const float temp = thermalManager.degBed(),
              target = thermalManager.degTargetBed();

    #if ENABLED(STATUS_HEAT_PERCENT) || (DO_DRAW_BED && DISABLED(STATUS_BED_ANIM))
      const bool isHeat = BED_ALT();
    #endif

    #if DO_DRAW_BED && DISABLED(STATUS_BED_ANIM)
      #define STATIC_BED    true
      #define BED_DOT       isHeat
    #else
      #define STATIC_BED    false
      #define BED_DOT       false
    #endif

    if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_BOT)) {

      #define BAR_TALL (STATUS_HEATERS_HEIGHT - 2)

      const float prop = target - 20,
                  perc = prop > 0 && temp >= 20 ? (temp - 20) / prop : 0;
      uint8_t tall = uint8_t(perc * BAR_TALL + 0.5f);
      NOMORE(tall, BAR_TALL);

      // Draw a heating progress bar, if specified
      #if ENABLED(STATUS_HEAT_PERCENT)

        if (isHeat) {
          const uint8_t bx = STATUS_BED_X + STATUS_BED_WIDTH;
          u8g.drawFrame(bx, STATUS_HEATERS_Y, 3, STATUS_HEATERS_HEIGHT);
          if (tall) {
            const uint8_t ph = STATUS_HEATERS_HEIGHT - 1 - tall;
            if (PAGE_OVER(STATUS_HEATERS_Y + ph))
              u8g.drawVLine(bx + 1, STATUS_HEATERS_Y + ph, tall);
          }
        }

      #endif

    } // PAGE_CONTAINS

    if (PAGE_UNDER(7)) {
      #if HEATER_IDLE_HANDLER
        const bool dodraw = (blink || !thermalManager.bed_idle.timed_out);
      #else
        constexpr bool dodraw = true;
      #endif
      if (dodraw) _draw_centered_temp(target + 0.5, tx, 7);
    }

    if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
      _draw_centered_temp(temp + 0.5f, tx, 28);

    if (STATIC_BED && BED_DOT && PAGE_CONTAINS(17, 19)) {
      u8g.setColorIndex(0); // set to white on black
      u8g.drawBox(tx, 20 - 2, 2, 2);
      u8g.setColorIndex(1); // restore black on white
    }

  }

#endif // DO_DRAW_BED

#if DO_DRAW_CHAMBER

  FORCE_INLINE void _draw_chamber_status() {
    #if HAS_HEATED_CHAMBER
      if (PAGE_UNDER(7))
        _draw_centered_temp(thermalManager.degTargetChamber() + 0.5f, STATUS_CHAMBER_TEXT_X, 7);
    #endif

    if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
      _draw_centered_temp(thermalManager.degChamber() + 0.5f, STATUS_CHAMBER_TEXT_X, 28);
  }

#endif // DO_DRAW_CHAMBER

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink) {
  const AxisEnum a = (
    #if ENABLED(LCD_SHOW_E_TOTAL)
      axis == E_AXIS ? X_AXIS :
    #endif
    axis
  );
  const uint8_t offs = (XYZ_SPACING) * a;
  lcd_put_wchar(X_LABEL_POS + offs, XYZ_BASELINE, axis_codes[axis]);
  lcd_moveto(X_VALUE_POS + offs, XYZ_BASELINE);
  if (blink)
    lcd_put_u8str(value);
  else {
    if (!TEST(axis_homed, axis))
      while (const char c = *value++) lcd_put_wchar(c <= '.' ? c : '?');
    else {
      #if NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING)
        if (!TEST(axis_known_position, axis))
          lcd_put_u8str_P(axis == Z_AXIS ? PSTR("       ") : PSTR("    "));
        else
      #endif
          lcd_put_u8str(value);
    }
  }
}

void MarlinUI::draw_status_screen() {

  static char xstring[5
    #if ENABLED(LCD_SHOW_E_TOTAL)
      + 7
    #endif
  ], ystring[5], zstring[8];
  #if ENABLED(FILAMENT_LCD_DISPLAY)
    static char wstring[5], mstring[4];
  #endif

  #if HAS_PRINT_PROGRESS
    #if DISABLED(DOGM_SD_PERCENT)
      #define _SD_INFO_X(len) (PROGRESS_BAR_X + (PROGRESS_BAR_WIDTH) / 2 - (len) * (MENU_FONT_WIDTH) / 2)
    #else
      #define _SD_INFO_X(len) (LCD_PIXEL_WIDTH - (len) * (MENU_FONT_WIDTH))
    #endif

    #if ENABLED(DOGM_SD_PERCENT)
      static char progress_string[5];
    #endif
    static uint8_t lastElapsed = 0xFF, lastProgress = 0xFF;
    static u8g_uint_t elapsed_x_pos = 0, progress_bar_solid_width = 0;
    static char elapsed_string[16];
    #if ENABLED(SHOW_REMAINING_TIME)
      static u8g_uint_t estimation_x_pos = 0;
      static char estimation_string[10];
      #if BOTH(DOGM_SD_PERCENT, ROTATE_PROGRESS_DISPLAY)
        static u8g_uint_t progress_x_pos = 0;
        static uint8_t progress_state = 0;
        static bool prev_blink = 0;
      #endif
    #endif
  #endif

  const bool showxy = (true
    #if ENABLED(LCD_SHOW_E_TOTAL)
      && !printingIsActive()
    #endif
  );

  // At the first page, generate new display values
  if (first_page) {
    #if ANIM_HBCC
      uint8_t new_bits = 0;
      #if ANIM_HOTEND
        HOTEND_LOOP() if (thermalManager.isHeatingHotend(e)) SBI(new_bits, HEATBIT_HOTEND + e);
      #endif
      #if ANIM_BED
        if (thermalManager.isHeatingBed()) SBI(new_bits, HEATBIT_BED);
      #endif
      #if DO_DRAW_CHAMBER && HAS_HEATED_CHAMBER
        if (thermalManager.isHeatingChamber()) SBI(new_bits, HEATBIT_CHAMBER);
      #endif
      #if ANIM_CUTTER
        if (cutter.enabled()) SBI(new_bits, HEATBIT_CUTTER);
      #endif
      heat_bits = new_bits;
    #endif

    const xyz_pos_t lpos = current_position.asLogical();
    strcpy(zstring, ftostr52sp(lpos.z));

    if (showxy) {
      strcpy(xstring, ftostr4sign(lpos.x));
      strcpy(ystring, ftostr4sign(lpos.y));
    }
    else {
      #if ENABLED(LCD_SHOW_E_TOTAL)
        const uint8_t escale = e_move_accumulator >= 100000.0f ? 10 : 1; // After 100m switch to cm
        sprintf_P(xstring, PSTR("%ld%cm"), uint32_t(_MAX(e_move_accumulator, 0.0f)) / escale, escale == 10 ? 'c' : 'm'); // 1234567mm
      #endif
    }

    #if ENABLED(FILAMENT_LCD_DISPLAY)
      strcpy(wstring, ftostr12ns(filwidth.measured_mm));
      strcpy(mstring, i16tostr3(planner.volumetric_percent(parser.volumetric_enabled)));
    #endif

    // Progress / elapsed / estimation updates and string formatting to avoid float math on each LCD draw
    #if HAS_PRINT_PROGRESS
      const progress_t progress =
        #if HAS_PRINT_PROGRESS_PERMYRIAD
          get_progress_permyriad()
        #else
          get_progress_percent()
        #endif
      ;
      duration_t elapsed = print_job_timer.duration();
      const uint8_t p = progress & 0xFF, ev = elapsed.value & 0xFF;
      if (p != lastProgress) {
        lastProgress = p;

        progress_bar_solid_width = u8g_uint_t((PROGRESS_BAR_WIDTH - 2) * (progress / (PROGRESS_SCALE)) * 0.01f);

        #if ENABLED(DOGM_SD_PERCENT)
          if (progress == 0) {
            progress_string[0] = '\0';
            #if ENABLED(SHOW_REMAINING_TIME)
              estimation_string[0] = '\0';
              estimation_x_pos = _SD_INFO_X(0);
            #endif
          }
          else {
            strcpy(progress_string, (
              #if ENABLED(PRINT_PROGRESS_SHOW_DECIMALS)
                permyriadtostr4(progress)
              #else
                ui8tostr3(progress / (PROGRESS_SCALE))
              #endif
            ));
          }
          #if BOTH(SHOW_REMAINING_TIME, ROTATE_PROGRESS_DISPLAY) // Tri-state progress display mode
            progress_x_pos = _SD_INFO_X(strlen(progress_string) + 1);
          #endif
        #endif
      }

      if (ev != lastElapsed) {
        lastElapsed = ev;
        const bool has_days = (elapsed.value >= 60*60*24L);
        const uint8_t len = elapsed.toDigital(elapsed_string, has_days);
        elapsed_x_pos = _SD_INFO_X(len);

        #if ENABLED(SHOW_REMAINING_TIME)
          if (!(ev & 0x3)) {
            uint32_t timeval = (0
              #if BOTH(LCD_SET_PROGRESS_MANUALLY, USE_M73_REMAINING_TIME)
                + get_remaining_time()
              #endif
            );
            if (!timeval && progress > 0) timeval = elapsed.value * (100 * (PROGRESS_SCALE) - progress) / progress;
            if (!timeval) {
              estimation_string[0] = '\0';
              estimation_x_pos = _SD_INFO_X(0);
            }
            else {
              duration_t estimation = timeval;
              const bool has_days = (estimation.value >= 60*60*24L);
              const uint8_t len = estimation.toDigital(estimation_string, has_days);
              estimation_x_pos = _SD_INFO_X(len
                #if !BOTH(DOGM_SD_PERCENT, ROTATE_PROGRESS_DISPLAY)
                  + 1
                #endif
              );
            }
          }
        #endif
      }
    #endif
  }

  const bool blink = get_blink();

  // Status Menu Font
  set_font(FONT_STATUSMENU);

  #if DO_DRAW_LOGO
    if (PAGE_CONTAINS(STATUS_LOGO_Y, STATUS_LOGO_Y + STATUS_LOGO_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_LOGO_X, STATUS_LOGO_Y, STATUS_LOGO_BYTEWIDTH, STATUS_LOGO_HEIGHT, status_logo_bmp);
  #endif

  #if STATUS_HEATERS_WIDTH
    // Draw all heaters (and maybe the bed) in one go
    if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_Y + STATUS_HEATERS_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_HEATERS_X, STATUS_HEATERS_Y, STATUS_HEATERS_BYTEWIDTH, STATUS_HEATERS_HEIGHT, status_heaters_bmp);
  #endif

  #if DO_DRAW_CUTTER
    #if ANIM_CUTTER
      #define CUTTER_BITMAP(S) ((S) ? status_cutter_on_bmp : status_cutter_bmp)
    #else
      #define CUTTER_BITMAP(S) status_cutter_bmp
    #endif
    const uint8_t cuttery = STATUS_CUTTER_Y(CUTTER_ALT()),
                  cutterh = STATUS_CUTTER_HEIGHT(CUTTER_ALT());
    if (PAGE_CONTAINS(cuttery, cuttery + cutterh - 1))
      u8g.drawBitmapP(STATUS_CUTTER_X, cuttery, STATUS_CUTTER_BYTEWIDTH, cutterh, CUTTER_BITMAP(CUTTER_ALT()));
  #endif

  #if DO_DRAW_BED && DISABLED(STATUS_COMBINE_HEATERS)
    #if ANIM_BED
      #define BED_BITMAP(S) ((S) ? status_bed_on_bmp : status_bed_bmp)
    #else
      #define BED_BITMAP(S) status_bed_bmp
    #endif
    const uint8_t bedy = STATUS_BED_Y(BED_ALT()),
                  bedh = STATUS_BED_HEIGHT(BED_ALT());
    if (PAGE_CONTAINS(bedy, bedy + bedh - 1))
      u8g.drawBitmapP(STATUS_BED_X, bedy, STATUS_BED_BYTEWIDTH, bedh, BED_BITMAP(BED_ALT()));
  #endif

  #if DO_DRAW_CHAMBER
    #if ANIM_CHAMBER
      #define CHAMBER_BITMAP(S) ((S) ? status_chamber_on_bmp : status_chamber_bmp)
    #else
      #define CHAMBER_BITMAP(S) status_chamber_bmp
    #endif
    const uint8_t chambery = STATUS_CHAMBER_Y(CHAMBER_ALT()),
                  chamberh = STATUS_CHAMBER_HEIGHT(CHAMBER_ALT());
    if (PAGE_CONTAINS(chambery, chambery + chamberh - 1))
      u8g.drawBitmapP(STATUS_CHAMBER_X, chambery, STATUS_CHAMBER_BYTEWIDTH, chamberh, CHAMBER_BITMAP(CHAMBER_ALT()));
  #endif

  #if DO_DRAW_FAN
    #if STATUS_FAN_FRAMES > 2
      static bool old_blink;
      static uint8_t fan_frame;
      if (old_blink != blink) {
        old_blink = blink;
        if (!thermalManager.fan_speed[0] || ++fan_frame >= STATUS_FAN_FRAMES) fan_frame = 0;
      }
    #endif
    if (PAGE_CONTAINS(STATUS_FAN_Y, STATUS_FAN_Y + STATUS_FAN_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_FAN_X, STATUS_FAN_Y, STATUS_FAN_BYTEWIDTH, STATUS_FAN_HEIGHT,
        #if STATUS_FAN_FRAMES > 2
          fan_frame == 1 ? status_fan1_bmp :
          fan_frame == 2 ? status_fan2_bmp :
          #if STATUS_FAN_FRAMES > 3
            fan_frame == 3 ? status_fan3_bmp :
          #endif
        #elif STATUS_FAN_FRAMES > 1
          blink && thermalManager.fan_speed[0] ? status_fan1_bmp :
        #endif
        status_fan0_bmp
      );
  #endif

  //
  // Temperature Graphics and Info
  //
  if (PAGE_UNDER(6 + 1 + 12 + 1 + 6 + 1)) {
    // Extruders
    #if DO_DRAW_HOTENDS
      for (uint8_t e = 0; e < MAX_HOTEND_DRAW; ++e)
        _draw_hotend_status((heater_ind_t)e, blink);
    #endif

    // Laser / Spindle
    #if DO_DRAW_CUTTER
      if (cutter.power && PAGE_CONTAINS(STATUS_CUTTER_TEXT_Y - INFO_FONT_ASCENT, STATUS_CUTTER_TEXT_Y - 1)) {
        lcd_put_u8str(STATUS_CUTTER_TEXT_X, STATUS_CUTTER_TEXT_Y, i16tostr3(cutter.powerPercent(cutter.power)));
        lcd_put_wchar('%');
      }
    #endif

    // Heated Bed
    #if DO_DRAW_BED
      _draw_bed_status(blink);
    #endif

    // Heated Chamber
    #if DO_DRAW_CHAMBER
      _draw_chamber_status();
    #endif

    // Fan, if a bitmap was provided
    #if DO_DRAW_FAN
      if (PAGE_CONTAINS(STATUS_FAN_TEXT_Y - INFO_FONT_ASCENT, STATUS_FAN_TEXT_Y - 1)) {
        char c = '%';
        uint16_t spd = thermalManager.fan_speed[0];
        if (spd) {
          #if ENABLED(ADAPTIVE_FAN_SLOWING)
            if (!blink && thermalManager.fan_speed_scaler[0] < 128) {
              spd = thermalManager.scaledFanSpeed(0, spd);
              c = '*';
            }
          #endif
          lcd_put_u8str(STATUS_FAN_TEXT_X, STATUS_FAN_TEXT_Y, i16tostr3(thermalManager.fanPercent(spd)));
          lcd_put_wchar(c);
        }
      }
    #endif
  }

  #if ENABLED(SDSUPPORT)
    //
    // SD Card Symbol
    //
    if (card.isFileOpen() && PAGE_CONTAINS(42, 51)) {
      // Upper box
      u8g.drawBox(42, 42, 8, 7);     // 42-48 (or 41-47)
      // Right edge
      u8g.drawBox(50, 44, 2, 5);     // 44-48 (or 43-47)
      // Bottom hollow box
      u8g.drawFrame(42, 49, 10, 4);  // 49-52 (or 48-51)
      // Corner pixel
      u8g.drawPixel(50, 43);         // 43 (or 42)
    }
  #endif // SDSUPPORT

  #if HAS_PRINT_PROGRESS
    //
    // Progress bar frame
    //

    if (PAGE_CONTAINS(49, 52))
      u8g.drawFrame(PROGRESS_BAR_X, 49, PROGRESS_BAR_WIDTH, 4);

    //
    // Progress bar solid part
    //

    if (PAGE_CONTAINS(50, 51))     // 50-51 (or just 50)
      u8g.drawBox(PROGRESS_BAR_X + 1, 50, progress_bar_solid_width, 2);

    if (PAGE_CONTAINS(EXTRAS_BASELINE - INFO_FONT_ASCENT, EXTRAS_BASELINE - 1)) {

      #if ALL(DOGM_SD_PERCENT, SHOW_REMAINING_TIME, ROTATE_PROGRESS_DISPLAY)

        if (prev_blink != blink) {
          prev_blink = blink;
          if (++progress_state >= 3) progress_state = 0;
        }

        if (progress_state == 0) {
          if (progress_string[0]) {
            lcd_put_u8str(progress_x_pos, EXTRAS_BASELINE, progress_string);
            lcd_put_wchar('%');
          }
        }
        else if (progress_state == 2 && estimation_string[0]) {
          lcd_put_u8str(PROGRESS_BAR_X, EXTRAS_BASELINE, "R:");
          lcd_put_u8str(estimation_x_pos, EXTRAS_BASELINE, estimation_string);
        }
        else if (elapsed_string[0]) {
          lcd_put_u8str(PROGRESS_BAR_X, EXTRAS_BASELINE, "E:");
          lcd_put_u8str(elapsed_x_pos, EXTRAS_BASELINE, elapsed_string);
        }

      #else // !DOGM_SD_PERCENT || !SHOW_REMAINING_TIME || !ROTATE_PROGRESS_DISPLAY

        //
        // SD Percent Complete
        //

        #if ENABLED(DOGM_SD_PERCENT)
          if (progress_string[0]) {
            lcd_put_u8str(55, 48, progress_string); // Percent complete
            lcd_put_wchar('%');
          }
        #endif

        //
        // Elapsed Time
        //

        #if ENABLED(SHOW_REMAINING_TIME)
          if (blink && estimation_string[0]) {
            lcd_put_wchar(estimation_x_pos, EXTRAS_BASELINE, 'R');
            lcd_put_u8str(estimation_string);
          }
          else
        #endif
            lcd_put_u8str(elapsed_x_pos, EXTRAS_BASELINE, elapsed_string);

      #endif // !DOGM_SD_PERCENT || !SHOW_REMAINING_TIME || !ROTATE_PROGRESS_DISPLAY
    }

  #endif // HAS_PRINT_PROGRESS

  //
  // XYZ Coordinates
  //

  #if ENABLED(XYZ_HOLLOW_FRAME)
    #define XYZ_FRAME_TOP 29
    #define XYZ_FRAME_HEIGHT INFO_FONT_ASCENT + 3
  #else
    #define XYZ_FRAME_TOP 30
    #define XYZ_FRAME_HEIGHT INFO_FONT_ASCENT + 1
  #endif

  if (PAGE_CONTAINS(XYZ_FRAME_TOP, XYZ_FRAME_TOP + XYZ_FRAME_HEIGHT - 1)) {

    #if ENABLED(XYZ_HOLLOW_FRAME)
      u8g.drawFrame(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT); // 8: 29-40  7: 29-39
    #else
      u8g.drawBox(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT);   // 8: 30-39  7: 30-37
    #endif

    if (PAGE_CONTAINS(XYZ_BASELINE - (INFO_FONT_ASCENT - 1), XYZ_BASELINE)) {

      #if DISABLED(XYZ_HOLLOW_FRAME)
        u8g.setColorIndex(0); // white on black
      #endif

      #if DUAL_MIXING_EXTRUDER

        // Two-component mix / gradient instead of XY

        char mixer_messages[12];
        const char *mix_label;
        #if ENABLED(GRADIENT_MIX)
          if (mixer.gradient.enabled) {
            mixer.update_mix_from_gradient();
            mix_label = "Gr";
          }
          else
        #endif
          {
            mixer.update_mix_from_vtool();
            mix_label = "Mx";
          }
        sprintf_P(mixer_messages, PSTR("%s %d;%d%% "), mix_label, int(mixer.mix[0]), int(mixer.mix[1]));
        lcd_put_u8str(X_LABEL_POS, XYZ_BASELINE, mixer_messages);

      #else

        if (showxy) {
          _draw_axis_value(X_AXIS, xstring, blink);
          _draw_axis_value(Y_AXIS, ystring, blink);
        }
        else {
          _draw_axis_value(E_AXIS, xstring, true);
          lcd_put_u8str_P(PSTR("       "));
        }

      #endif

      _draw_axis_value(Z_AXIS, zstring, blink);

      #if DISABLED(XYZ_HOLLOW_FRAME)
        u8g.setColorIndex(1); // black on white
      #endif
    }
  }

  //
  // Feedrate
  //
  #define EXTRAS_2_BASELINE (EXTRAS_BASELINE + 3)

  if (PAGE_CONTAINS(EXTRAS_2_BASELINE - INFO_FONT_ASCENT, EXTRAS_2_BASELINE - 1)) {
    set_font(FONT_MENU);
    lcd_put_wchar(3, EXTRAS_2_BASELINE, LCD_STR_FEEDRATE[0]);

    set_font(FONT_STATUSMENU);
    lcd_put_u8str(12, EXTRAS_2_BASELINE, i16tostr3(feedrate_percentage));
    lcd_put_wchar('%');

    //
    // Filament sensor display if SD is disabled
    //
    #if ENABLED(FILAMENT_LCD_DISPLAY) && DISABLED(SDSUPPORT)
      lcd_put_u8str(56, EXTRAS_2_BASELINE, wstring);
      lcd_put_u8str(102, EXTRAS_2_BASELINE, mstring);
      lcd_put_wchar('%');
      set_font(FONT_MENU);
      lcd_put_wchar(47, EXTRAS_2_BASELINE, LCD_STR_FILAM_DIA[0]); // lcd_put_u8str_P(PSTR(LCD_STR_FILAM_DIA));
      lcd_put_wchar(93, EXTRAS_2_BASELINE, LCD_STR_FILAM_MUL[0]);
    #endif
  }

  //
  // Status line
  //
  if (PAGE_CONTAINS(STATUS_BASELINE - INFO_FONT_ASCENT, STATUS_BASELINE + INFO_FONT_DESCENT)) {
    lcd_moveto(0, STATUS_BASELINE);

    #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
      // Alternate Status message and Filament display
      if (ELAPSED(millis(), next_filament_display)) {
        lcd_put_u8str_P(PSTR(LCD_STR_FILAM_DIA));
        lcd_put_wchar(':');
        lcd_put_u8str(wstring);
        lcd_put_u8str_P(PSTR("  " LCD_STR_FILAM_MUL));
        lcd_put_wchar(':');
        lcd_put_u8str(mstring);
        lcd_put_wchar('%');
      }
      else
    #endif
        draw_status_message(blink);
  }
}

void MarlinUI::draw_status_message(const bool blink) {

  // Get the UTF8 character count of the string
  uint8_t slen = utf8_strlen(status_message);

  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    static bool last_blink = false;

    if (slen <= LCD_WIDTH) {
      // The string fits within the line. Print with no scrolling
      lcd_put_u8str(status_message);
      while (slen < LCD_WIDTH) { lcd_put_wchar(' '); ++slen; }
    }
    else {
      // String is longer than the available space

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
      const char *stat = status_and_len(rlen);
      lcd_put_u8str_max(stat, LCD_PIXEL_WIDTH);

      // If the remaining string doesn't completely fill the screen
      if (rlen < LCD_WIDTH) {
        lcd_put_wchar('.');                     // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;       // Amount of space left in characters
        if (--chars) {                          // Draw a second dot if there's space
          lcd_put_wchar('.');
          if (--chars) {                        // Print a second copy of the message
            lcd_put_u8str_max(status_message, LCD_PIXEL_WIDTH - (rlen + 2) * (MENU_FONT_WIDTH));
            lcd_put_wchar(' ');
          }
        }
      }
      if (last_blink != blink) {
        last_blink = blink;
        advance_status_scroll();
      }
    }

  #else // !STATUS_MESSAGE_SCROLLING

    UNUSED(blink);

    // Just print the string to the LCD
    lcd_put_u8str_max(status_message, LCD_PIXEL_WIDTH);

    // Fill the rest with spaces
    for (; slen < LCD_WIDTH; ++slen) lcd_put_wchar(' ');

  #endif // !STATUS_MESSAGE_SCROLLING
}

#endif // HAS_GRAPHICAL_LCD && !LIGHTWEIGHT_UI
