/***************
 * unicode.cpp *
 ***************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../ftdi_extended.h"

#if BOTH(FTDI_EXTENDED, TOUCH_UI_USE_UTF8)

  using namespace FTDI;

  /**
   * Return true if a string has UTF8 characters
   *
   * Parameters:
   *
   *   c  - Pointer to a string.
   *
   * Returns: True if the strings has UTF8 characters
   */

  bool FTDI::has_utf8_chars(const char *str) {
    for (;;) {
      const char c = *str++;
      if (!c) break;
      if ((c & 0xC0) == 0x80) return true;
    }
    return false;
  }

  bool FTDI::has_utf8_chars(progmem_str _str) {
    const char *str = (const char *) _str;
    for (;;) {
      const char c = pgm_read_byte(str++);
      if (!c) break;
      if ((c & 0xC0) == 0x80) return true;
    }
    return false;
  }

  /**
   * Return a character in a UTF8 string and increment the
   * pointer to the next character
   *
   * Parameters:
   *
   *   c  - Pointer to a UTF8 encoded string.
   *
   * Returns: The packed bytes of a UTF8 encoding of a single
   *          character (this is not the unicode codepoint)
   */

  utf8_char_t FTDI::get_utf8_char_and_inc(char *&c) {
    utf8_char_t val = *(uint8_t*)c++;
    if ((val & 0xC0) == 0xC0)
      while ((*c & 0xC0) == 0x80)
        val = (val << 8) | *(uint8_t*)c++;
    return val;
  }

  utf8_char_t FTDI::get_utf8_char_and_inc(char *&c) {
    utf8_char_t val = *(uint8_t*)c++;
    if ((val & 0xC0) == 0xC0)
      while ((*c & 0xC0) == 0x80)
        val = (val << 8) | *(uint8_t*)c++;
    return val;
  }

  /**
   * Helper function to draw and/or measure a UTF8 string
   *
   * Parameters:
   *
   *   cmd  - If non-NULL the symbol is drawn to the screen.
   *          If NULL, only increment position for text measurement.
   *
   *   x, y - The location at which to draw the string.
   *
   *   str  - The UTF8 string to draw or measure.
   *
   *   fs   - A scaling object used to specify the font size.
   */

  static uint16_t render_utf8_text(CommandProcessor* cmd, int x, int y, const char *str, font_size_t fs, size_t maxlen=SIZE_MAX) {
    const int start_x = x;
    while (*str && maxlen--) {
      const utf8_char_t c = get_utf8_char_and_inc(str);
      #ifdef TOUCH_UI_UTF8_CYRILLIC_CHARSET
        CyrillicCharSet::render_glyph(cmd, x, y, fs, c) ||
      #endif
      #ifdef TOUCH_UI_UTF8_WESTERN_CHARSET
        WesternCharSet::render_glyph(cmd, x, y, fs, c) ||
      #endif
      StandardCharSet::render_glyph(cmd, x, y, fs, c);
    }
    return x - start_x;
  }

  /**
   * Load the font bitmap data into RAMG. Called once at program start.
   *
   * Parameters:
   *
   *   addr  - Address in RAMG where the font data is written
   */

  void FTDI::load_utf8_data(uint32_t addr) {
    #ifdef TOUCH_UI_UTF8_CYRILLIC_CHARSET
      addr = CyrillicCharSet::load_data(addr);
    #endif
    #ifdef TOUCH_UI_UTF8_WESTERN_CHARSET
      addr = WesternCharSet::load_data(addr);
    #endif
    addr = StandardCharSet::load_data(addr);
  }

  /**
   * Populate the bitmap handles for the custom fonts into the display list.
   * Called once at the start of each display list.
   *
   * Parameters:
   *
   *   cmd  - Object used for writing to the FTDI chip command queue.
   */

  void FTDI::load_utf8_bitmaps(CommandProcessor &cmd) {
    cmd.cmd(SAVE_CONTEXT());
    #ifdef TOUCH_UI_UTF8_CYRILLIC_CHARSET
      CyrillicCharSet::load_bitmaps(cmd);
    #endif
    #ifdef TOUCH_UI_UTF8_WESTERN_CHARSET
      WesternCharSet::load_bitmaps(cmd);
    #endif
    StandardCharSet::load_bitmaps(cmd);
    cmd.cmd(RESTORE_CONTEXT());
  }

   /**
    * Measure a UTF8 text character
    *
    * Parameters:
    *
    *   c    - The unicode code point to measure.
    *
    *   fs   - A scaling object used to specify the font size.
    *
    * Returns: A width in pixels
    */

  uint16_t FTDI::get_utf8_char_width(utf8_char_t c, font_size_t fs) {
    int x = 0, y = 0;
    #ifdef TOUCH_UI_UTF8_CYRILLIC_CHARSET
      CyrillicCharSet::render_glyph(nullptr, x, y, fs, c) ||
    #endif
    #ifdef TOUCH_UI_UTF8_WESTERN_CHARSET
      WesternCharSet::render_glyph(nullptr, x, y, fs, c) ||
    #endif
      StandardCharSet::render_glyph(nullptr, x, y, fs, c);
    return x;
  }

   /**
    * Measure a UTF8 text string
    *
    * Parameters:
    *
    *   str  - The UTF8 string to measure.
    *
    *   fs   - A scaling object used to specify the font size.
    *
    * Returns: A width in pixels
    */

  uint16_t FTDI::get_utf8_text_width(const char *str, font_size_t fs, size_t maxlen) {
    return render_utf8_text(nullptr, 0, 0, str, fs, maxlen);
  }

  uint16_t FTDI::get_utf8_text_width(progmem_str pstr, font_size_t fs) {
    char str[strlen_P((const char*)pstr) + 1];
    strcpy_P(str, (const char*)pstr);
    return get_utf8_text_width(str, fs);
  }

   /**
    * Draw a UTF8 text string
    *
    * Parameters:
    *
    *   cmd  - Object used for writing to the FTDI chip command queue.
    *
    *   x, y - The location at which to draw the string.
    *
    *   str  - The UTF8 string to draw.
    *
    *   fs   - A scaling object used to specify the font size.
    *
    *   options - Text alignment options (i.e. OPT_CENTERX, OPT_CENTERY, OPT_CENTER or OPT_RIGHTX)
    *
    *   maxlen - Maximum characters to draw
    */

  void FTDI::draw_utf8_text(CommandProcessor& cmd, int x, int y, const char *str, font_size_t fs, uint16_t options, size_t maxlen) {
    cmd.cmd(SAVE_CONTEXT());
    cmd.cmd(BITMAP_TRANSFORM_A(fs.get_coefficient()));
    cmd.cmd(BITMAP_TRANSFORM_E(fs.get_coefficient()));
    cmd.cmd(BEGIN(BITMAPS));

    // Apply alignment options
    if (options & OPT_CENTERX)
      x -= get_utf8_text_width(str, fs, maxlen) / 2;
    else if (options & OPT_RIGHTX)
      x -= get_utf8_text_width(str, fs, maxlen);
    if (options & OPT_CENTERY)
      y -= fs.get_height()/2;

    // Render the text
    render_utf8_text(&cmd, x, y, str, fs, maxlen);
    cmd.cmd(RESTORE_CONTEXT());
  }

  void FTDI::draw_utf8_text(CommandProcessor& cmd, int x, int y, progmem_str pstr, font_size_t fs, uint16_t options) {
    char str[strlen_P((const char*)pstr) + 1];
    strcpy_P(str, (const char*)pstr);
    draw_utf8_text(cmd, x, y, (const char*) str, fs, options);
  }

#endif // FTDI_EXTENDED && TOUCH_UI_USE_UTF8
