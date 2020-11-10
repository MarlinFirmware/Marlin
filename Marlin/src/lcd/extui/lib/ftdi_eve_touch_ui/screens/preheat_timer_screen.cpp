/****************************
 * preheat_timer_screen.cpp *
 ****************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE) && defined(TOUCH_UI_COCOA_PRESS)

#include "screens.h"
#include "screen_data.h"

#include "../ftdi_eve_lib/extras/circular_progress.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define GRID_COLS 2
#define GRID_ROWS 5

void PreheatTimerScreen::draw_message(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0);
    draw_text_box(cmd, BTN_POS(2,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_HEATING), OPT_CENTER, font_large);
  }
}

uint16_t PreheatTimerScreen::secondsRemaining() {
  const uint32_t elapsed_sec = (millis() - screen_data.PreheatTimerScreen.start_ms) / 1000;
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
    draw_circular_progress(cmd, BTN_POS(1,1), BTN_SIZE(1,5), float(secondsRemaining()) * 100 / COCOA_PRESS_PREHEAT_SECONDS, str, theme_dark, theme_darkest);
  }
}

void PreheatTimerScreen::draw_interaction_buttons(draw_mode_t what) {
  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(1).button( BTN_POS(2,5), BTN_SIZE(1,1), F("Cancel"));
  }
}

void PreheatTimerScreen::onEntry() {
  screen_data.PreheatTimerScreen.start_ms = millis();
}

void PreheatTimerScreen::onRedraw(draw_mode_t what) {
  draw_message(what);
  draw_time_remaining(what);
  draw_interaction_buttons(what);
}

bool PreheatTimerScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); return true;
    default: break;
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

#endif // TOUCH_UI_FTDI_EVE
