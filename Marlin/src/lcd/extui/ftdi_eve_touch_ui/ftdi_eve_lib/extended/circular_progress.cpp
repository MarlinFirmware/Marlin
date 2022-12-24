/*************************
 * circular_progress.cpp *
 *************************/

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

/* This function draws a circular progress "ring" */
namespace FTDI {
  void draw_circular_progress(CommandProcessor& cmd, int x, int y, int w, int h, float percent, char *text, uint32_t bgcolor, uint32_t fgcolor) {
    const float rim = 0.3;
    const float a  = percent/100.0*2.0*PI;
    const float a1 = min(PI/2, a);
    const float a2 = min(PI/2, a-a1);
    const float a3 = min(PI/2, a-a1-a2);
    const float a4 = min(PI/2, a-a1-a2-a3);

    const int ro  = min(w,h) * 8;
    const int rr = ro * rim;
    const int cx = x * 16 + w * 8;
    const int cy = y * 16 + h * 8;

    // Load a rim shape into stencil buffer
    cmd.cmd(SAVE_CONTEXT());
    cmd.cmd(TAG_MASK(0));
    cmd.cmd(CLEAR(0,1,0));
    cmd.cmd(COLOR_MASK(0,0,0,0));
    cmd.cmd(STENCIL_OP(STENCIL_OP_KEEP, STENCIL_OP_INVERT));
    cmd.cmd(STENCIL_FUNC(STENCIL_FUNC_ALWAYS, 255, 255));
    cmd.cmd(BEGIN(POINTS));
    cmd.cmd(POINT_SIZE(ro));
    cmd.cmd(VERTEX2F(cx, cy));
    cmd.cmd(POINT_SIZE(ro - rr));
    cmd.cmd(VERTEX2F(cx, cy));
    cmd.cmd(RESTORE_CONTEXT());

    // Mask further drawing by stencil buffer
    cmd.cmd(SAVE_CONTEXT());
    cmd.cmd(STENCIL_FUNC(STENCIL_FUNC_NOTEQUAL, 0, 255));

    // Fill the background
    cmd.cmd(COLOR_RGB(bgcolor));
    cmd.cmd(BEGIN(POINTS));
    cmd.cmd(POINT_SIZE(ro));
    cmd.cmd(VERTEX2F(cx, cy));
    cmd.cmd(COLOR_RGB(fgcolor));

    // Paint upper-right quadrant
    cmd.cmd(BEGIN(EDGE_STRIP_A));
    cmd.cmd(VERTEX2F(cx, cy));
    cmd.cmd(VERTEX2F(cx + ro*sin(a1) + 16,cy - ro*cos(a1) + 8));

    // Paint lower-right quadrant
    if (a > PI/2) {
      cmd.cmd(BEGIN(EDGE_STRIP_R));
      cmd.cmd(VERTEX2F(cx, cy));
      cmd.cmd(VERTEX2F(cx + ro*cos(a2),cy + ro*sin(a2) + 16));
    }

    // Paint lower-left quadrant
    if (a > PI) {
      cmd.cmd(BEGIN(EDGE_STRIP_B));
      cmd.cmd(VERTEX2F(cx, cy));
      cmd.cmd(VERTEX2F(cx - ro*sin(a3) - 8,cy + ro*cos(a3)));
    }

    // Paint upper-left quadrant
    if (a > 1.5*PI) {
      cmd.cmd(BEGIN(EDGE_STRIP_L));
      cmd.cmd(VERTEX2F(cx, cy));
      cmd.cmd(VERTEX2F(cx - ro*cos(a4),cy - ro*sin(a4)));
    }
    cmd.cmd(RESTORE_CONTEXT());

    // Draw the text

    cmd.cmd(SAVE_CONTEXT());
    cmd.cmd(COLOR_RGB(fgcolor));
    cmd.text(x,y,w,h,text, OPT_CENTERX | OPT_CENTERY);
    cmd.cmd(RESTORE_CONTEXT());
  }

  void draw_circular_progress(CommandProcessor& cmd, int x, int y, int w, int h, float percent, uint32_t bgcolor, uint32_t fgcolor) {
    char str[5];
    sprintf(str,"%d\%%",int(percent));
    draw_circular_progress(cmd, x, y, w, h, percent, str, bgcolor, fgcolor);
  }
} // namespace FTDI

#endif // FTDI_EXTENDED
