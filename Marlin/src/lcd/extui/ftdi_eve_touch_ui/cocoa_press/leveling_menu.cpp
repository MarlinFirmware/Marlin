/*********************************
 * cocoa_press/leveling_menu.cpp *
 *********************************/

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

#if ENABLED(COCOA_LEVELING_MENU)

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#if ANY(MESH_BED_LEVELING, AUTO_BED_LEVELING_UBL)
  #define GRID_COLS 3
  #define GRID_ROWS 6
  #define BED_MESH_TITLE_POS BTN_POS(1,1), BTN_SIZE(3,1)
  #define WARNING_POS        BTN_POS(1,2), BTN_SIZE(3,2)
  #define PROBE_BED_POS      BTN_POS(1,4), BTN_SIZE(1,1)
  #define SHOW_MESH_POS      BTN_POS(2,4), BTN_SIZE(1,1)
  #define EDIT_MESH_POS      BTN_POS(3,4), BTN_SIZE(1,1)
  #define BACK_POS           BTN_POS(1,6), BTN_SIZE(3,1)
#else
  #define GRID_COLS 2
  #define GRID_ROWS 6
  #define BED_MESH_TITLE_POS BTN_POS(1,1), BTN_SIZE(2,1)
  #define WARNING_POS        BTN_POS(1,2), BTN_SIZE(2,2)
  #define PROBE_BED_POS      BTN_POS(1,4), BTN_SIZE(1,1)
  #define SHOW_MESH_POS      BTN_POS(2,4), BTN_SIZE(1,1)
  #define BACK_POS           BTN_POS(1,6), BTN_SIZE(2,1)

  // Hide the editor button if motion to grid point not supported
  #define EDIT_MESH_POS      BTN_POS(4,7), BTN_SIZE(1,1)
#endif

void LevelingMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_large)
       .cmd(COLOR_RGB(bg_text_enabled))
       .text(BED_MESH_TITLE_POS, GET_TEXT_F(MSG_BED_LEVELING))
       .font(font_medium).colors(normal_btn)
       .tag(2).button(PROBE_BED_POS, GET_TEXT_F(MSG_PROBE_BED))
              .enabled(ENABLED(HAS_MESH))
       .tag(3).button(SHOW_MESH_POS, GET_TEXT_F(MSG_MESH_VIEW))
              .enabled(ENABLED(HAS_MESH))
       .tag(4).button(EDIT_MESH_POS, GET_TEXT_F(MSG_EDIT_MESH))
       .colors(action_btn)
       .tag(1).button(BACK_POS, GET_TEXT_F(MSG_BUTTON_DONE))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0);
    draw_text_box(cmd, WARNING_POS, F("Remove chocolate cartridge before probing.  This reduces the possibility of damaging a part."), OPT_CENTER, font_medium);
  }
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    case 2: SaveSettingsDialogBox::settingsChanged(); injectCommands(F(BED_LEVELING_COMMANDS)); break;
    case 3: BedMeshViewScreen::show(); break;
    case 4: SaveSettingsDialogBox::settingsChanged(); BedMeshEditScreen::show(); break;
    default: return false;
  }
  return true;
}

#endif // COCOA_LEVELING_MENU
