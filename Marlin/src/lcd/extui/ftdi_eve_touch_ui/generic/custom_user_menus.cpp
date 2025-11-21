/*****************************
 * custom_user_menu.cpp *
 *****************************/

/****************************************************************************
 *   Written By Brian Kahl      2023 - FAME3D.                              *
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

#ifdef FTDI_CUSTOM_USER_MENUS

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define _ITEM_TAG(N) (10+N)
#define _USER_DESC(N) MAIN_MENU_ITEM_##N##_DESC
#define _USER_GCODE(N) MAIN_MENU_ITEM_##N##_GCODE
#define _USER_ITEM(N) .tag(_ITEM_TAG(N)).button(USER_ITEM_POS(N), _USER_DESC(N))
#define _USER_ACTION(N) case _ITEM_TAG(N): injectCommands(F(_USER_GCODE(N))); TERN_(CUSTOM_MENU_MAIN_SCRIPT_RETURN, GOTO_SCREEN(StatusScreen)); break;

void CustomUserMenus::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true, true, true));
  }

  #if ENABLED(TOUCH_UI_PORTRAIT)
    #ifdef TOOLHEAD_Legacy_Universal
      #define GRID_ROWS 10
    #else
      #define GRID_ROWS 7
    #endif
    #define GRID_COLS 1
    #define TOOLHEAD_LABL_POS BTN_POS(1, 1), BTN_SIZE(GRID_COLS,1)
    #define USER_ITEM_POS(N)  BTN_POS(1, N), BTN_SIZE(GRID_COLS,1)
    #define TOOLHEAD_SWAP_POS BTN_POS(1,GRID_ROWS-1), BTN_SIZE(GRID_COLS,1)
    #define BACK_POS          BTN_POS(1,GRID_ROWS), BTN_SIZE(GRID_COLS,1)
  #else
    #define GRID_ROWS 9
    #define GRID_COLS 1
    #define TOOLHEAD_LABL_POS BTN_POS(1, 1), BTN_SIZE(GRID_COLS,1)
    #define USER_ITEM_POS(N)  BTN_POS(1, N), BTN_SIZE(GRID_COLS,1)
    #define TOOLHEAD_SWAP_POS BTN_POS(1,GRID_ROWS-1), BTN_SIZE(GRID_COLS,1)
    #define BACK_POS          BTN_POS(1,GRID_ROWS), BTN_SIZE(GRID_COLS,1)
  #endif

  btn_colors thcolor[8] = {normal_btn};
  //.color(TH_color[1])

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
       .tag(0).text(TOOLHEAD_LABL_POS, GET_TEXT_F(MSG_CUSTOM_MENU_MAIN_TITLE));
    cmd.colors(accent_btn)
       .font(Theme::font_medium)
       #ifdef MAIN_MENU_ITEM_1_DESC
        //_USER_ITEM(1)
        .tag(_ITEM_TAG(11)).button(USER_ITEM_POS(1), MAIN_MENU_ITEM_1_DESC)
       #endif
      .colors(thcolor[1])
       #ifdef MAIN_MENU_ITEM_2_DESC
        //_USER_ITEM(2)
        .tag(_ITEM_TAG(12)).button(USER_ITEM_POS(2), MAIN_MENU_ITEM_2_DESC)
       #endif
       .colors(thcolor[2])
       #ifdef MAIN_MENU_ITEM_3_DESC
        //_USER_ITEM(3)
        .tag(_ITEM_TAG(13)).button(USER_ITEM_POS(3), MAIN_MENU_ITEM_3_DESC)
       #endif
       .colors(thcolor[3])
       #ifdef MAIN_MENU_ITEM_4_DESC
        //_USER_ITEM(4)
        .tag(_ITEM_TAG(14)).button(USER_ITEM_POS(4), MAIN_MENU_ITEM_4_DESC)
       #endif
       .colors(thcolor[4])
       #ifdef MAIN_MENU_ITEM_5_DESC
        //_USER_ITEM(5)
        .tag(_ITEM_TAG(15)).button(USER_ITEM_POS(5), MAIN_MENU_ITEM_5_DESC)
       #endif
       .colors(thcolor[5])
       #ifdef MAIN_MENU_ITEM_6_DESC
        //_USER_ITEM(6)
        .tag(_ITEM_TAG(16)).button(USER_ITEM_POS(6), MAIN_MENU_ITEM_6_DESC)
       #endif
       .colors(thcolor[6])
       #ifdef MAIN_MENU_ITEM_7_DESC
        //_USER_ITEM(7)
        .tag(_ITEM_TAG(17)).button(USER_ITEM_POS(7), MAIN_MENU_ITEM_7_DESC)
       #endif
       .colors(thcolor[7])
       #ifdef MAIN_MENU_ITEM_8_DESC
        //_USER_ITEM(8)
        .tag(_ITEM_TAG(18)).button(USER_ITEM_POS(8), MAIN_MENU_ITEM_8_DESC)
       #endif

       #if DISABLED(TOOLHEAD_Legacy_Universal)
        #undef GRID_ROWS
        #define GRID_ROWS 8
       #endif
      #ifdef PARKING_COMMAND_GCODE
        .tag(20).colors(normal_btn).button(TOOLHEAD_SWAP_POS, GET_TEXT_F(MSG_TOOL_HEAD_SWAP))
      #endif
      .tag(1).colors(action_btn).button(BACK_POS, GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

#include "../../../../gcode/queue.h"

template<bool> void _lcd_custom_menu_gcode(FSTR_P const fstr);

FORCE_INLINE void _lcd_custom_menu_gcode_done() {
  TERN_(CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK, ui.completion_feedback());
  TERN_(CUSTOM_MENU_MAIN_SCRIPT_RETURN, ui.return_to_status());
}
template<> void _lcd_custom_menu_gcode<true>(FSTR_P const fstr) {
  gcode.process_subcommands_now(fstr);
  _lcd_custom_menu_gcode_done();
}
template<> void _lcd_custom_menu_gcode<false>(FSTR_P const fstr) {
  queue.inject(fstr);
  _lcd_custom_menu_gcode_done();
}

bool CustomUserMenus::onTouchEnd(uint8_t tag) {
  switch (tag) {
    #ifdef MAIN_MENU_ITEM_1_DESC
      case _ITEM_TAG(11): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_1_IMMEDIATE)>(F(MAIN_MENU_ITEM_1_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_2_DESC
      case _ITEM_TAG(12): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_2_IMMEDIATE)>(F(MAIN_MENU_ITEM_2_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_3_DESC
      case _ITEM_TAG(13): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_3_IMMEDIATE)>(F(MAIN_MENU_ITEM_3_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_4_DESC
      case _ITEM_TAG(14): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_4_IMMEDIATE)>(F(MAIN_MENU_ITEM_4_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_5_DESC
      case _ITEM_TAG(15): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_5_IMMEDIATE)>(F(MAIN_MENU_ITEM_5_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_6_DESC
      case _ITEM_TAG(16): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_6_IMMEDIATE)>(F(MAIN_MENU_ITEM_6_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_7_DESC
      case _ITEM_TAG(17): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_7_IMMEDIATE)>(F(MAIN_MENU_ITEM_7_GCODE)); break;
    #endif
    #ifdef MAIN_MENU_ITEM_8_DESC
      case _ITEM_TAG(18): _lcd_custom_menu_gcode<ENABLED(MAIN_MENU_ITEM_8_IMMEDIATE)>(F(MAIN_MENU_ITEM_8_GCODE)); break;
    #endif
    case 1: GOTO_PREVIOUS(); break;
    #ifdef PARKING_COMMAND_GCODE
      case 20: injectCommands(F(PARKING_COMMAND_GCODE)); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // FTDI_CUSTOM_USER_MENUS
