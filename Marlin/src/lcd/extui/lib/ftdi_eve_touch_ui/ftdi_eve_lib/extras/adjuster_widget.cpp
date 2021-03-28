/***********************
 * adjuster_widget.cpp *
 ***********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - Cocoa Press                          *
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

#include "../ftdi_eve_lib.h"
#include "../extended/grid_layout.h"

#include "adjuster_widget.h"

#define SUB_COLS          9
#define SUB_ROWS          1
#define VAL_POS           SUB_POS(1,1), SUB_SIZE(5,1)
#define INC_POS           SUB_POS(6,1), SUB_SIZE(2,1)
#define DEC_POS           SUB_POS(8,1), SUB_SIZE(2,1)

void draw_adjuster_value(CommandProcessor& cmd, int16_t x, int16_t y, int16_t w, int16_t h, float value, progmem_str units, int8_t width, uint8_t precision) {
  char str[width + precision + 10 + (units ? strlen_P((const char*) units) : 0)];
  if (isnan(value))
    strcpy_P(str, PSTR("-"));
  else
    dtostrf(value, width, precision, str);

  if (units) {
    strcat_P(str, PSTR(" "));
    strcat_P(str, (const char*) units);
  }

  cmd.text(VAL_POS, str);
}

void draw_adjuster(CommandProcessor& cmd, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t tag, float value, progmem_str units, int8_t width, uint8_t precision, draw_mode_t what) {
  if (what & BACKGROUND)
    cmd.tag(0).button(VAL_POS, F(""), FTDI::OPT_FLAT);

  if (what & FOREGROUND) {
    draw_adjuster_value(cmd, x, y, w, h, value, units, width, precision);
    cmd.tag(tag  ).button(INC_POS, F("-"))
       .tag(tag+1).button(DEC_POS, F("+"));
  }
}
