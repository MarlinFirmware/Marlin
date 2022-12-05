/***************************************
 * cocoapress/preheat_timer_screen.cpp *
 ***************************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Cocoa Press                          *
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
#include "../screen_data.h"

#ifdef COCOA_PREHEAT_SCREEN

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

constexpr static PreheatTimerScreenData &mydata = screen_data.PreheatTimerScreen;

#define GRID_COLS 2
#define GRID_ROWS 8

#define HEADER_POS      BTN_POS(2,1), BTN_SIZE(1,2)
#define NOZZLE_ADJ_POS  BTN_POS(2,3), BTN_SIZE(1,2)
#define BODY_ADJ_POS    BTN_POS(2,5), BTN_SIZE(1,2)
#define CHAMBER_ADJ_POS BTN_POS(2,7), BTN_SIZE(1,2)
#define PROGRESS_POS    BTN_POS(1,1), BTN_SIZE(1,7)
#define BACK_POS        BTN_POS(1,8), BTN_SIZE(1,1)

void PreheatTimerScreen::draw_message(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0);
    draw_text_box(cmd, HEADER_POS, GET_TEXT_F(MSG_HEATING), OPT_CENTER, font_large);
  }
}

uint16_t PreheatTimerScreen::secondsRemaining() {
  const uint32_t elapsed_sec = (millis() - mydata.start_ms) / 1000;
  return (COCOA_PRESS_PREHEAT_SECONDS > elapsed_sec) ? COCOA_PRESS_PREHEAT_SECONDS - elapsed_sec : 0;
}

void PreheatTimerScreen::draw_time_remaining(draw_mode_t what) {
  if (what & FOREGROUND) {
    const uint16_t elapsed_sec = secondsRemaining();
    const uint8_t min = elapsed_sec / 60,
                  sec = elapsed_sec % 60;

    char str[10];
    sprintf_P(str, PSTR("%02d:%02d"), min, sec);

    CommandProcessor cmd;
    cmd.font(font_xlarge);
    draw_circular_progress(cmd, PROGRESS_POS, float(secondsRemaining()) * 100 / COCOA_PRESS_PREHEAT_SECONDS, str, theme_dark, theme_darkest);
  }
}

void PreheatTimerScreen::draw_interaction_buttons(draw_mode_t what) {
  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(1).button(BACK_POS, GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

void PreheatTimerScreen::draw_adjuster(draw_mode_t what, uint8_t tag, FSTR_P label, float value, int16_t x, int16_t y, int16_t w, int16_t h) {
    #define SUB_COLS          9
    #define SUB_ROWS          2

    CommandProcessor cmd;
    cmd.tag(0)
       .font(font_small);
    if (what & BACKGROUND) {
        cmd.text(  SUB_POS(1,1), SUB_SIZE(9,1), label)
           .button(SUB_POS(1,2), SUB_SIZE(5,1), F(""), OPT_FLAT);
    }

    if (what & FOREGROUND) {
        char str[32];
        dtostrf(value, 5, 1, str);
        strcat_P(str, PSTR(" "));
        strcat_P(str, (const char*) GET_TEXT_F(MSG_UNITS_C));

        cmd.text(SUB_POS(1,2), SUB_SIZE(5,1), str)
           .font(font_medium)
           .tag(tag  ).button(SUB_POS(6,2), SUB_SIZE(2,1), F("-"))
           .tag(tag+1).button(SUB_POS(8,2), SUB_SIZE(2,1), F("+"));
    }
}

void PreheatTimerScreen::onEntry() {
  mydata.start_ms = millis();
}

void PreheatTimerScreen::onRedraw(draw_mode_t what) {
  draw_message(what);
  draw_time_remaining(what);
  draw_interaction_buttons(what);
  draw_adjuster(what, 2, GET_TEXT_F(MSG_NOZZLE),  getTargetTemp_celsius(E0),      NOZZLE_ADJ_POS);
  draw_adjuster(what, 4, GET_TEXT_F(MSG_BODY),    getTargetTemp_celsius(E1),      BODY_ADJ_POS);
  draw_adjuster(what, 6, GET_TEXT_F(MSG_CHAMBER), getTargetTemp_celsius(CHAMBER), CHAMBER_ADJ_POS);
}

bool PreheatTimerScreen::onTouchHeld(uint8_t tag) {
  const float increment = (tag == 6 || tag == 7) ? 1 : 0.1;
  switch (tag) {
    case 2: UI_DECREMENT(TargetTemp_celsius, E0); break;
    case 3: UI_INCREMENT(TargetTemp_celsius, E0); break;
    case 4: UI_DECREMENT(TargetTemp_celsius, E1); break;
    case 5: UI_INCREMENT(TargetTemp_celsius, E1); break;
    case 6: UI_DECREMENT(TargetTemp_celsius, CHAMBER); break;
    case 7: UI_INCREMENT(TargetTemp_celsius, CHAMBER); break;
    default:
      return false;
  }
  return true;
}

bool PreheatTimerScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); return true;
    default: return current_screen.onTouchHeld(tag);
  }
  return false;
}

void PreheatTimerScreen::onIdle() {
  if (secondsRemaining() == 0) {
    AlertDialogBox::show(GET_TEXT_F(MSG_PREHEAT_FINISHED));
    // Remove SaveSettingsDialogBox from the stack
    // so the alert box doesn't return to me.
    current_screen.forget();
  }

  reset_menu_timeout();
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

#endif // COCOA_PREHEAT_SCREEN
