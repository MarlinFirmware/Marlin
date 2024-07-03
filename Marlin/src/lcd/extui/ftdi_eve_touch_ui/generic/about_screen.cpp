/********************
 * about_screen.cpp *
 ********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Brian Kahl 2023 - FAME3D                                    *
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

#include "../config.h"
#include "../screens.h"
#include "../../ui_api.h"

#ifdef FTDI_ABOUT_SCREEN

#define GRID_COLS 4
#define GRID_ROWS 30

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void AboutScreen::onEntry() {
  BaseScreen::onEntry();
  sound.play(chimes, PLAY_ASYNCHRONOUS);
}

void AboutScreen::onRedraw(draw_mode_t) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(CLEAR(true,true,true))
     .cmd(COLOR_RGB(bg_text_enabled))
     .tag(0);

  draw_text_box(cmd, BTN_POS(1,1), BTN_SIZE(4,6), F(
      #if ENABLED(LULZBOT_LONG_BED)
        "" LULZBOT_LCD_MACHINE_NAME " \nWith Long Bed"
      #elif ENABLED(LULZBOT_LONG_BED_V2)
        "" LULZBOT_LCD_MACHINE_NAME " \nWith Long Bed V2"
      #elif ENABLED(LULZBOT_BLTouch) && NONE(LULZBOT_LONG_BED_V2, TAZProV2)
        "" LULZBOT_LCD_MACHINE_NAME " \nWith BLTouch"
      #else
        "" LULZBOT_LCD_MACHINE_NAME "\n"
      #endif
  ), OPT_CENTER, font_xxlarge);

  cmd.tag(3);
  draw_text_box(cmd, BTN_POS(1,7), BTN_SIZE(4,3), F(
        "Firmware:"
  ), OPT_CENTER, font_xlarge);

  draw_text_box(cmd, BTN_POS(1,10), BTN_SIZE(4,2), F(
         "" LULZBOT_M115_EXTRUDER_TYPE ""
  ), OPT_CENTER, font_xlarge);

  #if ENABLED(SHOW_TOOL_HEAD_ID)
    draw_text_box(cmd, BTN_POS(1,13), BTN_SIZE(4,3), F(
      "Tool Head:"
    ), OPT_CENTER, font_xlarge);

    switch(getToolHeadIdNumber()){
      case 1:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "M175"
      ), OPT_CENTER, font_large);
      break;
      case 2:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "SL"
      ), OPT_CENTER, font_large);
      break;
      case 3:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "SE"
      ), OPT_CENTER, font_large);
      break;
      case 4:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "HE"
      ), OPT_CENTER, font_large);
      break;
      case 5:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "HS"
      ), OPT_CENTER, font_large);
      break;
      case 6:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "HS+"
      ), OPT_CENTER, font_large);
      break;
      case 7:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "H175"
      ), OPT_CENTER, font_large);
      break;
      case 8:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "MET175"
      ), OPT_CENTER, font_large);
      break;
      case 9:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "MET285"
      ), OPT_CENTER, font_large);
      break;
      case 10:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "AST285"
      ), OPT_CENTER, font_large);
      break;
      case 11:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "Twin Nebula 175"
      ), OPT_CENTER, font_large);
      break;
      case 12:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "Twin Nebula 285"
      ), OPT_CENTER, font_large);
      break;
      case 13:
      draw_text_box(cmd, BTN_POS(1,16), BTN_SIZE(4,2), F(
        "Legacy Dual"
      ), OPT_CENTER, font_large);
      break;
    }
  #endif

  draw_text_box(cmd, BTN_POS(1,19), BTN_SIZE(4,3), F(
        "Version:"
  ), OPT_CENTER, font_xlarge);

  draw_text_box(cmd, BTN_POS(1,22), BTN_SIZE(4,2), F(
    "Marlin " SHORT_BUILD_VERSION ""
  ), OPT_CENTER, font_xlarge);


  cmd.font(font_medium).colors(normal_btn).tag(1).button(BTN_POS(1,24), BTN_SIZE(4,3), GET_TEXT_F(MSG_INFO_PRINTER_STATS_MENU));

  cmd.font(font_medium).colors(action_btn).tag(2).button(BTN_POS(1,27), BTN_SIZE(4,3), GET_TEXT_F(MSG_BUTTON_DONE));

}

bool AboutScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1: GOTO_SCREEN(StatisticsScreen);    break;
    case 2: GOTO_PREVIOUS();            return true;
    case 3: GOTO_SCREEN(DeveloperMenu); return true;
    default:                           return false;
  }
}

#endif // EXTENSIBLE_UI