/********************
 * about_screen.cpp *
 ********************/

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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"

#ifdef FTDI_ABOUT_SCREEN

#define GRID_COLS 4
#define GRID_ROWS 8

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

  #define HEADING_POS BTN_POS(1,1), BTN_SIZE(4,2)
  #define FW_VERS_POS BTN_POS(1,3), BTN_SIZE(4,1)
  #define FW_INFO_POS BTN_POS(1,4), BTN_SIZE(4,1)
  #define LICENSE_POS BTN_POS(1,5), BTN_SIZE(4,3)
  #define STATS_POS   BTN_POS(1,8), BTN_SIZE(2,1)
  #define BACK_POS    BTN_POS(3,8), BTN_SIZE(2,1)

  char about_str[1
    + strlen_P(GET_TEXT(MSG_ABOUT_TOUCH_PANEL_2))
    #ifdef TOOLHEAD_NAME
      + strlen_P(TOOLHEAD_NAME)
    #endif
  ];
  #ifdef TOOLHEAD_NAME
    // If MSG_ABOUT_TOUCH_PANEL_2 has %s, substitute in the toolhead name.
    // But this is optional, so squelch the compiler warning here.
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-extra-args"
    sprintf_P(about_str, GET_TEXT(MSG_ABOUT_TOUCH_PANEL_2), TOOLHEAD_NAME);
    #pragma GCC diagnostic pop
  #else
    strcpy_P(about_str, GET_TEXT(MSG_ABOUT_TOUCH_PANEL_2));
  #endif

  draw_text_box(cmd, HEADING_POS,
    #ifdef CUSTOM_MACHINE_NAME
      F(CUSTOM_MACHINE_NAME)
    #else
      GET_TEXT_F(MSG_ABOUT_TOUCH_PANEL_1)
    #endif
    , OPT_CENTER, font_xlarge
  );
  #if BOTH(TOUCH_UI_DEVELOPER_MENU, FTDI_DEVELOPER_MENU)
    cmd.tag(3);
  #endif
  draw_text_box(cmd, FW_VERS_POS,
  #ifdef TOUCH_UI_VERSION
    F(TOUCH_UI_VERSION)
  #else
    progmem_str(getFirmwareName_str())
  #endif
  , OPT_CENTER, font_medium);
  cmd.tag(0);
  draw_text_box(cmd, FW_INFO_POS, about_str, OPT_CENTER, font_medium);
  draw_text_box(cmd, LICENSE_POS, GET_TEXT_F(MSG_LICENSE), OPT_CENTER, font_tiny);

  cmd.font(font_medium);
  #if BOTH(PRINTCOUNTER, FTDI_STATISTICS_SCREEN)
    cmd.colors(normal_btn)
       .tag(2).button(STATS_POS, GET_TEXT_F(MSG_INFO_STATS_MENU));
  #endif
  cmd.colors(action_btn)
     .tag(1).button(BACK_POS,  GET_TEXT_F(MSG_BUTTON_DONE));
}

bool AboutScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    #if BOTH(PRINTCOUNTER, FTDI_STATISTICS_SCREEN)
      case 2: GOTO_SCREEN(StatisticsScreen); break;
    #endif
    #if BOTH(TOUCH_UI_DEVELOPER_MENU, FTDI_DEVELOPER_MENU)
      case 3: GOTO_SCREEN(DeveloperMenu); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // FTDI_ABOUT_SCREEN
