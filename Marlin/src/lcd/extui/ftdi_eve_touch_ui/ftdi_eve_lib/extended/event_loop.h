/****************
 * event_loop.h *
 ****************/

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

#define STATUS_UPDATE_INTERVAL     1000
#define TOUCH_UPDATE_INTERVAL        50
#define TOUCH_REPEATS_PER_SECOND      4
#define DEBOUNCE_PERIOD             150

class UIData {
  private:
    typedef union {
      struct {
        uint8_t touch_start_sound  : 1;
        uint8_t touch_end_sound    : 1;
        uint8_t touch_repeat_sound : 1;
        uint8_t show_animations    : 1;
        uint8_t touch_debouncing   : 1;
        uint8_t ignore_unpress     : 1;
        uint8_t prevent_reentry    : 1;
      } bits;
      uint8_t value;
    } flags_t;

  public:
    static flags_t flags;

    static uint8_t get_persistent_data_mask();
    static uint8_t get_persistent_data();
    static void set_persistent_data(uint8_t value);
    static void reset_persistent_data();

    static void enable_touch_sounds(bool enabled);
    static bool touch_sounds_enabled();
    static void enable_animations(bool enabled);
    static bool animations_enabled();
};

namespace FTDI {
  class EventLoop {
    private:
      static constexpr FTDI::effect_t press_sound   = FTDI::CHACK;
      static constexpr FTDI::effect_t repeat_sound  = FTDI::CHACK;
      static constexpr FTDI::effect_t unpress_sound = FTDI::POP;
      static void process_events();

    public:
      static void setup();
      static void loop();

      static uint8_t get_pressed_tag();
      static bool is_touch_held();
  };
}
