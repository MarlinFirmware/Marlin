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

#if ENABLED(SDSUPPORT)
  #include "../../sd/cardreader.h"
#endif

#if HAS_PRINT_PROGRESS
  #include "../../module/printcounter.h"
#endif

#if DUAL_MIXING_EXTRUDER
  #include "../../feature/mixing.h"
#endif

FORCE_INLINE void _draw_centered_temp(const int16_t temp, const uint8_t tx, const uint8_t ty) {
  const char *str = i16tostr3(temp);
  const uint8_t len = str[0] != ' ' ? 3 : str[1] != ' ' ? 2 : 1;
  lcd_moveto(tx - len * (INFO_FONT_WIDTH) / 2 + 1, ty);
  lcd_put_u8str(&str[3-len]);
  lcd_put_wchar(LCD_STR_DEGREE[0]);
}

#define X_LABEL_POS      3
#define X_VALUE_POS     11
#define XYZ_SPACING     37
#define XYZ_BASELINE    (30 + INFO_FONT_ASCENT)
#define EXTRAS_BASELINE (40 + INFO_FONT_ASCENT)
#define STATUS_BASELINE (LCD_PIXEL_HEIGHT - INFO_FONT_DESCENT)

#define DO_DRAW_BED (HAS_HEATED_BED && STATUS_BED_WIDTH && HOTENDS <= 3 && DISABLED(STATUS_COMBINE_HEATERS))
#define DO_DRAW_FAN (HAS_FAN0 && STATUS_FAN_WIDTH && STATUS_FAN_FRAMES)
#define ANIM_HOTEND (HOTENDS && ENABLED(STATUS_HOTEND_ANIM))
#define ANIM_BED (DO_DRAW_BED && ENABLED(STATUS_BED_ANIM))
#define ANIM_CHAMBER (HAS_HEATED_CHAMBER && ENABLED(STATUS_CHAMBER_ANIM))

#if ANIM_HOTEND || ANIM_BED
  uint8_t heat_bits;
#endif
#if ANIM_HOTEND
  #define HOTEND_ALT(N) TEST(heat_bits, N)
#else
  #define HOTEND_ALT(N) false
#endif
#if ANIM_BED
  #define BED_ALT() TEST(heat_bits, 7)
#else
  #define BED_ALT() false
#endif
#if ANIM_CHAMBER
  #define CHAMBER_ALT() TEST(heat_bits, 6)
#else
  #define CHAMBER_ALT() false
#endif

#define MAX_HOTEND_DRAW MIN(HOTENDS, ((LCD_PIXEL_WIDTH - (STATUS_LOGO_BYTEWIDTH + STATUS_FAN_BYTEWIDTH) * 8) / (STATUS_HEATERS_XSPACE)))
#define STATUS_HEATERS_BOT (STATUS_HEATERS_Y + STATUS_HEATERS_HEIGHT - 1)

#if ENABLED(MARLIN_DEV_MODE)
  #define SHOW_ON_STATE READ(X_MIN_PIN)
#else
  #define SHOW_ON_STATE false
#endif

FORCE_INLINE void _draw_heater_status(const int8_t heater, const bool blink) {
  #if !HEATER_IDLE_HANDLER
    UNUSED(blink);
  #endif

  #if HAS_HEATED_BED
    const bool isBed = heater < 0;
    #define IFBED(A,B) (isBed ? (A) : (B))
  #else
    #define IFBED(A,B) (B)
  #endif

  #if ENABLED(MARLIN_DEV_MODE)
    constexpr bool isHeat = true;
  #else
    const bool isHeat = IFBED(BED_ALT(), HOTEND_ALT(heater));
  #endif

  const uint8_t tx = IFBED(STATUS_BED_TEXT_X, STATUS_HOTEND_TEXT_X(heater));

  #if ENABLED(MARLIN_DEV_MODE)
    const float temp = 20 + (millis() >> 8) % IFBED(100, 200);
    const float target = IFBED(100, 200);
  #else
    const float temp = IFBED(thermalManager.degBed(), thermalManager.degHotend(heater)),
                target = IFBED(thermalManager.degTargetBed(), thermalManager.degTargetHotend(heater));
  #endif

  #if HAS_HEATED_CHAMBER
    FORCE_INLINE void _draw_chamber_status(const bool blink) {
      const float temp = thermalManager.degChamber(),
                  target = thermalManager.degTargetChamber();
      #if !HEATER_IDLE_HANDLER
        UNUSED(blink);
      #endif
      if (PAGE_UNDER(7)) {
        #if HEATER_IDLE_HANDLER
          const bool is_idle = false, // thermalManager.chamber_idle.timed_out,
                     dodraw = (blink || !is_idle);
        #else
          constexpr bool dodraw = true;
        #endif
        if (dodraw) _draw_centered_temp(target + 0.5, STATUS_CHAMBER_TEXT_X, 7);
      }
      if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
        _draw_centered_temp(temp + 0.5f, STATUS_CHAMBER_TEXT_X, 28);
    }
  #endif

  #if DISABLED(STATUS_HOTEND_ANIM)
    #define STATIC_HOTEND true
    #define HOTEND_DOT    isHeat
  #else
    #define STATIC_HOTEND false
    #define HOTEND_DOT    false
  #endif

  #if HAS_HEATED_BED && DISABLED(STATUS_BED_ANIM)
    #define STATIC_BED    true
    #define BED_DOT       isHeat
  #else
    #define STATIC_BED    false
    #define BED_DOT       false
  #endif

  #if ANIM_HOTEND && ENABLED(STATUS_HOTEND_INVERTED)
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

    #ifdef STATUS_HOTEND_ANIM
      // Draw hotend bitmap, either whole or split by the heating percent
      if (IFBED(0, 1)) {
        const uint8_t hx = STATUS_HOTEND_X(heater), bw = STATUS_HOTEND_BYTEWIDTH(heater);
        #if ENABLED(STATUS_HEAT_PERCENT)
          if (isHeat && tall <= BAR_TALL) {
            const uint8_t ph = STATUS_HEATERS_HEIGHT - 1 - tall;
            u8g.drawBitmapP(hx, STATUS_HEATERS_Y, bw, ph, HOTEND_BITMAP(heater, false));
            u8g.drawBitmapP(hx, STATUS_HEATERS_Y + ph, bw, tall + 1, HOTEND_BITMAP(heater, true) + ph * bw);
          }
          else
        #endif
            u8g.drawBitmapP(hx, STATUS_HEATERS_Y, bw, STATUS_HEATERS_HEIGHT, HOTEND_BITMAP(heater, isHeat));
      }
    #endif

    // Draw a heating progress bar, if specified
    #if ENABLED(STATUS_HEAT_PERCENT)

      if (IFBED(true, STATIC_HOTEND) && isHeat) {
        const uint8_t bx = IFBED(STATUS_BED_X + STATUS_BED_WIDTH, STATUS_HOTEND_X(heater) + STATUS_HOTEND_WIDTH(heater)) + 1;
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
      const bool is_idle = IFBED(thermalManager.bed_idle.timed_out, thermalManager.hotend_idle[heater].timed_out),
                 dodraw = (blink || !is_idle);
    #else
      constexpr bool dodraw = true;
    #endif
    if (dodraw) _draw_centered_temp(target + 0.5, tx, 7);
  }

  if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
    _draw_centered_temp(temp + 0.5f, tx, 28);

  if (IFBED(STATIC_BED && BED_DOT, STATIC_HOTEND && HOTEND_DOT) && PAGE_CONTAINS(17, 19)) {
    u8g.setColorIndex(0); // set to white on black
    u8g.drawBox(tx, IFBED(20-2, 20-3), 2, 2);
    u8g.setColorIndex(1); // restore black on white
  }

}

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink) {
  const uint8_t offs = (XYZ_SPACING) * axis;
  lcd_moveto(X_LABEL_POS + offs, XYZ_BASELINE);
  lcd_put_wchar('X' + axis);
  lcd_moveto(X_VALUE_POS + offs, XYZ_BASELINE);
  if (blink)
    lcd_put_u8str(value);
  else {
    if (!TEST(axis_homed, axis))
      while (const char c = *value++) lcd_put_wchar(c <= '.' ? c : '?');
    else {
      #if DISABLED(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING)
        if (!TEST(axis_known_position, axis))
          lcd_put_u8str_P(axis == Z_AXIS ? PSTR("       ") : PSTR("    "));
        else
      #endif
          lcd_put_u8str(value);
    }
  }
}

#if ENABLED(MARLIN_DEV_MODE)
  uint16_t count_renders = 0;
  uint32_t total_cycles = 0;
#endif

void MarlinUI::draw_status_screen() {

  #if ENABLED(MARLIN_DEV_MODE)
    if (first_page) count_renders++;
  #endif

  static char xstring[5], ystring[5], zstring[8];
  #if ENABLED(FILAMENT_LCD_DISPLAY)
    static char wstring[5], mstring[4];
  #endif

  // At the first page, generate new display values
  if (first_page) {
    #if ANIM_HOTEND || ANIM_BED || ANIM_CHAMBER
      uint8_t new_bits = 0;
      #if ANIM_HOTEND
        HOTEND_LOOP() if (thermalManager.isHeatingHotend(e) ^ SHOW_ON_STATE) SBI(new_bits, e);
      #endif
      #if ANIM_BED
        if (thermalManager.isHeatingBed() ^ SHOW_ON_STATE) SBI(new_bits, 7);
      #endif
      #if ANIM_CHAMBER
        if (thermalManager.isHeatingChamber() ^ SHOW_ON_STATE) SBI(new_bits, 6);
      #endif
      heat_bits = new_bits;
    #endif
    strcpy(xstring, ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])));
    strcpy(ystring, ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])));
    strcpy(zstring, ftostr52sp(LOGICAL_Z_POSITION(current_position[Z_AXIS])));
    #if ENABLED(FILAMENT_LCD_DISPLAY)
      strcpy(wstring, ftostr12ns(filament_width_meas));
      strcpy(mstring, i16tostr3(100.0 * (
          parser.volumetric_enabled
            ? planner.volumetric_area_nominal / planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
            : planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
        )
      ));
    #endif
  }

  const bool blink = get_blink();

  // Status Menu Font
  set_font(FONT_STATUSMENU);

  #if ENABLED(MARLIN_DEV_MODE)
    TCNT5 = 0;
  #endif

  #if STATUS_LOGO_WIDTH
    if (PAGE_CONTAINS(STATUS_LOGO_Y, STATUS_LOGO_Y + STATUS_LOGO_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_LOGO_X, STATUS_LOGO_Y, STATUS_LOGO_BYTEWIDTH, STATUS_LOGO_HEIGHT, status_logo_bmp);
  #endif

  #if STATUS_HEATERS_WIDTH
    // Draw all heaters (and maybe the bed) in one go
    if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_Y + STATUS_HEATERS_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_HEATERS_X, STATUS_HEATERS_Y, STATUS_HEATERS_BYTEWIDTH, STATUS_HEATERS_HEIGHT, status_heaters_bmp);
  #endif

  #if DO_DRAW_BED
    #if ANIM_BED
      #define BED_BITMAP(S) ((S) ? status_bed_on_bmp : status_bed_bmp)
    #else
      #define BED_BITMAP(S) status_bed_bmp
    #endif
    const uint8_t bedy = STATUS_BED_Y(BED_ALT()), bedh = STATUS_BED_HEIGHT(BED_ALT());
    if (PAGE_CONTAINS(bedy, bedy + bedh - 1))
      u8g.drawBitmapP(STATUS_BED_X, bedy, STATUS_BED_BYTEWIDTH, bedh, BED_BITMAP(BED_ALT()));
  #endif

  #if DO_DRAW_CHAMBER
    #if ANIM_HAMBER
      #define CHAMBER_BITMAP(S) ((S) ? status_chamber_on_bmp : status_chamber_bmp)
    #else
      #define CHAMBER_BITMAP(S) status_chamber_bmp
    #endif
    if (PAGE_CONTAINS(STATUS_CHAMBER_Y, STATUS_CHAMBER_Y + STATUS_CHAMBER_HEIGHT - 1))
      u8g.drawBitmapP(
        STATUS_CHAMBER_X, STATUS_CHAMBER_Y,
        STATUS_CHAMBER_BYTEWIDTH, STATUS_CHAMBER_HEIGHT,
        CHAMBER_BITMAP(CHAMBER_ALT())
      );
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
      u8g.drawBitmapP(
        STATUS_FAN_X, STATUS_FAN_Y,
        STATUS_FAN_BYTEWIDTH, STATUS_FAN_HEIGHT,
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
    for (uint8_t e = 0; e < MAX_HOTEND_DRAW; ++e)
      _draw_heater_status(e, blink);

    // Heated bed
    #if HAS_HEATED_BED && HOTENDS < 4
      _draw_heater_status(-1, blink);
    #endif

    #if HAS_HEATED_CHAMBER
      _draw_chamber_status(blink);
    #endif

    // Fan, if a bitmap was provided
    #if DO_DRAW_FAN
      if (PAGE_CONTAINS(STATUS_FAN_TEXT_Y - INFO_FONT_ASCENT, STATUS_FAN_TEXT_Y - 1)) {
        char c = '%';
        uint16_t spd = thermalManager.fan_speed[0];
        if (spd) {
          #if ENABLED(ADAPTIVE_FAN_SLOWING)
            if (!blink && thermalManager.fan_speed_scaler[0] < 128) {
              spd = (spd * thermalManager.fan_speed_scaler[0]) >> 7;
              c = '*';
            }
          #endif
          lcd_moveto(STATUS_FAN_TEXT_X, STATUS_FAN_TEXT_Y);
          lcd_put_u8str(i16tostr3(thermalManager.fanPercent(spd)));
          lcd_put_wchar(c);
        }
      }
    #endif
  }

  #if ENABLED(MARLIN_DEV_MODE)
    total_cycles += TCNT5;
  #endif

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
    #define PROGRESS_BAR_X 54
    #define PROGRESS_BAR_WIDTH (LCD_PIXEL_WIDTH - PROGRESS_BAR_X)

    if (PAGE_CONTAINS(49, 52))
      u8g.drawFrame(PROGRESS_BAR_X, 49, PROGRESS_BAR_WIDTH, 4);

    const uint8_t progress = get_progress();

    if (progress > 1) {

      //
      // Progress bar solid part
      //

      if (PAGE_CONTAINS(50, 51))     // 50-51 (or just 50)
        u8g.drawBox(
          PROGRESS_BAR_X + 1, 50,
          (uint16_t)((PROGRESS_BAR_WIDTH - 2) * progress * 0.01), 2
        );

      //
      // SD Percent Complete
      //

      #if ENABLED(DOGM_SD_PERCENT)
        if (PAGE_CONTAINS(41, 48)) {
          // Percent complete
          lcd_moveto(55, 48);
          lcd_put_u8str(ui8tostr3(progress));
          lcd_put_wchar('%');
        }
      #endif
    }

    //
    // Elapsed Time
    //

    #if DISABLED(DOGM_SD_PERCENT)
      #define SD_DURATION_X (PROGRESS_BAR_X + (PROGRESS_BAR_WIDTH / 2) - len * (MENU_FONT_WIDTH / 2))
    #else
      #define SD_DURATION_X (LCD_PIXEL_WIDTH - len * MENU_FONT_WIDTH)
    #endif

    if (PAGE_CONTAINS(EXTRAS_BASELINE - INFO_FONT_ASCENT, EXTRAS_BASELINE - 1)) {
      char buffer[13];
      duration_t elapsed = print_job_timer.duration();
      bool has_days = (elapsed.value >= 60*60*24L);
      uint8_t len = elapsed.toDigital(buffer, has_days);
      lcd_moveto(SD_DURATION_X, EXTRAS_BASELINE);
      lcd_put_u8str(buffer);
    }

  #endif // HAS_PRINT_PROGRESS

  //
  // XYZ Coordinates
  //

  #define X_LABEL_POS  3
  #define X_VALUE_POS 11
  #define XYZ_SPACING 37

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

        lcd_moveto(X_LABEL_POS, XYZ_BASELINE);

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
        lcd_put_u8str(mixer_messages);

      #else

        _draw_axis_value(X_AXIS, xstring, blink);
        _draw_axis_value(Y_AXIS, ystring, blink);

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
    lcd_moveto(3, EXTRAS_2_BASELINE);
    lcd_put_wchar(LCD_STR_FEEDRATE[0]);

    set_font(FONT_STATUSMENU);
    lcd_moveto(12, EXTRAS_2_BASELINE);
    lcd_put_u8str(i16tostr3(feedrate_percentage));
    lcd_put_wchar('%');

    //
    // Filament sensor display if SD is disabled
    //
    #if ENABLED(FILAMENT_LCD_DISPLAY) && DISABLED(SDSUPPORT)
      lcd_moveto(56, EXTRAS_2_BASELINE);
      lcd_put_u8str(wstring);
      lcd_moveto(102, EXTRAS_2_BASELINE);
      lcd_put_u8str(mstring);
      lcd_put_wchar('%');
      set_font(FONT_MENU);
      lcd_moveto(47, EXTRAS_2_BASELINE);
      lcd_put_wchar(LCD_STR_FILAM_DIA[0]); // lcd_put_u8str_P(PSTR(LCD_STR_FILAM_DIA));
      lcd_moveto(93, EXTRAS_2_BASELINE);
      lcd_put_wchar(LCD_STR_FILAM_MUL[0]);
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

  #if ENABLED(MARLIN_DEV_MODE)
    if (PAGE_CONTAINS(64-8, 64-1)) {
      lcd_put_int(total_cycles);
      lcd_put_wchar('/');
      lcd_put_int(count_renders);
      lcd_put_wchar('=');
      lcd_put_int(int(total_cycles / count_renders));
      return;
    }
  #endif

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
