/*****************
 * grid_layout.h *
 *****************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

#pragma once

/* The grid layout macros allow buttons to be arranged on a grid so
 * that their locations become independent of the display size. The
 * layout model is similar to that of HTML TABLEs.
 *
 * These macros are meant to be evaluated into constants at compile
 * time, so resolution independence can be as efficient as using
 * hard-coded coordinates.
 */

// Margin defines the margin (in pixels) on each side of a button in
// the layout

#ifdef TOUCH_UI_800x480
  #define MARGIN_L         5
  #define MARGIN_R         5
  #define MARGIN_T         5
  #define MARGIN_B         5
  #define MARGIN_DEFAULT   5
#else
  #define MARGIN_L         3
  #define MARGIN_R         3
  #define MARGIN_T         3
  #define MARGIN_B         3
  #define MARGIN_DEFAULT   3
#endif

// EDGE_R adds some black space on the right edge of the display
// This shifts some of the screens left to visually center them.

#define EDGE_R           0

// GRID_X and GRID_Y computes the positions of the divisions on
// the layout grid.
#define GRID_X(x)        ((x)*(FTDI::display_width-EDGE_R)/GRID_COLS)
#define GRID_Y(y)        ((y)*FTDI::display_height/GRID_ROWS)

// BTN_X, BTN_Y, BTN_W and BTN_X returns the top-left and width
// and height of a button, taking into account the button margins.

#define BTN_X(x)         (GRID_X((x)-1) + MARGIN_L)
#define BTN_Y(y)         (GRID_Y((y)-1) + MARGIN_T)
#define BTN_W(w)         (GRID_X(w)   - MARGIN_L - MARGIN_R)
#define BTN_H(h)         (GRID_Y(h)   - MARGIN_T - MARGIN_B)

// Abbreviations for common phrases, to allow a button to be
// defined in one line of source.
#define BTN_POS(x,y)     BTN_X(x), BTN_Y(y)
#define BTN_SIZE(w,h)    BTN_W(w), BTN_H(h)

// Draw a reference grid for ease of spacing out widgets.
#define DRAW_LAYOUT_GRID \
  { \
    cmd.cmd(LINE_WIDTH(4)); \
    for (int i = 1; i <= GRID_COLS; i++) { \
      cmd.cmd(BEGIN(LINES)); \
      cmd.cmd(VERTEX2F(GRID_X(i) *16, 0             *16)); \
      cmd.cmd(VERTEX2F(GRID_X(i) *16, FTDI::display_height *16)); \
    } \
    for (int i = 1; i < GRID_ROWS; i++) { \
      cmd.cmd(BEGIN(LINES)); \
      cmd.cmd(VERTEX2F(0                       *16, GRID_Y(i) *16)); \
      cmd.cmd(VERTEX2F(FTDI::display_width     *16, GRID_Y(i) *16)); \
    } \
    cmd.cmd(LINE_WIDTH(16)); \
  }

namespace FTDI {
  #ifdef TOUCH_UI_PORTRAIT
    constexpr uint16_t display_width  = Vsize;
    constexpr uint16_t display_height = Hsize;
  #else
    constexpr uint16_t display_width  = Hsize;
    constexpr uint16_t display_height = Vsize;
  #endif
}
