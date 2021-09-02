/**************
 * arrows.cpp *
 **************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver 3D                          *
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

#define COORD(X,Y) cx + s*(swapXY ? Y : (flipX ? -X : X)), cy + s*(swapXY ? (flipX ? -X : X) : Y)

namespace FTDI {

  void drawArrow(int x, int y, int w, int h, Direction direction) {
    const bool swapXY = direction == UP || direction == DOWN;
    const bool flipX  = direction == UP || direction == LEFT;
    const int s  = min(w,h);
    const int cx = (x + w/2)*16;
    const int cy = (y + h/2)*16;

    CommandProcessor cmd;
    cmd.cmd(SAVE_CONTEXT())
       .cmd(LINE_WIDTH(s/2))
       .cmd(BEGIN(LINES))
       .cmd(VERTEX2F(COORD( 5, 0)))
       .cmd(VERTEX2F(COORD( 2,-2)))
       .cmd(VERTEX2F(COORD( 5, 0)))
       .cmd(VERTEX2F(COORD( 2, 2)))
       .cmd(VERTEX2F(COORD( 5, 0)))
       .cmd(VERTEX2F(COORD(-5, 0)))
       .cmd(RESTORE_CONTEXT());
  }

} // namespace FTDI

#endif // FTDI_EXTENDED
