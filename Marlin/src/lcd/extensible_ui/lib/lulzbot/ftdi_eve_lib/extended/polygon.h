/*************
 * polygon.h *
 *************/

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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

/**
 * The Polygon class helps drawing filled or stroked polygons on the FTDI EVE:
 *
 *   CommandProcessor cmd;
 *   cmd.cmd(COLOR_RGB(0x00FF00));
 *
 *   Polygon p(cmd);
 *   p.begin_fill();
 *     p.begin_loop();
 *      p(10,10);
 *      p(20,10);
 *      p(20,20);
 *      p(10,20);
 *     p.end_loop();
 *     p.begin_loop();
 *        ...  // Additional closed paths
 *     p.end_loop();
 *        ...
 *   p.end_fill();
 *
 * Based on the example from "Applicaton Note AN_334, FT801 Polygon Application":
 *
 *   https://brtchip.com/wp-content/uploads/Support/Documentation/Application_Notes/ICs/EVE/AN_334-FT801_Polygon_Application.pdf
 */

namespace FTDI {
  class Polygon {
    private:
      FTDI::begin_t path_initiator = FTDI::LINE_STRIP;

    public:
      CommandProcessor &cmd;

      Polygon(CommandProcessor &c) : cmd(c) {}

      void begin_fill() {
        using namespace FTDI;
        cmd.cmd(SAVE_CONTEXT());
        cmd.cmd(TAG_MASK(0));
        cmd.cmd(CLEAR(0,1,0));
        cmd.cmd(COLOR_MASK(0,0,0,0));
        cmd.cmd(STENCIL_OP(STENCIL_OP_KEEP, STENCIL_OP_INVERT));
        cmd.cmd(STENCIL_FUNC(STENCIL_FUNC_ALWAYS, 255, 255));
        // Drawing the edge strip along scan lines
        // seems to yield the best performance
        #ifdef TOUCH_UI_PORTRAIT
          path_initiator = EDGE_STRIP_B;
        #else
          path_initiator = EDGE_STRIP_R;
        #endif
      }

      // Specify a clipping rectangle to paint fewer pixels and reduce rendering time, otherwise all pixels will be painted.
      void end_fill(const int16_t x1 = 0, const int16_t y1 = 0, const int16_t x2 = display_width * 16, const int16_t y2 = display_height * 16) {
        using namespace FTDI;
        cmd.cmd(RESTORE_CONTEXT());

        cmd.cmd(SAVE_CONTEXT());
        cmd.cmd(STENCIL_FUNC(STENCIL_FUNC_NOTEQUAL, 0, 255));
        cmd.cmd(BEGIN(RECTS));
        cmd.cmd(VERTEX2F(x1, y1));
        cmd.cmd(VERTEX2F(x2, y2));
        cmd.cmd(RESTORE_CONTEXT());
      }

      void begin_stroke() {path_initiator = FTDI::LINE_STRIP;}
      void begin_loop()   {cmd.cmd(FTDI::BEGIN(path_initiator));}
      void end_stroke()   {}
      void end_loop()     {}

      void operator()(const uint16_t x, const uint16_t y) {cmd.cmd(FTDI::VERTEX2F(x, y));}
  };
}
