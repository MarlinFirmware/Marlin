/****************************
 * endstop_state_screen.cpp *
 ****************************/

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

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void EndstopStatesScreen::onEntry() {
  BaseScreen::onEntry();
  enable_emi_pins(true);
}

void EndstopStatesScreen::onExit() {
  BaseScreen::onExit();
  enable_emi_pins(false);
}

void EndstopStatesScreen::onRedraw(draw_mode_t) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(COLOR_RGB(bg_text_enabled))
     .cmd(CLEAR(true,true,true))
     .tag(0);

  #define GRID_ROWS 7
  #define GRID_COLS 6

  #define PIN_BTN(X,Y,PIN,LABEL)          button(BTN_POS(X,Y), BTN_SIZE(2,1), LABEL)
  #define PIN_ENABLED(LABEL,PIN,INV,X,Y)  cmd.enabled(1).colors(READ(PIN##_PIN) != INV ? action_btn : normal_btn).PIN_BTN(X,Y,PIN,LABEL);
  #define PIN_DISABLED(LABEL,PIN,INV,X,Y) cmd.enabled(0).PIN_BTN(X,Y,PIN,LABEL);

  #ifdef TOUCH_UI_PORTRAIT
  cmd.font(font_large)
  #else
  cmd.font(font_medium)
  #endif
     .text(BTN_POS(1,1), BTN_SIZE(6,1), GET_TEXT_F(ENDSTOPS))
     .font(font_tiny);
  #if PIN_EXISTS(X_MAX)
    PIN_ENABLED (GET_TEXT_F(X_MAX), X_MAX,X_MAX_ENDSTOP_INVERTING,1,2)
  #else
    PIN_DISABLED(GET_TEXT_F(X_MAX),X_MAX,X_MAX_ENDSTOP_INVERTING,1,2)
  #endif
  #if PIN_EXISTS(Y_MAX)
    PIN_ENABLED (GET_TEXT_F(Y_MAX),Y_MAX,Y_MAX_ENDSTOP_INVERTING,3,2)
  #else
    PIN_DISABLED(GET_TEXT_F(Y_MAX),Y_MAX,Y_MAX_ENDSTOP_INVERTING,3,2)
  #endif
  #if PIN_EXISTS(Z_MAX)
    PIN_ENABLED (GET_TEXT_F(Z_MAX),Z_MAX,Z_MAX_ENDSTOP_INVERTING,5,2)
  #else
    PIN_DISABLED(GET_TEXT_F(Z_MAX),Z_MAX,Z_MAX_ENDSTOP_INVERTING,5,2)
  #endif
  #if PIN_EXISTS(X_MIN)
    PIN_ENABLED (GET_TEXT_F(X_MIN),X_MIN,X_MIN_ENDSTOP_INVERTING,1,3)
  #else
    PIN_DISABLED(GET_TEXT_F(X_MIN),X_MIN,X_MIN_ENDSTOP_INVERTING,1,3)
  #endif
  #if PIN_EXISTS(Y_MIN)
    PIN_ENABLED (GET_TEXT_F(Y_MIN),Y_MIN,Y_MIN_ENDSTOP_INVERTING,3,3)
  #else
    PIN_DISABLED(GET_TEXT_F(Y_MIN),Y_MIN,Y_MIN_ENDSTOP_INVERTING,3,3)
  #endif
  #if PIN_EXISTS(Z_MIN)
    PIN_ENABLED (GET_TEXT_F(Z_MIN),Z_MIN,Z_MIN_ENDSTOP_INVERTING,5,3)
  #else
    PIN_DISABLED(GET_TEXT_F(Z_MIN),Z_MIN,Z_MIN_ENDSTOP_INVERTING,5,3)
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR) && PIN_EXISTS(FIL_RUNOUT)
    PIN_ENABLED (GET_TEXT_F(RUNOUT_1),FIL_RUNOUT, FIL_RUNOUT_INVERTING,1,4)
  #else
    PIN_DISABLED(GET_TEXT_F(RUNOUT_1),FIL_RUNOUT, FIL_RUNOUT_INVERTING,1,4)
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR) && PIN_EXISTS(FIL_RUNOUT2)
    PIN_ENABLED (GET_TEXT_F(RUNOUT_2),FIL_RUNOUT2,FIL_RUNOUT_INVERTING,3,4)
  #else
    PIN_DISABLED(GET_TEXT_F(RUNOUT_2),FIL_RUNOUT2,FIL_RUNOUT_INVERTING,3,4)
  #endif
  #if PIN_EXISTS(Z_MIN_PROBE)
    PIN_ENABLED (GET_TEXT_F(Z_PROBE),Z_MIN_PROBE,Z_MIN_PROBE_ENDSTOP_INVERTING,5,4)
  #else
    PIN_DISABLED(GET_TEXT_F(Z_PROBE),Z_MIN_PROBE,Z_MIN_PROBE_ENDSTOP_INVERTING,5,4)
  #endif

  #if HAS_SOFTWARE_ENDSTOPS
    #undef EDGE_R
    #define EDGE_R 30
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .font(font_small)
       .text         (BTN_POS(1,5), BTN_SIZE(3,1), GET_TEXT_F(SOFT_ENDSTOPS), OPT_RIGHTX | OPT_CENTERY)
       .colors(ui_toggle)
       .tag(2).toggle2(BTN_POS(4,5), BTN_SIZE(3,1), GET_TEXT_F(NO), GET_TEXT_F(YES), getSoftEndstopState());
      #undef EDGE_R
      #define EDGE_R 0
  #endif

  cmd.font(font_medium)
     .colors(action_btn)
     .tag(1).button( BTN_POS(1,7), BTN_SIZE(6,1), GET_TEXT_F(BACK));
  #undef GRID_COLS
  #undef GRID_ROWS
}

bool EndstopStatesScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    #if HAS_SOFTWARE_ENDSTOPS
    case 2: setSoftEndstopState(!getSoftEndstopState());
    #endif
    default:
      return false;
  }
  return true;
}

void EndstopStatesScreen::onIdle() {
  constexpr uint32_t DIAGNOSTICS_UPDATE_INTERVAL = 100;

  if (refresh_timer.elapsed(DIAGNOSTICS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
    reset_menu_timeout();
  }
  BaseScreen::onIdle();
}

#endif // LULZBOT_TOUCH_UI
