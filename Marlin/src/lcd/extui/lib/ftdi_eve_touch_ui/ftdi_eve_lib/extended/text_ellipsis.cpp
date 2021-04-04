/*********************
 * text_ellipsis.cpp *
 *********************/

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

#include "ftdi_extended.h"

#if ENABLED(FTDI_EXTENDED)

namespace FTDI {

  /**
   * Helper function for drawing text with ellipses. The str buffer may be modified and should have space for up to two extra characters.
   */
  static void _draw_text_with_ellipsis(CommandProcessor& cmd, int16_t x, int16_t y, int16_t w, int16_t h, char *str, uint16_t options, uint8_t font) {
    FontMetrics fm(font);
    const int16_t ellipsisWidth = fm.get_char_width('.') * 3;

    // Compute the total line length, as well as
    // the location in the string where it can
    // split and still allow the ellipsis to fit.
    int16_t lineWidth = 0;
    char *breakPoint   = str;
    #ifdef TOUCH_UI_USE_UTF8
      char *tstr = str;
      while (*tstr) {
        breakPoint = tstr;
        const utf8_char_t c = get_utf8_char_and_inc(tstr);
        lineWidth += fm.get_char_width(c);
        if (lineWidth + ellipsisWidth < w)
          break;
      }
    #else
      for (char *c = str; *c; c++) {
        lineWidth += fm.get_char_width(*c);
        if (lineWidth + ellipsisWidth < w)
          breakPoint = c;
      }
    #endif

    if (lineWidth > w) {
      *breakPoint = '\0';
      strcpy_P(breakPoint,PSTR("..."));
    }

    cmd.apply_text_alignment(x, y, w, h, options);
    #if ENABLED(TOUCH_UI_USE_UTF8)
      if (has_utf8_chars(str)) {
        draw_utf8_text(cmd, x, y, str, font_size_t::from_romfont(font), options);
      } else
    #endif
      {
        cmd.CLCD::CommandFifo::text(x, y, font, options);
        cmd.CLCD::CommandFifo::str(str);
      }
  }

  /**
    * These functions draws text inside a bounding box, truncating the text and
    * adding ellipsis if the text does not fit.
    */
  void draw_text_with_ellipsis(CommandProcessor& cmd, int x, int y, int w, int h, const char *str, uint16_t options, uint8_t font) {
    char tmp[strlen(str) + 3];
    strcpy(tmp, str);
    _draw_text_with_ellipsis(cmd, x, y, w, h, tmp, options, font);
  }

  void draw_text_with_ellipsis(CommandProcessor& cmd, int x, int y, int w, int h, progmem_str pstr, uint16_t options, uint8_t font) {
    char tmp[strlen_P((const char*)pstr) + 3];
    strcpy_P(tmp, (const char*)pstr);
    _draw_text_with_ellipsis(cmd, x, y, w, h, tmp, options, font);
  }
} // namespace FTDI

#endif // FTDI_EXTENDED
