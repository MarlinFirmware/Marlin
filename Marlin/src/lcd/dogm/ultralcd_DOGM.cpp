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

/**
 * lcd/dogm/ultralcd_DOGM.h
 *
 * Implementation of the LCD display routines for a DOGM128 graphic display.
 * by STB for ErikZalm/Marlin. Common LCD 128x64 pixel graphic displays.
 *
 * Demonstrator: http://www.reprap.org/wiki/STB_Electronics
 * License: http://opensource.org/licenses/BSD-3-Clause
 *
 * With the use of:
 *  u8glib by Oliver Kraus
 *  https://github.com/olikraus/U8glib_Arduino
 *  License: http://opensource.org/licenses/BSD-3-Clause
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_LCD

#include "ultralcd_DOGM.h"
#include "u8g_fontutf8.h"

#if ENABLED(SHOW_BOOTSCREEN)
  #include "dogm_Bootscreen.h"
#endif

#include "../lcdprint.h"
#include "../fontutils.h"
#include "../../libs/numtostr.h"
#include "../ultralcd.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"

#if ENABLED(SDSUPPORT)
  #include "../../libs/duration_t.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

/**
 * Include all needed font files
 * (See http://marlinfw.org/docs/development/fonts.html)
 */
#include "fontdata/fontdata_ISO10646_1.h"
#if ENABLED(USE_SMALL_INFOFONT)
  #include "fontdata/fontdata_6x9_marlin.h"
  #define FONT_STATUSMENU_NAME u8g_font_6x9
#else
  #define FONT_STATUSMENU_NAME MENU_FONT_NAME
#endif

U8G_CLASS u8g(U8G_PARAM);

#include LANGUAGE_DATA_INCL(LCD_LANGUAGE)

#if HAS_LCD_CONTRAST

  int16_t MarlinUI::contrast = DEFAULT_LCD_CONTRAST;

  void MarlinUI::set_contrast(const int16_t value) {
    contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
    u8g.setContrast(contrast);
  }

#endif

void MarlinUI::set_font(const MarlinFont font_nr) {
  static char currentfont = 0;
  if (font_nr != currentfont) {
    switch ((currentfont = font_nr)) {
      case FONT_STATUSMENU : u8g.setFont(FONT_STATUSMENU_NAME); break;
      case FONT_EDIT       : u8g.setFont(EDIT_FONT_NAME);       break;
      default:
      case FONT_MENU       : u8g.setFont(MENU_FONT_NAME);       break;
    }
  }
}

bool MarlinUI::detected() { return true; }

#if ENABLED(SHOW_BOOTSCREEN)

  #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
    // Draws a slice of a particular frame of the custom bootscreen, without the u8g loop
    void MarlinUI::draw_custom_bootscreen(const uint8_t frame/*=0*/) {
      constexpr u8g_uint_t left = u8g_uint_t((LCD_PIXEL_WIDTH  - (CUSTOM_BOOTSCREEN_BMPWIDTH)) / 2),
                            top = u8g_uint_t((LCD_PIXEL_HEIGHT - (CUSTOM_BOOTSCREEN_BMPHEIGHT)) / 2);
      #if ENABLED(CUSTOM_BOOTSCREEN_INVERTED)
        constexpr u8g_uint_t right = left + CUSTOM_BOOTSCREEN_BMPWIDTH,
                            bottom = top + CUSTOM_BOOTSCREEN_BMPHEIGHT;
      #endif

      const u8g_pgm_uint8_t * const bmp =
        #if ENABLED(CUSTOM_BOOTSCREEN_ANIMATED)
          (u8g_pgm_uint8_t*)pgm_read_ptr(&custom_bootscreen_animation[frame])
        #else
          custom_start_bmp
        #endif
      ;

      u8g.drawBitmapP(left, top, CUSTOM_BOOTSCREEN_BMP_BYTEWIDTH, CUSTOM_BOOTSCREEN_BMPHEIGHT, bmp);

      #if ENABLED(CUSTOM_BOOTSCREEN_INVERTED)
        if (frame == 0) {
          u8g.setColorIndex(1);
          if (top) u8g.drawBox(0, 0, LCD_PIXEL_WIDTH, top);
          if (left) u8g.drawBox(0, top, left, CUSTOM_BOOTSCREEN_BMPHEIGHT);
          if (right < LCD_PIXEL_WIDTH) u8g.drawBox(right, top, LCD_PIXEL_WIDTH - right, CUSTOM_BOOTSCREEN_BMPHEIGHT);
          if (bottom < LCD_PIXEL_HEIGHT) u8g.drawBox(0, bottom, LCD_PIXEL_WIDTH, LCD_PIXEL_HEIGHT - bottom);
        }
      #endif
    }

    // Shows the custom bootscreen, with the u8g loop, animations and delays
    void MarlinUI::show_custom_bootscreen() {
      #if DISABLED(CUSTOM_BOOTSCREEN_ANIMATED)
        constexpr millis_t d = 0;
        constexpr uint8_t f = 0;
      #else
        constexpr millis_t d = CUSTOM_BOOTSCREEN_FRAME_TIME;
        LOOP_L_N(f, COUNT(custom_bootscreen_animation))
      #endif
        {
          u8g.firstPage();
          do { draw_custom_bootscreen(f); } while (u8g.nextPage());
          if (d) safe_delay(d);
        }

      #ifndef CUSTOM_BOOTSCREEN_TIMEOUT
        #define CUSTOM_BOOTSCREEN_TIMEOUT 2500
      #endif
      safe_delay(CUSTOM_BOOTSCREEN_TIMEOUT);
    }
  #endif // SHOW_CUSTOM_BOOTSCREEN

  // Two-part needed to display all info
  constexpr bool two_part = ((LCD_PIXEL_HEIGHT) - (START_BMPHEIGHT)) < ((MENU_FONT_ASCENT) * 2);

  // Draw the static Marlin bootscreen from a u8g loop
  // or the animated boot screen within its own u8g loop
  void MarlinUI::draw_marlin_bootscreen(const bool line2/*=false*/) {

    // Determine text space needed
    constexpr u8g_uint_t text_width_1 = u8g_uint_t((sizeof(SHORT_BUILD_VERSION) - 1) * (MENU_FONT_WIDTH)),
                         text_width_2 = u8g_uint_t((sizeof(MARLIN_WEBSITE_URL) - 1) * (MENU_FONT_WIDTH)),
                         text_max_width = _MAX(text_width_1, text_width_2),
                         text_total_height = (MENU_FONT_HEIGHT) * 2,
                         width = LCD_PIXEL_WIDTH, height = LCD_PIXEL_HEIGHT,
                         rspace = width - (START_BMPWIDTH);

    u8g_int_t offx, offy, txt_base, txt_offx_1, txt_offx_2;

    // Can the text fit to the right of the bitmap?
    if (text_max_width < rspace) {
      constexpr int8_t inter = (width - text_max_width - (START_BMPWIDTH)) / 3; // Evenly distribute horizontal space
      offx = inter;                             // First the boot logo...
      offy = (height - (START_BMPHEIGHT)) / 2;  // ...V-aligned in the full height
      txt_offx_1 = txt_offx_2 = inter + (START_BMPWIDTH) + inter; // Text right of the bitmap
      txt_base = (height + MENU_FONT_ASCENT + text_total_height - (MENU_FONT_HEIGHT)) / 2; // Text vertical center
    }
    else {
      constexpr int8_t inter = (height - text_total_height - (START_BMPHEIGHT)) / 3; // Evenly distribute vertical space
      offx = rspace / 2;                        // Center the boot logo in the whole space
      offy = inter;                             // V-align boot logo proportionally
      txt_offx_1 = (width - text_width_1) / 2;  // Text 1 centered
      txt_offx_2 = (width - text_width_2) / 2;  // Text 2 centered
      txt_base = offy + START_BMPHEIGHT + offy + text_total_height - (MENU_FONT_DESCENT);   // Even spacing looks best
    }
    NOLESS(offx, 0);
    NOLESS(offy, 0);

    auto _draw_bootscreen_bmp = [&](const uint8_t *bitmap) {
      u8g.drawBitmapP(offx, offy, START_BMP_BYTEWIDTH, START_BMPHEIGHT, bitmap);
      set_font(FONT_MENU);
      if (!two_part || !line2) lcd_put_u8str_P(txt_offx_1, txt_base - (MENU_FONT_HEIGHT), PSTR(SHORT_BUILD_VERSION));
      if (!two_part || line2) lcd_put_u8str_P(txt_offx_2, txt_base, PSTR(MARLIN_WEBSITE_URL));
    };

    auto draw_bootscreen_bmp = [&](const uint8_t *bitmap) {
      u8g.firstPage(); do { _draw_bootscreen_bmp(bitmap); } while (u8g.nextPage());
    };

    #if DISABLED(BOOT_MARLIN_LOGO_ANIMATED)
      draw_bootscreen_bmp(start_bmp);
    #else
      constexpr millis_t d = MARLIN_BOOTSCREEN_FRAME_TIME;
      LOOP_L_N(f, COUNT(marlin_bootscreen_animation)) {
        draw_bootscreen_bmp((uint8_t*)pgm_read_ptr(&marlin_bootscreen_animation[f]));
        if (d) safe_delay(d);
      }
    #endif
  }

  // Show the Marlin bootscreen, with the u8g loop and delays
  void MarlinUI::show_marlin_bootscreen() {
    #ifndef BOOTSCREEN_TIMEOUT
      #define BOOTSCREEN_TIMEOUT 2500
    #endif
    constexpr uint8_t pages = two_part ? 2 : 1;
    for (uint8_t q = pages; q--;) {
      draw_marlin_bootscreen(q == 0);
      safe_delay((BOOTSCREEN_TIMEOUT) / pages);
    }
  }

  void MarlinUI::show_bootscreen() {
    #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
      show_custom_bootscreen();
    #endif
    show_marlin_bootscreen();
  }

#endif // SHOW_BOOTSCREEN

#if ENABLED(LIGHTWEIGHT_UI)
  #include "status_screen_lite_ST7920.h"
#endif

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {
  #if DISABLED(MKS_LCD12864B)

    #if PIN_EXISTS(LCD_BACKLIGHT)
      OUT_WRITE(LCD_BACKLIGHT_PIN, (
        #if ENABLED(DELAYED_BACKLIGHT_INIT)
          LOW  // Illuminate after reset
        #else
          HIGH // Illuminate right away
        #endif
      ));
    #endif

    #if EITHER(MKS_12864OLED, MKS_12864OLED_SSD1306)
      SET_OUTPUT(LCD_PINS_DC);
      #ifndef LCD_RESET_PIN
        #define LCD_RESET_PIN LCD_PINS_RS
      #endif
    #endif

    #if PIN_EXISTS(LCD_RESET)
      // Perform a clean hardware reset with needed delays
      OUT_WRITE(LCD_RESET_PIN, LOW);
      _delay_ms(5);
      WRITE(LCD_RESET_PIN, HIGH);
      _delay_ms(5);
      u8g.begin();
    #endif

    #if PIN_EXISTS(LCD_BACKLIGHT) && ENABLED(DELAYED_BACKLIGHT_INIT)
      WRITE(LCD_BACKLIGHT_PIN, HIGH);
    #endif

    #if HAS_LCD_CONTRAST
      refresh_contrast();
    #endif

    #if ENABLED(LCD_SCREEN_ROT_90)
      u8g.setRot90();
    #elif ENABLED(LCD_SCREEN_ROT_180)
      u8g.setRot180();
    #elif ENABLED(LCD_SCREEN_ROT_270)
      u8g.setRot270();
    #endif

  #endif // !MKS_LCD12864B

  uxg_SetUtf8Fonts(g_fontinfo, COUNT(g_fontinfo));
}

// The kill screen is displayed for unrecoverable conditions
void MarlinUI::draw_kill_screen() {
  #if ENABLED(LIGHTWEIGHT_UI)
    ST7920_Lite_Status_Screen::clear_text_buffer();
  #endif
  const u8g_uint_t h4 = u8g.getHeight() / 4;
  u8g.firstPage();
  do {
    set_font(FONT_MENU);
    lcd_put_u8str(0, h4 * 1, status_message);
    lcd_put_u8str_P(0, h4 * 2, GET_TEXT(MSG_HALTED));
    lcd_put_u8str_P(0, h4 * 3, GET_TEXT(MSG_PLEASE_RESET));
  } while (u8g.nextPage());
}

void MarlinUI::clear_lcd() { } // Automatically cleared by Picture Loop

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  u8g_uint_t row_y1, row_y2;

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      row_y1 = row * (MENU_FONT_HEIGHT) + 1;
      row_y2 = row_y1 + MENU_FONT_HEIGHT - 1;

      if (!PAGE_CONTAINS(row_y1 + 1, row_y2 + 2)) return;

      lcd_put_wchar(LCD_PIXEL_WIDTH - 11 * (MENU_FONT_WIDTH), row_y2, 'E');
      lcd_put_wchar((char)('1' + extruder));
      lcd_put_wchar(' ');
      lcd_put_u8str(i16tostr3(thermalManager.degHotend(extruder)));
      lcd_put_wchar('/');

      if (get_blink() || !thermalManager.hotend_idle[extruder].timed_out)
        lcd_put_u8str(i16tostr3(thermalManager.degTargetHotend(extruder)));
    }

  #endif // ADVANCED_PAUSE_FEATURE

  // Set the colors for a menu item based on whether it is selected
  static bool mark_as_selected(const uint8_t row, const bool sel) {
    row_y1 = row * (MENU_FONT_HEIGHT) + 1;
    row_y2 = row_y1 + MENU_FONT_HEIGHT - 1;

    if (!PAGE_CONTAINS(row_y1 + 1, row_y2 + 2)) return false;

    if (sel) {
      #if ENABLED(MENU_HOLLOW_FRAME)
        u8g.drawHLine(0, row_y1 + 1, LCD_PIXEL_WIDTH);
        u8g.drawHLine(0, row_y2 + 2, LCD_PIXEL_WIDTH);
      #else
        u8g.setColorIndex(1); // black on white
        u8g.drawBox(0, row_y1 + 2, LCD_PIXEL_WIDTH, MENU_FONT_HEIGHT - 1);
        u8g.setColorIndex(0); // white on black
      #endif
    }
    #if DISABLED(MENU_HOLLOW_FRAME)
      else {
        u8g.setColorIndex(1); // unmarked text is black on white
      }
    #endif

    if (!PAGE_CONTAINS(row_y1, row_y2)) return false;

    lcd_moveto(0, row_y2);
    return true;
  }

  // Draw a static line of text in the same idiom as a menu item
  void MenuItem_static::draw(const uint8_t row, PGM_P const pstr, const uint8_t style/*=SS_DEFAULT*/, const char * const valstr/*=nullptr*/) {

    if (mark_as_selected(row, style & SS_INVERT)) {

      u8g_uint_t n = LCD_PIXEL_WIDTH; // pixel width of string allowed

      if ((style & SS_CENTER) && !valstr) {
        int8_t pad = (LCD_WIDTH - utf8_strlen_P(pstr)) / 2;
        while (--pad >= 0) { lcd_put_wchar(' '); n--; }
      }
      n = lcd_put_u8str_ind_P(pstr, itemIndex, LCD_WIDTH) * (MENU_FONT_WIDTH);
      if (valstr) n -= lcd_put_u8str_max(valstr, n);
      while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
    }
  }

  // Draw a generic menu item
  void MenuItemBase::_draw(const bool sel, const uint8_t row, PGM_P const pstr, const char, const char post_char) {
    if (mark_as_selected(row, sel)) {
      u8g_uint_t n = lcd_put_u8str_ind_P(pstr, itemIndex, LCD_WIDTH - 2) * (MENU_FONT_WIDTH);
      while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
      lcd_put_wchar(LCD_PIXEL_WIDTH - (MENU_FONT_WIDTH), row_y2, post_char);
      lcd_put_wchar(' ');
    }
  }

  // Draw a menu item with an editable value
  void MenuEditItemBase::draw(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm) {
    if (mark_as_selected(row, sel)) {
      const uint8_t vallen = (pgm ? utf8_strlen_P(data) : utf8_strlen((char*)data));
      u8g_uint_t n = lcd_put_u8str_ind_P(pstr, itemIndex, LCD_WIDTH - 2 - vallen) * (MENU_FONT_WIDTH);
      if (vallen) {
        lcd_put_wchar(':');
        while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
        lcd_moveto(LCD_PIXEL_WIDTH - (MENU_FONT_WIDTH) * vallen, row_y2);
        if (pgm) lcd_put_u8str_P(data); else lcd_put_u8str((char*)data);
      }
    }
  }

  void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char* const value/*=nullptr*/) {
    ui.encoder_direction_normal();

    const u8g_uint_t labellen = utf8_strlen_P(pstr), vallen = utf8_strlen(value);
    bool extra_row = labellen > LCD_WIDTH - 2 - vallen;

    #if ENABLED(USE_BIG_EDIT_FONT)
      // Use the menu font if the label won't fit on a single line
      constexpr u8g_uint_t lcd_edit_width = (LCD_PIXEL_WIDTH) / (EDIT_FONT_WIDTH);
      u8g_uint_t lcd_chr_fit, one_chr_width;
      if (labellen <= lcd_edit_width - 1) {
        if (labellen + vallen + 1 > lcd_edit_width) extra_row = true;
        lcd_chr_fit = lcd_edit_width + 1;
        one_chr_width = EDIT_FONT_WIDTH;
        ui.set_font(FONT_EDIT);
      }
      else {
        lcd_chr_fit = LCD_WIDTH;
        one_chr_width = MENU_FONT_WIDTH;
        ui.set_font(FONT_MENU);
      }
    #else
      constexpr u8g_uint_t lcd_chr_fit = LCD_WIDTH,
                           one_chr_width = MENU_FONT_WIDTH;
    #endif

    // Center the label and value lines on the middle line
    u8g_uint_t baseline = extra_row ? (LCD_PIXEL_HEIGHT) / 2 - 1
                                    : (LCD_PIXEL_HEIGHT + EDIT_FONT_ASCENT) / 2;

    // Assume the label is alpha-numeric (with a descender)
    bool onpage = PAGE_CONTAINS(baseline - (EDIT_FONT_ASCENT - 1), baseline + EDIT_FONT_DESCENT);
    if (onpage) lcd_put_u8str_ind_P(0, baseline, pstr, itemIndex);

    // If a value is included, print a colon, then print the value right-justified
    if (value != nullptr) {
      lcd_put_wchar(':');
      if (extra_row) {
        // Assume that value is numeric (with no descender)
        baseline += EDIT_FONT_ASCENT + 2;
        onpage = PAGE_CONTAINS(baseline - (EDIT_FONT_ASCENT - 1), baseline);
      }
      if (onpage) {
        lcd_put_wchar(((lcd_chr_fit - 1) - (vallen + 1)) * one_chr_width, baseline, ' '); // Right-justified, padded, add a leading space
        lcd_put_u8str(value);
      }
    }
  }

  inline void draw_boxed_string(const u8g_uint_t x, const u8g_uint_t y, PGM_P const pstr, const bool inv) {
    const u8g_uint_t len = utf8_strlen_P(pstr), bw = len * (MENU_FONT_WIDTH),
                     bx = x * (MENU_FONT_WIDTH), by = (y + 1) * (MENU_FONT_HEIGHT);
    if (inv) {
      u8g.setColorIndex(1);
      u8g.drawBox(bx - 1, by - (MENU_FONT_ASCENT) + 1, bw + 2, MENU_FONT_HEIGHT - 1);
      u8g.setColorIndex(0);
    }
    lcd_put_u8str_P(bx, by, pstr);
    if (inv) u8g.setColorIndex(1);
  }

  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
    ui.draw_select_screen_prompt(pref, string, suff);
    draw_boxed_string(1, LCD_HEIGHT - 1, no, !yesno);
    draw_boxed_string(LCD_WIDTH - (utf8_strlen_P(yes) + 1), LCD_HEIGHT - 1, yes, yesno);
  }

  #if ENABLED(SDSUPPORT)

    void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
      if (mark_as_selected(row, sel)) {
        if (isDir) lcd_put_wchar(LCD_STR_FOLDER[0]);
        constexpr uint8_t maxlen = LCD_WIDTH - 1;
        const u8g_uint_t pixw = maxlen * (MENU_FONT_WIDTH);
        u8g_uint_t n = pixw - lcd_put_u8str_max(ui.scrolled_filename(theCard, maxlen, row, sel), pixw);
        while (n > MENU_FONT_WIDTH) n -= lcd_put_wchar(' ');
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
      u8g_uint_t x_map_pixels = ((MAP_MAX_PIXELS_X - 4) / (GRID_MAX_POINTS_X)) * (GRID_MAX_POINTS_X),
                 y_map_pixels = ((MAP_MAX_PIXELS_Y - 4) / (GRID_MAX_POINTS_Y)) * (GRID_MAX_POINTS_Y),

              pixels_per_x_mesh_pnt = x_map_pixels / (GRID_MAX_POINTS_X),
              pixels_per_y_mesh_pnt = y_map_pixels / (GRID_MAX_POINTS_Y),

              x_offset = MAP_UPPER_LEFT_CORNER_X + 1 + (MAP_MAX_PIXELS_X - x_map_pixels - 2) / 2,
              y_offset = MAP_UPPER_LEFT_CORNER_Y + 1 + (MAP_MAX_PIXELS_Y - y_map_pixels - 2) / 2;

      // Clear the Mesh Map

      if (PAGE_CONTAINS(y_offset - 2, y_offset + y_map_pixels + 4)) {
        u8g.setColorIndex(1);  // First draw the bigger box in White so we have a border around the mesh map box
        u8g.drawBox(x_offset - 2, y_offset - 2, x_map_pixels + 4, y_map_pixels + 4);
        if (PAGE_CONTAINS(y_offset, y_offset + y_map_pixels)) {
          u8g.setColorIndex(0);  // Now actually clear the mesh map box
          u8g.drawBox(x_offset, y_offset, x_map_pixels, y_map_pixels);
        }
      }

      // Display Mesh Point Locations

      u8g.setColorIndex(1);
      const u8g_uint_t sx = x_offset + pixels_per_x_mesh_pnt / 2;
            u8g_uint_t  y = y_offset + pixels_per_y_mesh_pnt / 2;
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++, y += pixels_per_y_mesh_pnt)
        if (PAGE_CONTAINS(y, y))
          for (uint8_t i = 0, x = sx; i < GRID_MAX_POINTS_X; i++, x += pixels_per_x_mesh_pnt)
            u8g.drawBox(x, y, 1, 1);

      // Fill in the Specified Mesh Point

      const uint8_t y_plot_inv = (GRID_MAX_POINTS_Y - 1) - y_plot;  // The origin is typically in the lower right corner.  We need to
                                                                    // invert the Y to get it to plot in the right location.

      const u8g_uint_t by = y_offset + y_plot_inv * pixels_per_y_mesh_pnt;
      if (PAGE_CONTAINS(by, by + pixels_per_y_mesh_pnt))
        u8g.drawBox(
          x_offset + x_plot * pixels_per_x_mesh_pnt, by,
          pixels_per_x_mesh_pnt, pixels_per_y_mesh_pnt
        );

      // Put Relevant Text on Display

      // Show X and Y positions at top of screen
      u8g.setColorIndex(1);
      if (PAGE_UNDER(7)) {
        const xy_pos_t pos = { ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot) },
                       lpos = pos.asLogical();
        lcd_put_u8str(5, 7, "X:");
        lcd_put_u8str(ftostr52(lpos.x));
        lcd_put_u8str(74, 7, "Y:");
        lcd_put_u8str(ftostr52(lpos.y));
      }

      // Print plot position
      if (PAGE_CONTAINS(LCD_PIXEL_HEIGHT - (INFO_FONT_HEIGHT - 1), LCD_PIXEL_HEIGHT)) {
        lcd_put_wchar(5, LCD_PIXEL_HEIGHT, '(');
        u8g.print(x_plot);
        lcd_put_wchar(',');
        u8g.print(y_plot);
        lcd_put_wchar(')');

        // Show the location value
        lcd_put_u8str(74, LCD_PIXEL_HEIGHT, "Z:");
        if (!isnan(ubl.z_values[x_plot][y_plot]))
          lcd_put_u8str(ftostr43sign(ubl.z_values[x_plot][y_plot]));
        else
          lcd_put_u8str_P(PSTR(" -----"));
      }

    }

  #endif // AUTO_BED_LEVELING_UBL

  #if EITHER(BABYSTEP_ZPROBE_GFX_OVERLAY, MESH_EDIT_GFX_OVERLAY)

    const unsigned char cw_bmp[] PROGMEM = {
      B00000000,B11111110,B00000000,
      B00000011,B11111111,B10000000,
      B00000111,B11000111,B11000000,
      B00000111,B00000001,B11100000,
      B00000000,B00000000,B11100000,
      B00000000,B00000000,B11110000,
      B00000000,B00000000,B01110000,
      B00000100,B00000000,B01110000,
      B00001110,B00000000,B01110000,
      B00011111,B00000000,B01110000,
      B00111111,B10000000,B11110000,
      B00001110,B00000000,B11100000,
      B00001111,B00000001,B11100000,
      B00000111,B11000111,B11000000,
      B00000011,B11111111,B10000000,
      B00000000,B11111110,B00000000
    };

    const unsigned char ccw_bmp[] PROGMEM = {
      B00000000,B11111110,B00000000,
      B00000011,B11111111,B10000000,
      B00000111,B11000111,B11000000,
      B00001111,B00000001,B11100000,
      B00001110,B00000000,B11100000,
      B00111111,B10000000,B11110000,
      B00011111,B00000000,B01110000,
      B00001110,B00000000,B01110000,
      B00000100,B00000000,B01110000,
      B00000000,B00000000,B01110000,
      B00000000,B00000000,B11110000,
      B00000000,B00000000,B11100000,
      B00000111,B00000001,B11100000,
      B00000111,B11000111,B11000000,
      B00000011,B11111111,B10000000,
      B00000000,B11111110,B00000000
    };

    const unsigned char up_arrow_bmp[] PROGMEM = {
      B00000100,B00000000,
      B00001110,B00000000,
      B00011111,B00000000,
      B00111111,B10000000,
      B01111111,B11000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000
    };

    const unsigned char down_arrow_bmp[] PROGMEM = {
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B00001110,B00000000,
      B01111111,B11000000,
      B00111111,B10000000,
      B00011111,B00000000,
      B00001110,B00000000,
      B00000100,B00000000
    };

    const unsigned char offset_bedline_bmp[] PROGMEM = {
      B11111111,B11111111,B11111111
    };

    const unsigned char nozzle_bmp[] PROGMEM = {
      B01111111,B10000000,
      B11111111,B11000000,
      B11111111,B11000000,
      B11111111,B11000000,
      B01111111,B10000000,
      B01111111,B10000000,
      B11111111,B11000000,
      B11111111,B11000000,
      B11111111,B11000000,
      B00111111,B00000000,
      B00011110,B00000000,
      B00001100,B00000000
    };

    void _lcd_zoffset_overlay_gfx(const float zvalue) {
      // Determine whether the user is raising or lowering the nozzle.
      static int8_t dir;
      static float old_zvalue;
      if (zvalue != old_zvalue) {
        dir = zvalue ? zvalue < old_zvalue ? -1 : 1 : 0;
        old_zvalue = zvalue;
      }

      #if ENABLED(OVERLAY_GFX_REVERSE)
        const unsigned char *rot_up = ccw_bmp, *rot_down = cw_bmp;
      #else
        const unsigned char *rot_up = cw_bmp, *rot_down = ccw_bmp;
      #endif

      #if ENABLED(USE_BIG_EDIT_FONT)
        const int left = 0, right = 45, nozzle = 95;
      #else
        const int left = 5, right = 90, nozzle = 60;
      #endif

      // Draw a representation of the nozzle
      if (PAGE_CONTAINS(3, 16))  u8g.drawBitmapP(nozzle + 6, 4 - dir, 2, 12, nozzle_bmp);
      if (PAGE_CONTAINS(20, 20)) u8g.drawBitmapP(nozzle + 0, 20, 3, 1, offset_bedline_bmp);

      // Draw cw/ccw indicator and up/down arrows.
      if (PAGE_CONTAINS(47, 62)) {
        u8g.drawBitmapP(right + 0, 48 - dir, 2, 13, up_arrow_bmp);
        u8g.drawBitmapP(left  + 0, 49 - dir, 2, 13, down_arrow_bmp);
        u8g.drawBitmapP(left  + 13, 47, 3, 16, rot_down);
        u8g.drawBitmapP(right + 13, 47, 3, 16, rot_up);
      }
    }

  #endif // BABYSTEP_ZPROBE_GFX_OVERLAY || MESH_EDIT_GFX_OVERLAY

#endif // HAS_LCD_MENU

#endif // HAS_GRAPHICAL_LCD
