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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * lcd/dwin/marlin/ultralcd_dwin.cpp
 *
 * MarlinUI implementation for a Creality DWIN display
 * with image at index 0 (0_start.jpg) as the Boot Screen.
 */

#include "../../../inc/MarlinConfigPre.h"

#if IS_DWIN_MARLINUI

#include "ultralcd_dwin.h"
#include "../dwin_lcd.h"

#include "../../lcdprint.h"
#include "../../fontutils.h"
#include "../../../libs/numtostr.h"
#include "../../marlinui.h"

#include "../../../sd/cardreader.h"
#include "../../../module/motion.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"

#if ENABLED(SDSUPPORT)
  #include "../../../libs/duration_t.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../../feature/bedlevel/bedlevel.h"
#endif

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

#define S(V) (char*)(V)

// DWIN printing specifies the font on each string operation
// but we'll make the font modal for Marlin
dwin_font_t dwin_font = { font8x16, 8, 16, Color_White, Color_Bg_Black, true };
void MarlinUI::set_font(const uint8_t font_nr) {
  if (font_nr != dwin_font.index) {
    dwin_font.index = font_nr;
    uint8_t w, h;
    switch (font_nr) {
      default:
      case font6x12:  w =  6; h = 12; break;
      case font8x16:  w =  8; h = 16; break;
      case font10x20: w = 10; h = 20; break;
      case font12x24: w = 12; h = 24; break;
      case font14x28: w = 14; h = 28; break;
      case font16x32: w = 16; h = 32; break;
      case font20x40: w = 20; h = 40; break;
      case font24x48: w = 24; h = 48; break;
      case font28x56: w = 28; h = 56; break;
      case font32x64: w = 32; h = 64; break;
    }
    dwin_font.width = w;
    dwin_font.height = h;
    // TODO: Array with dimensions, auto fit menu items,
    // update char width / height of the screen based on
    // new (fixed-width) font size.
  }
}

// Since we don't have 'lcd' and 'u8g' objects to help out,
// we'll track the cursor, draw color, etc. ourselves.

void set_dwin_text_fg(const uint16_t color_ind) { dwin_font.fg = color_ind; }
void set_dwin_text_bg(const uint16_t color_ind) { dwin_font.bg = color_ind; }
void set_dwin_text_solid(const bool solid) { dwin_font.solid = solid; }

// This display is always detected
bool MarlinUI::detected() { return true; }

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() { DWIN_Startup(); }

// This LCD should clear where it will draw anew
void MarlinUI::clear_lcd() {
  DWIN_Frame_Clear(Color_Bg_Black);
  DWIN_UpdateLCD();
}

#if ENABLED(SHOW_BOOTSCREEN)

  // A Bootscreen will be shown if the DWIN_SET includes
  // a startup image. So just show a fake "loading" bar
  // with this option.
  void MarlinUI::show_bootscreen() {
    for (uint16_t t = 0; t <= 100; t += 2) {
      // Draw the whole bar each time, but then...
      DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
      // ...erase the right end of the bar
      DWIN_Draw_Rectangle(1, Color_Bg_Black, 15 + t * 242 / 100, 260, 257, 280);
      DWIN_UpdateLCD();
      delay(20); // 50 fps
    }
    clear_lcd();
  }

#endif

// The kill screen is displayed for unrecoverable conditions
void MarlinUI::draw_kill_screen() {
  set_font(DWIN_FONT_ALERT);
  // TODO: Draw a Red and Yellow Box
  //       Landscape and Portrait versions centered using current font metrics
  DWIN_Draw_String(false,true,DWIN_FONT_ALERT, Popup_Text_Color, Color_Bg_Window, 24, 260, status_message);
  DWIN_Draw_String(false,true,DWIN_FONT_ALERT, Popup_Text_Color, Color_Bg_Window, 24, 280, GET_TEXT_F(MSG_HALTED));
  DWIN_Draw_String(false,true,DWIN_FONT_ALERT, Popup_Text_Color, Color_Bg_Window, 24, 300, GET_TEXT_F(MSG_PLEASE_RESET));
  DWIN_UpdateLCD();
}

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink) {
  lcd_put_wchar('X' + uint8_t(axis));
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

FORCE_INLINE void _draw_heater_status(const heater_id_t heater, const char prefix, const bool blink) {
  #if HAS_HEATED_BED
    const bool isBed = heater < 0;
    const float t1 = (isBed ? thermalManager.degBed()       : thermalManager.degHotend(heater)),
                t2 = (isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater));
  #else
    const float t1 = thermalManager.degHotend(heater), t2 = thermalManager.degTargetHotend(heater);
  #endif

  if (prefix >= 0) lcd_put_wchar(prefix);

  lcd_put_u8str(i16tostr3rj(t1 + 0.5));
  lcd_put_wchar('/');

  #if !HEATER_IDLE_HANDLER
    UNUSED(blink);
  #else
    const bool is_idle = (
      #if HAS_HEATED_BED
        isBed ? thermalManager.bed_idle.timed_out :
      #endif
      thermalManager.hotend_idle[heater].timed_out
    );

    if (!blink && is_idle) {
      lcd_put_wchar(' ');
      if (t2 >= 10) lcd_put_wchar(' ');
      if (t2 >= 100) lcd_put_wchar(' ');
    }
    else
  #endif
      lcd_put_u8str(i16tostr3left(t2 + 0.5));

  if (prefix >= 0) {
    lcd_put_wchar('C');
    lcd_put_wchar(' ');
    if (t2 < 10) lcd_put_wchar(' ');
  }
}

FORCE_INLINE void _draw_bed_status(const bool blink) {
  _draw_heater_status(H_BED, (
      #if HAS_LEVELING
        planner.leveling_active && blink ? '_' :
      #endif
      'B'
    ),
    blink
  );
}

#if HAS_PRINT_PROGRESS

  FORCE_INLINE void _draw_print_progress() {
    const uint8_t progress = ui.get_progress_percent();
    lcd_put_u8str_P(PSTR(
      #if ENABLED(SDSUPPORT)
        "SD"
      #elif ENABLED(LCD_SET_PROGRESS_MANUALLY)
        "P:"
      #endif
    ));
    if (progress)
      lcd_put_u8str(ui8tostr3rj(progress));
    else
      lcd_put_u8str_P(PSTR("---"));
    lcd_put_wchar('%');
  }

#endif

#if ENABLED(LCD_PROGRESS_BAR)

  void MarlinUI::draw_progress_bar(const uint8_t percent) {
    // TODO: Draw a bar in the status message area, for now
  }

#endif // LCD_PROGRESS_BAR

void MarlinUI::draw_status_message(const bool blink) {

  lcd_moveto(0, LCD_HEIGHT - 1);

  #if ENABLED(LCD_PROGRESS_BAR)

    // Draw the progress bar if the message has shown long enough
    // or if there is no message set.
    if (ELAPSED(millis(), progress_bar_ms + PROGRESS_BAR_MSG_TIME) || !has_status()) {
      const uint8_t progress = get_progress_percent();
      if (progress > 2) return draw_progress_bar(progress);
    }

  #elif BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)

    // Alternate Status message and Filament display
    if (ELAPSED(millis(), next_filament_display)) {
      lcd_put_u8str_P(PSTR("Dia "));
      lcd_put_u8str(ftostr12ns(filwidth.measured_mm));
      lcd_put_u8str_P(PSTR(" V"));
      lcd_put_u8str(i16tostr3rj(planner.volumetric_percent(parser.volumetric_enabled)));
      lcd_put_wchar('%');
      return;
    }

  #endif // FILAMENT_LCD_DISPLAY && SDSUPPORT

  #if ENABLED(STATUS_MESSAGE_SCROLLING)
    static bool last_blink = false;

    // Get the UTF8 character count of the string
    uint8_t slen = utf8_strlen(status_message);

    // If the string fits into the LCD, just print it and do not scroll it
    if (slen <= LCD_WIDTH) {

      // The string isn't scrolling and may not fill the screen
      lcd_put_u8str(status_message);

      // Fill the rest with spaces
      while (slen < LCD_WIDTH) { lcd_put_wchar(' '); ++slen; }
    }
    else {
      // String is larger than the available space in screen.

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
      const char *stat = status_and_len(rlen);
      lcd_put_u8str_max(stat, LCD_WIDTH);     // The string leaves space

      // If the remaining string doesn't completely fill the screen
      if (rlen < LCD_WIDTH) {
        lcd_put_wchar('.');                   // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;     // Amount of space left in characters
        if (--chars) {                        // Draw a second dot if there's space
          lcd_put_wchar('.');
          if (--chars)
            lcd_put_u8str_max(status_message, chars); // Print a second copy of the message
        }
      }
      if (last_blink != blink) {
        last_blink = blink;
        advance_status_scroll();
      }
    }
  #else
    UNUSED(blink);

    // Get the UTF8 character count of the string
    uint8_t slen = utf8_strlen(status_message);

    // Just print the string to the LCD
    lcd_put_u8str_max(status_message, LCD_WIDTH);

    // Fill the rest with spaces if there are missing spaces
    while (slen < LCD_WIDTH) {
      lcd_put_wchar(' ');
      ++slen;
    }
  #endif
}

/**
 *  LCD_INFO_SCREEN_STYLE 0 : Classic Status Screen
 *
 *  16x2   |000/000 B000/000|
 *         |0123456789012345|
 *
 *  16x4   |000/000 B000/000|
 *         |SD---%  Z 000.00|
 *         |F---%     T--:--|
 *         |0123456789012345|
 *
 *  20x2   |T000/000° B000/000° |
 *         |01234567890123456789|
 *
 *  20x4   |T000/000° B000/000° |
 *         |X 000 Y 000 Z000.000|
 *         |F---%  SD---% T--:--|
 *         |01234567890123456789|
 *
 *  LCD_INFO_SCREEN_STYLE 1 : Průša-style Status Screen
 *
 *  |T000/000°  Z 000.00 |
 *  |B000/000°  F---%    |
 *  |SD---%     T--:--   |
 *  |01234567890123456789|
 *
 *  |T000/000°  Z 000.00 |
 *  |T000/000°  F---%    |
 *  |B000/000°  SD---%   |
 *  |01234567890123456789|
 */

void MarlinUI::draw_status_screen() {

  DWIN_Frame_Clear(Color_Bg_Window);

  const bool blink = get_blink();

  ui.set_font(DWIN_FONT_MENU);

  lcd_moveto(0, 0);

  #if LCD_INFO_SCREEN_STYLE == 0

    // ========== Line 1 ==========

    //
    // Hotend 0 Temperature
    //
    _draw_heater_status(H_E0, 'T', blink);

    //
    // Hotend 1 or Bed Temperature
    //
    #if HAS_MULTI_HOTEND
      lcd_moveto(10, 0);
      _draw_heater_status(H_E1, 'T', blink);
    #elif HAS_HEATED_BED
      lcd_moveto(10, 0);
      _draw_bed_status(blink);
    #endif

    // ========== Line 2 ==========

    lcd_moveto(0, 1);

    // If the first line has two extruder temps,
    // show more temperatures on the next line

    #if HOTENDS > 2 || (HAS_MULTI_HOTEND && HAS_HEATED_BED)

      #if HOTENDS > 2
        _draw_heater_status(H_E2, 'T', blink);
        lcd_moveto(10, 1);
      #endif

      _draw_bed_status(blink);

    #else // HOTENDS <= 2 && (HOTENDS <= 1 || !HAS_HEATED_BED)

      #if HAS_DUAL_MIXING

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
        lcd_put_u8str(mixer_messages);

      #else // !HAS_DUAL_MIXING

        const bool show_e_total = TERN0(LCD_SHOW_E_TOTAL, printingIsActive() || marlin_state == MF_SD_COMPLETE);

        if (show_e_total) {
          #if ENABLED(LCD_SHOW_E_TOTAL)
            char tmp[20];
            const uint8_t escale = e_move_accumulator >= 100000.0f ? 10 : 1; // After 100m switch to cm
            sprintf_P(tmp, PSTR("E %ld%cm       "), uint32_t(_MAX(e_move_accumulator, 0.0f)) / escale, escale == 10 ? 'c' : 'm'); // 1234567mm
            lcd_put_u8str(tmp);
          #endif
        }
        else {
          const xy_pos_t lpos = current_position.asLogical();
          _draw_axis_value(X_AXIS, ftostr4sign(lpos.x), blink);
          lcd_put_wchar(' ');
          _draw_axis_value(Y_AXIS, ftostr4sign(lpos.y), blink);
        }

      #endif // !HAS_DUAL_MIXING

    #endif // HOTENDS <= 2 && (HOTENDS <= 1 || !HAS_HEATED_BED)

    lcd_moveto(LCD_WIDTH - 8, 1);
    _draw_axis_value(Z_AXIS, ftostr52sp(LOGICAL_Z_POSITION(current_position.z)), blink);

    #if HAS_LEVELING && !HAS_HEATED_BED
      lcd_put_wchar(planner.leveling_active || blink ? '_' : ' ');
    #endif

    // ========== Line 3 ==========

    lcd_put_wchar(0, 2, 'F');
    lcd_put_u8str(i16tostr3rj(feedrate_percentage));
    lcd_put_wchar('%');

    char buffer[14];
    duration_t elapsed = print_job_timer.duration();
    const uint8_t len = elapsed.toDigital(buffer),
                  timepos = LCD_WIDTH - len - 1;
    lcd_put_wchar(timepos, 2, 'C');
    lcd_put_u8str(buffer);

    lcd_moveto(timepos - 7, 2);
    #if HAS_PRINT_PROGRESS
      _draw_print_progress();
    #else
      char c;
      uint16_t per;
      #if HAS_FAN0
        if (true
          #if EXTRUDERS && ENABLED(ADAPTIVE_FAN_SLOWING)
            && (blink || thermalManager.fan_speed_scaler[0] < 128)
          #endif
        ) {
          uint16_t spd = thermalManager.fan_speed[0];
          if (blink) c = 'F';
          #if ENABLED(ADAPTIVE_FAN_SLOWING)
            else { c = '*'; spd = thermalManager.scaledFanSpeed(0, spd); }
          #endif
          per = thermalManager.fanPercent(spd);
        }
        else
      #endif
        {
          #if EXTRUDERS
            c = 'E';
            per = planner.flow_percentage[0];
          #endif
        }
      lcd_put_wchar(c);
      lcd_put_u8str(i16tostr3rj(per));
      lcd_put_wchar('%');
    #endif

  #elif LCD_INFO_SCREEN_STYLE == 1

    // ========== Line 1 ==========

    //
    // Hotend 0 Temperature
    //
    _draw_heater_status(H_E0, 'T', blink);

    //
    // Z Coordinate
    //
    lcd_moveto(LCD_WIDTH - 9, 0);
    _draw_axis_value(Z_AXIS, ftostr52sp(LOGICAL_Z_POSITION(current_position.z)), blink);

    #if HAS_LEVELING && (HAS_MULTI_HOTEND || !HAS_HEATED_BED)
      lcd_put_wchar(LCD_WIDTH - 1, 0, planner.leveling_active || blink ? '_' : ' ');
    #endif

    // ========== Line 2 ==========

    //
    // Hotend 1 or Bed Temperature
    //
    lcd_moveto(0, 1);
    #if HAS_MULTI_HOTEND
      _draw_heater_status(H_E1, 'T', blink);
    #elif HAS_HEATED_BED
      _draw_bed_status(blink);
    #endif

    lcd_put_wchar(LCD_WIDTH - 9, 1, 'F');
    lcd_put_u8str(i16tostr3rj(feedrate_percentage));
    lcd_put_wchar('%');

    // ========== Line 3 ==========

    //
    // SD Percent, Hotend 2, or Bed
    //
    lcd_moveto(0, 2);
    #if HOTENDS > 2
      _draw_heater_status(H_E2, 'T', blink);
    #elif HAS_MULTI_HOTEND && HAS_HEATED_BED
      _draw_bed_status(blink);
    #elif HAS_PRINT_PROGRESS
      #define DREW_PRINT_PROGRESS
      _draw_print_progress();
    #endif

    //
    // Elapsed Time or SD Percent
    //
    lcd_moveto(LCD_WIDTH - 9, 2);
    #if HAS_PRINT_PROGRESS && !defined(DREW_PRINT_PROGRESS)
      _draw_print_progress();
    #else
      duration_t elapsed = print_job_timer.duration();
      char buffer[14];
      (void)elapsed.toDigital(buffer);
      lcd_put_wchar('C');
      lcd_put_u8str(buffer);
    #endif

  #endif // LCD_INFO_SCREEN_STYLE 1

  // ========= Last Line ========

  //
  // Status Message (which may be a Progress Bar or Filament display)
  //
  draw_status_message(blink);

  //DWIN_UpdateLCD();
}

#if HAS_LCD_MENU

  #include "../../menu/menu.h"

  dwin_coord_t row_y1, row_y2;

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      // TODO: Erase the background where these elements will be drawn
      row_y1 = row * (MENU_FONT_HEIGHT) + 1;
      row_y2 = row_y1 + MENU_FONT_HEIGHT - 1;

      lcd_put_wchar(DWIN_WIDTH - 11 * (MENU_FONT_WIDTH), row_y2, 'E');
      lcd_put_wchar((char)('1' + extruder));
      lcd_put_wchar(' ');
      lcd_put_u8str(i16tostr3rj(thermalManager.degHotend(extruder)));
      lcd_put_wchar('/');

      if (get_blink() || !thermalManager.hotend_idle[extruder].timed_out)
        lcd_put_u8str(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    }

  #endif // ADVANCED_PAUSE_FEATURE

  // Set the colors for a menu item based on whether it is selected
  static bool mark_as_selected(const uint8_t row, const bool sel) {
    row_y1 = row * (MENU_FONT_HEIGHT) + 1;
    row_y2 = row_y1 + MENU_FONT_HEIGHT - 1;
    lcd_moveto(0, row_y2);
    if (row_y1 >= LCD_PIXEL_HEIGHT) return false;

    if (sel) {
      #if ENABLED(MENU_HOLLOW_FRAME)
        DWIN_Draw_HLine(Select_Color, 0, row_y1 + 1, DWIN_WIDTH);
        DWIN_Draw_HLine(Select_Color, 0, row_y2 + 2, DWIN_WIDTH);
      #else
        DWIN_Draw_Box(1, Select_Color, 0, row_y1 + 2, DWIN_WIDTH, MENU_FONT_HEIGHT - 1);
      #endif
    }
    return true;
  }

  // Draw a static line of text in the same idiom as a menu item
  void MenuItem_static::draw(const uint8_t row, PGM_P const pstr, const uint8_t style/*=SS_DEFAULT*/, const char * const vstr/*=nullptr*/) {

    // TODO: Return if the row to draw is off-screen
    //       Menus may deal with this for us.

    //const uint8_t old_bkgd = dwin_font.bg;
    //set_dwin_text_bg((style & SS_INVERT) ? Select_Color : Color_Bg_Black);

    // Call mark_as_selected to draw a bigger selection box
    // and draw the text without a background

    if (mark_as_selected(row, (bool)(style & SS_INVERT))) {

      pixel_len_t n = LCD_PIXEL_WIDTH; // pixel width of string allowed

      const int8_t plen = pstr ? utf8_strlen_P(pstr) : 0,
                   vlen = vstr ? utf8_strlen(vstr) : 0;
      if (style & SS_CENTER) {
        int8_t pad = (LCD_WIDTH - plen - vlen) / 2;
        while (--pad >= 0) n -= lcd_put_wchar(' ');
      }

      if (plen) n = lcd_put_u8str_ind_P(pstr, itemIndex, itemString, n / (MENU_FONT_WIDTH)) * (MENU_FONT_WIDTH);
      if (vlen) n -= lcd_put_u8str_max(vstr, n);
      while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
    }

    //set_dwin_text_bg(old_bkgd);
  }

  // Draw a generic menu item
  void MenuItemBase::_draw(const bool sel, const uint8_t row, PGM_P const pstr, const char, const char post_char) {

    // TODO: Exit if the row is off-screen

    //set_dwin_text_bg(sel ? Select_Color : Color_Bg_Black);
    if (mark_as_selected(row, sel)) {
      pixel_len_t n = lcd_put_u8str_ind_P(pstr, itemIndex, itemString, LCD_WIDTH - 1) * (MENU_FONT_WIDTH);
      while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
      lcd_put_wchar(DWIN_WIDTH - (MENU_FONT_WIDTH), row_y2, post_char);
      lcd_put_wchar(' ');
    }
  }

  // Draw a menu item with an editable value
  void MenuEditItemBase::draw(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm) {
    if (mark_as_selected(row, sel)) {
      const uint8_t vallen = (pgm ? utf8_strlen_P(data) : utf8_strlen((char*)data));
      //dwin_coord_t n = lcd_put_u8str_ind_P(pstr, itemIndex, LCD_WIDTH - 2 - vallen) * (MENU_FONT_WIDTH);
      pixel_len_t n = lcd_put_u8str_ind_P(pstr, itemIndex, itemString, LCD_WIDTH - 2 - vallen) * (MENU_FONT_WIDTH);
      if (vallen) {
        lcd_put_wchar(':');
        while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
        lcd_moveto(DWIN_WIDTH - (MENU_FONT_WIDTH)*vallen, row_y2);
        if (pgm) lcd_put_u8str_P(data); else lcd_put_u8str((char*)data);
      }
    }
  }

  void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char* const value/*=nullptr*/) {
    ui.encoder_direction_normal();

    const dwin_coord_t labellen = utf8_strlen_P(pstr), vallen = utf8_strlen(value);
    bool extra_row = labellen > LCD_WIDTH - 2 - vallen;

    #if ENABLED(USE_BIG_EDIT_FONT)
      // Use the menu font if the label won't fit on a single line
      constexpr dwin_coord_t lcd_edit_width = (DWIN_WIDTH) / (EDIT_FONT_WIDTH);
      dwin_coord_t lcd_chr_fit, one_chr_width;
      if (labellen <= lcd_edit_width - 1) {
        if (labellen + vallen + 1 > lcd_edit_width)
          extra_row = true;
        lcd_chr_fit = lcd_edit_width + 1;
        one_chr_width = EDIT_FONT_WIDTH;
        ui.set_font(FONT_EDIT);
      }
      else {
        lcd_chr_fit = LCD_WIDTH;
        one_chr_width = MENU_FONT_WIDTH;
        ui.set_font(DWIN_FONT_MENU);
      }
    #else
      constexpr dwin_coord_t lcd_chr_fit = LCD_WIDTH,
                             one_chr_width = MENU_FONT_WIDTH;
    #endif

    // Center the label and value lines on the middle line
    dwin_coord_t baseline = extra_row ? (DWIN_HEIGHT) / 2 - 1
                                      : (DWIN_HEIGHT + EDIT_FONT_ASCENT) / 2;

    // Assume the label is alpha-numeric (with a descender)
    lcd_put_u8str_ind_P(0, baseline, pstr, itemIndex);

    // If a value is included, print a colon, then print the value right-justified
    if (value != nullptr) {
      lcd_put_wchar(':');
      if (extra_row) {
        // Assume that value is numeric (with no descender)
        baseline += EDIT_FONT_ASCENT + 2;
      }
      lcd_put_wchar(((lcd_chr_fit - 1) - (vallen + 1)) * one_chr_width, baseline, ' '); // Right-justified, padded, add a leading space
      lcd_put_u8str(value);
    }
  }

  inline void draw_boxed_string(const dwin_coord_t x, const dwin_coord_t y, PGM_P const pstr, const bool inv) {
    const dwin_coord_t len = utf8_strlen_P(pstr), bw = len * (MENU_FONT_WIDTH),
                     bx = x * (MENU_FONT_WIDTH), by = (y + 1) * (MENU_FONT_HEIGHT);
    if (inv) {
      DWIN_Draw_Box(1, Select_Color, bx - 1, by - (MENU_FONT_ASCENT) + 1, bw + 2, MENU_FONT_HEIGHT - 1);
      set_dwin_text_fg(Color_Bg_Black);
    }
    lcd_put_u8str_P(bx, by, pstr);
    if (inv) set_dwin_text_fg(Color_White);
  }

  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
    ui.draw_select_screen_prompt(pref, string, suff);
    draw_boxed_string(1, LCD_HEIGHT - 1, no, !yesno);
    draw_boxed_string(LCD_WIDTH - (utf8_strlen_P(yes) + 1), LCD_HEIGHT - 1, yes, yesno);
  }

  #if ENABLED(SDSUPPORT)

    void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
      if (mark_as_selected(row, sel)) {
        #if 0
        // TODO: Draw a folder icon (poly)
        if (isDir) lcd_put_wchar('F');
        constexpr uint8_t maxlen = LCD_WIDTH - 1;
        const dwin_coord_t pixw = maxlen * (MENU_FONT_WIDTH);
        dwin_coord_t n = pixw - lcd_put_u8str_max(ui.scrolled_filename(theCard, maxlen, row, sel), pixw);
        while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
        #endif
      }
    }

  #endif // SDSUPPORT

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    /**
     * UBL LCD "radar" map data
     */
    #define MAP_UPPER_LEFT_CORNER_X 35  // These probably should be moved to the .h file  But for now,
    #define MAP_UPPER_LEFT_CORNER_Y  8  // it is easier to play with things having them here
    #define MAP_MAX_PIXELS_X        53
    #define MAP_MAX_PIXELS_Y        49

    void MarlinUI::ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {
      // Scale the box pixels appropriately
      dwin_coord_t x_map_pixels = ((MAP_MAX_PIXELS_X - 4) / (GRID_MAX_POINTS_X)) * (GRID_MAX_POINTS_X),
                   y_map_pixels = ((MAP_MAX_PIXELS_Y - 4) / (GRID_MAX_POINTS_Y)) * (GRID_MAX_POINTS_Y),

              pixels_per_x_mesh_pnt = x_map_pixels / (GRID_MAX_POINTS_X),
              pixels_per_y_mesh_pnt = y_map_pixels / (GRID_MAX_POINTS_Y),

              x_offset = MAP_UPPER_LEFT_CORNER_X + 1 + (MAP_MAX_PIXELS_X - x_map_pixels - 2) / 2,
              y_offset = MAP_UPPER_LEFT_CORNER_Y + 1 + (MAP_MAX_PIXELS_Y - y_map_pixels - 2) / 2;

      #if 0

      // Clear the Mesh Map

      set_dwin_text_fg(Color_White);  // First draw the bigger box in White so we have a border around the mesh map box
      u8g.drawBox(x_offset - 2, y_offset - 2, x_map_pixels + 4, y_map_pixels + 4);
      set_dwin_text_fg(Color_Bg_Black);  // Now actually clear the mesh map box
      u8g.drawBox(x_offset, y_offset, x_map_pixels, y_map_pixels);

      // Display Mesh Point Locations

      set_dwin_text_fg(Color_White);
      const dwin_coord_t sx = x_offset + pixels_per_x_mesh_pnt / 2;
            dwin_coord_t  y = y_offset + pixels_per_y_mesh_pnt / 2;
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++, y += pixels_per_y_mesh_pnt)
        for (uint8_t i = 0, x = sx; i < GRID_MAX_POINTS_X; i++, x += pixels_per_x_mesh_pnt)
          u8g.drawBox(x, y, 1, 1);

      // Fill in the Specified Mesh Point

      const uint8_t y_plot_inv = (GRID_MAX_POINTS_Y - 1) - y_plot;  // The origin is typically in the lower right corner.  We need to
                                                                    // invert the Y to get it to plot in the right location.

      const dwin_coord_t by = y_offset + y_plot_inv * pixels_per_y_mesh_pnt;
      u8g.drawBox(
        x_offset + x_plot * pixels_per_x_mesh_pnt, by,
        pixels_per_x_mesh_pnt, pixels_per_y_mesh_pnt
      );

      // Put Relevant Text on Display

      // Show X and Y positions at top of screen
      set_dwin_text_fg(Color_White);
      const xy_pos_t pos = { ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot) },
                     lpos = pos.asLogical();
      lcd_put_u8str_P(5, 7, X_LBL);
      lcd_put_u8str(ftostr52(lpos.x));
      lcd_put_u8str_P(74, 7, Y_LBL);
      lcd_put_u8str(ftostr52(lpos.y));

      // Print plot position
      lcd_put_wchar(5, DWIN_HEIGHT, '(');
      u8g.print(x_plot);
      lcd_put_wchar(',');
      u8g.print(y_plot);
      lcd_put_wchar(')');

      // Show the location value
      lcd_put_u8str_P(74, DWIN_HEIGHT, Z_LBL);
      if (!isnan(ubl.z_values[x_plot][y_plot]))
        lcd_put_u8str(ftostr43sign(ubl.z_values[x_plot][y_plot]));
      else
        lcd_put_u8str_P(PSTR(" -----"));
      #endif
    }

  #endif // AUTO_BED_LEVELING_UBL

  #if EITHER(BABYSTEP_ZPROBE_GFX_OVERLAY, MESH_EDIT_GFX_OVERLAY)

    // cw_bmp = DWIN_BITMAP_CLOCKWISE;
    // ccw_bmp = DWIN_BITMAP_COUNTERCLOCKWISE;
    // up_arrow_bmp = DWIN_UP_ARROW;
    // down_arrow_bmp = DWIN_DOWN_ARROW;
    // offset_bedline_bmp = DWIN_BEDLINE;
    // nozzle_bmp = DWIN_NOZZLE;

    void _lcd_zoffset_overlay_gfx(const float zvalue) {
      // Determine whether the user is raising or lowering the nozzle.
      static int8_t dir;
      static float old_zvalue;
      if (zvalue != old_zvalue) {
        dir = zvalue ? zvalue < old_zvalue ? -1 : 1 : 0;
        old_zvalue = zvalue;
      }

      #if 0
      const unsigned char *rot_up = TERN(OVERLAY_GFX_REVERSE, ccw_bmp, cw_bmp),
                        *rot_down = TERN(OVERLAY_GFX_REVERSE, cw_bmp, ccw_bmp);

      #if ENABLED(USE_BIG_EDIT_FONT)
        const int left = 0, right = 45, nozzle = 95;
      #else
        const int left = 5, right = 90, nozzle = 60;
      #endif

      // Draw a representation of the nozzle
      u8g.drawBitmapP(nozzle + 6, 4 - dir, 2, 12, nozzle_bmp);
      u8g.drawBitmapP(nozzle + 0, 20, 3, 1, offset_bedline_bmp);

      // Draw cw/ccw indicator and up/down arrows.
      u8g.drawBitmapP(right + 0, 48 - dir, 2, 13, up_arrow_bmp);
      u8g.drawBitmapP(left  + 0, 49 - dir, 2, 13, down_arrow_bmp);
      u8g.drawBitmapP(left  + 13, 47, 3, 16, rot_down);
      u8g.drawBitmapP(right + 13, 47, 3, 16, rot_up);
      #endif
    }

  #endif // BABYSTEP_ZPROBE_GFX_OVERLAY || MESH_EDIT_GFX_OVERLAY

#endif // HAS_LCD_MENU

#endif // DWIN_MARLINUI_PORTRAIT
