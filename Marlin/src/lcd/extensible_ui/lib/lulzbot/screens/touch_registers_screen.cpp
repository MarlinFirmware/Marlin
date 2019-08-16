/******************************
 * touch_registers_screen.cpp *
 ******************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && ENABLED(DEVELOPER_SCREENS)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void TouchRegistersScreen::onRedraw(draw_mode_t) {
   const uint32_t T_Transform_A = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_A);
   const uint32_t T_Transform_B = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_B);
   const uint32_t T_Transform_C = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_C);
   const uint32_t T_Transform_D = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_D);
   const uint32_t T_Transform_E = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_E);
   const uint32_t T_Transform_F = CLCD::mem_read_32(CLCD::REG::TOUCH_TRANSFORM_F);
   char b[20];

   CommandProcessor cmd;
   cmd.cmd(CLEAR_COLOR_RGB(bg_color))
      .cmd(CLEAR(true,true,true))
      .tag(0);

   #define GRID_ROWS 7
   #define GRID_COLS 2
   cmd.tag(0)
      .font(font_xsmall)
      .fgcolor(transformA)  .button( BTN_POS(1,1), BTN_SIZE(1,1), F("TOUCH_XFORM_A"))
      .fgcolor(transformB)  .button( BTN_POS(1,2), BTN_SIZE(1,1), F("TOUCH_XFORM_B"))
      .fgcolor(transformC)  .button( BTN_POS(1,3), BTN_SIZE(1,1), F("TOUCH_XFORM_C"))
      .fgcolor(transformD)  .button( BTN_POS(1,4), BTN_SIZE(1,1), F("TOUCH_XFORM_D"))
      .fgcolor(transformE)  .button( BTN_POS(1,5), BTN_SIZE(1,1), F("TOUCH_XFORM_E"))
      .fgcolor(transformF)  .button( BTN_POS(1,6), BTN_SIZE(1,1), F("TOUCH_XFORM_F"))

      .fgcolor(transformVal).button( BTN_POS(2,1), BTN_SIZE(1,1), F(""), OPT_FLAT)
      .fgcolor(transformVal).button( BTN_POS(2,2), BTN_SIZE(1,1), F(""), OPT_FLAT)
      .fgcolor(transformVal).button( BTN_POS(2,3), BTN_SIZE(1,1), F(""), OPT_FLAT)
      .fgcolor(transformVal).button( BTN_POS(2,4), BTN_SIZE(1,1), F(""), OPT_FLAT)
      .fgcolor(transformVal).button( BTN_POS(2,5), BTN_SIZE(1,1), F(""), OPT_FLAT)
      .fgcolor(transformVal).button( BTN_POS(2,6), BTN_SIZE(1,1), F(""), OPT_FLAT);

   sprintf_P(b, PSTR("0x%08lX"), T_Transform_A); cmd.text( BTN_POS(2,1), BTN_SIZE(1,1), b);
   sprintf_P(b, PSTR("0x%08lX"), T_Transform_B); cmd.text( BTN_POS(2,2), BTN_SIZE(1,1), b);
   sprintf_P(b, PSTR("0x%08lX"), T_Transform_C); cmd.text( BTN_POS(2,3), BTN_SIZE(1,1), b);
   sprintf_P(b, PSTR("0x%08lX"), T_Transform_D); cmd.text( BTN_POS(2,4), BTN_SIZE(1,1), b);
   sprintf_P(b, PSTR("0x%08lX"), T_Transform_E); cmd.text( BTN_POS(2,5), BTN_SIZE(1,1), b);
   sprintf_P(b, PSTR("0x%08lX"), T_Transform_F); cmd.text( BTN_POS(2,6), BTN_SIZE(1,1), b);

   cmd.colors(action_btn).font(font_medium)
      .tag(1).button( BTN_POS(2,7), BTN_SIZE(1,1), F("Back"));
   #undef GRID_COLS
   #undef GRID_ROWS
 }

 bool TouchRegistersScreen::onTouchEnd(uint8_t tag) {
   switch (tag) {
     case 1:        GOTO_PREVIOUS();                 break;
     default:
       return false;
   }
   return true;
 }

#endif // LULZBOT_TOUCH_UI
