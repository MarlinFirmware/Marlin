/*******************************
 * interface_sounds_screen.cpp *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

uint8_t InterfaceSoundsScreen::event_sounds[];

const char* InterfaceSoundsScreen::getSoundSelection(event_t event) {
  return SoundList::name(event_sounds[event]);
}

void InterfaceSoundsScreen::toggleSoundSelection(event_t event) {
  event_sounds[event] = (event_sounds[event]+1) % SoundList::n;
  playEventSound(event);
}

void InterfaceSoundsScreen::setSoundSelection(event_t event, const SoundPlayer::sound_t* sound) {
  for (uint8_t i = 0; i < SoundList::n; i++)
    if (SoundList::data(i) == sound)
      event_sounds[event] = i;
}

void InterfaceSoundsScreen::playEventSound(event_t event, play_mode_t mode) {
  sound.play(SoundList::data(event_sounds[event]), mode);
}

void InterfaceSoundsScreen::defaultSettings() {
  setSoundSelection(PRINTING_STARTED,  twinkle);
  setSoundSelection(PRINTING_FINISHED, fanfare);
  setSoundSelection(PRINTING_FAILED,   sad_trombone);
}

void InterfaceSoundsScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)

    #define GRID_COLS 4
    #define GRID_ROWS 9

       .font(font_medium)
       .text(BTN_POS(1,1), BTN_SIZE(4,1), GET_TEXT_F(MSG_SOUNDS))
    #undef EDGE_R
    #define EDGE_R 30
       .font(font_small)
       .tag(0).text      (BTN_POS(1,2), BTN_SIZE(2,1), GET_TEXT_F(MSG_SOUND_VOLUME),   OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,3), BTN_SIZE(2,1), GET_TEXT_F(MSG_CLICK_SOUNDS),   OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,5), BTN_SIZE(2,1), GET_TEXT_F(MSG_PRINT_STARTING), OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,6), BTN_SIZE(2,1), GET_TEXT_F(MSG_PRINT_FINISHED), OPT_RIGHTX | OPT_CENTERY)
              .text      (BTN_POS(1,7), BTN_SIZE(2,1), GET_TEXT_F(MSG_PRINT_ERROR),    OPT_RIGHTX | OPT_CENTERY);
    #undef EDGE_R
  }

  if (what & FOREGROUND) {
    #ifdef TOUCH_UI_PORTRAIT
      constexpr uint8_t w = 2;
    #else
      constexpr uint8_t w = 1;
    #endif

    cmd.font(font_medium)
       .colors(ui_slider)
    #define EDGE_R 30
       .tag(2).slider    (BTN_POS(3,2), BTN_SIZE(2,1), screen_data.InterfaceSettingsScreen.volume, 0xFF)
       .colors(ui_toggle)
       .tag(3).toggle2   (BTN_POS(3,3), BTN_SIZE(w,1), GET_TEXT_F(MSG_NO), GET_TEXT_F(MSG_YES), UIData::touch_sounds_enabled())
    #undef EDGE_R
       .colors(normal_btn)
    #define EDGE_R 0
       .tag(4).button    (BTN_POS(3,5), BTN_SIZE(2,1), getSoundSelection(PRINTING_STARTED))
       .tag(5).button    (BTN_POS(3,6), BTN_SIZE(2,1), getSoundSelection(PRINTING_FINISHED))
       .tag(6).button    (BTN_POS(3,7), BTN_SIZE(2,1), getSoundSelection(PRINTING_FAILED))
       .colors(action_btn)
       .tag(1).button    (BTN_POS(1,9), BTN_SIZE(4,1), GET_TEXT_F(MSG_BACK));
  }
}

void InterfaceSoundsScreen::onEntry() {
  screen_data.InterfaceSettingsScreen.volume = SoundPlayer::get_volume();
  BaseScreen::onEntry();
}

bool InterfaceSoundsScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS();                                              return true;
    case 3: UIData::enable_touch_sounds(!UIData::touch_sounds_enabled()); break;
    case 4: toggleSoundSelection(PRINTING_STARTED);                       break;
    case 5: toggleSoundSelection(PRINTING_FINISHED);                      break;
    case 6: toggleSoundSelection(PRINTING_FAILED);                        break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

bool InterfaceSoundsScreen::onTouchStart(uint8_t tag) {
  CommandProcessor cmd;
  #undef EDGE_R
  #define EDGE_R 30
  switch (tag) {
    case 2: cmd.track_linear(BTN_POS(3,2), BTN_SIZE(2,1), 2).execute(); break;
    default: break;
  }
  return true;
}

void InterfaceSoundsScreen::onIdle() {
  if (refresh_timer.elapsed(TOUCH_UPDATE_INTERVAL)) {
    refresh_timer.start();

    uint16_t value;
    CommandProcessor cmd;
    switch (cmd.track_tag(value)) {
      case 2:
        screen_data.InterfaceSettingsScreen.volume = value >> 8;
        SoundPlayer::set_volume(screen_data.InterfaceSettingsScreen.volume);
        SaveSettingsDialogBox::settingsChanged();
        break;
      default:
        return;
    }
    onRefresh();
  }
  BaseScreen::onIdle();
}

#endif // TOUCH_UI_FTDI_EVE
