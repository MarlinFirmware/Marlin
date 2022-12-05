/*****************************
 * interface_sounds_screen.h *
 *****************************/

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

#pragma once

#define FTDI_INTERFACE_SOUNDS_SCREEN
#define FTDI_INTERFACE_SOUNDS_SCREEN_CLASS InterfaceSoundsScreen

class InterfaceSoundsScreen : public BaseScreen, public CachedScreen<INTERFACE_SOUNDS_SCREEN_CACHE> {
  public:
    enum event_t {
      PRINTING_STARTED  = 0,
      PRINTING_FINISHED = 1,
      PRINTING_FAILED   = 2,

      NUM_EVENTS
    };

  private:
    friend class InterfaceSettingsScreen;

    static uint8_t event_sounds[NUM_EVENTS];

    static const char* getSoundSelection(event_t);
    static void toggleSoundSelection(event_t);
    static void setSoundSelection(event_t, const FTDI::SoundPlayer::sound_t*);

  public:
    static void playEventSound(event_t, FTDI::play_mode_t = FTDI::PLAY_ASYNCHRONOUS);

    static void defaultSettings();

    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};
