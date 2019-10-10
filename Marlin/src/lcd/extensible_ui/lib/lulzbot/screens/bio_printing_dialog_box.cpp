/*******************************
 * bio_printing_dialog_box.cpp *
 *******************************/

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

#if ENABLED(LULZBOT_TOUCH_UI) && defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

#include "../ftdi_eve_lib/extras/circular_progress.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define GRID_COLS 2
#define GRID_ROWS 9

void BioPrintingDialogBox::draw_status_message(draw_mode_t what, const char* message) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .tag(0);
    draw_text_box(cmd, BTN_POS(1,2), BTN_SIZE(2,2), message, OPT_CENTER, font_large);
  }
}

void BioPrintingDialogBox::draw_progress(draw_mode_t what) {
  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_large)
       .text(BTN_POS(1,1), BTN_SIZE(2,2), isPrinting() ? F("Printing...") : F("Finished."))
       .tag(1)
       .font(font_xlarge);

    draw_circular_progress(cmd, BTN_POS(1,4), BTN_SIZE(2,3), getProgress_percent(), theme_dark, theme_darkest);
  }
}

void BioPrintingDialogBox::draw_time_remaining(draw_mode_t what) {
  if (what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    const uint8_t hrs = elapsed/3600;
    const uint8_t min = (elapsed/60)%60;

    char time_str[10];
    sprintf_P(time_str, PSTR("%02dh %02dm"), hrs, min);

    CommandProcessor cmd;
    cmd.font(font_large)
       .text(BTN_POS(1,7), BTN_SIZE(2,2), time_str);
  }
}

void BioPrintingDialogBox::draw_interaction_buttons(draw_mode_t what) {
  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(font_medium)
       .colors(isPrinting() ? action_btn : normal_btn)
       .tag(2).button(BTN_POS(1,9), BTN_SIZE(1,1), F("Menu"))
        #if ENABLED(SDSUPPORT)
          .enabled(isPrinting() ? isPrintingFromMedia() : 1)
        #else
          .enabled(isPrinting() ? 0 : 1)
        #endif
       .tag(3)
       .colors(isPrinting() ? normal_btn : action_btn)
       .button( BTN_POS(2,9), BTN_SIZE(1,1), isPrinting() ? F("Cancel") : F("Back"));
  }
}

void BioPrintingDialogBox::onRedraw(draw_mode_t what) {
  if (what & FOREGROUND) {
    draw_progress(FOREGROUND);
    draw_time_remaining(FOREGROUND);
    draw_interaction_buttons(FOREGROUND);
  }
}

bool BioPrintingDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_SCREEN(FeedratePercentScreen); break;
    case 2: GOTO_SCREEN(TuneMenu); break;
    case 3:
     if (isPrinting()) {
       GOTO_SCREEN(ConfirmAbortPrintDialogBox);
     } else {
       GOTO_SCREEN(StatusScreen);
     }
     break;
    default: return false;
  }
  return true;
}

void BioPrintingDialogBox::setStatusMessage(progmem_str message) {
  char buff[strlen_P((const char*)message)+1];
  strcpy_P(buff, (const char*) message);
  setStatusMessage(buff);
}

void BioPrintingDialogBox::setStatusMessage(const char* message) {
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(CLEAR(true,true,true));

  draw_status_message(BACKGROUND, message);
  draw_progress(BACKGROUND);
  draw_time_remaining(BACKGROUND);
  draw_interaction_buttons(BACKGROUND);
  storeBackground();

  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR("New status message: ", message);
  #endif

  if (AT_SCREEN(BioPrintingDialogBox)) {
    current_screen.onRefresh();
  }
}

void BioPrintingDialogBox::onIdle() {
  reset_menu_timeout();
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

void BioPrintingDialogBox::show() {
  GOTO_SCREEN(BioPrintingDialogBox);
}

#endif // LULZBOT_TOUCH_UI
