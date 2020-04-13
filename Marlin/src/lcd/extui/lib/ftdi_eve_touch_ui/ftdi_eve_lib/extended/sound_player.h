/******************
 * sound_player.h *
 ******************/

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

#pragma once

namespace FTDI {
  typedef enum {
    PLAY_ASYNCHRONOUS,
    PLAY_SYNCHRONOUS
  } play_mode_t;

  class SoundPlayer {
    typedef FTDI::ftdi_registers  REG;
    typedef FTDI::ftdi_memory_map MAP;

    public:
      struct sound_t {
        effect_t  effect;      // The sound effect number
        note_t    note;        // The MIDI note value
        uint16_t  sixteenths;  // Duration of note, in sixteeths of a second, or zero to play to completion
      };

      const uint8_t WAIT = 0;

    private:
      const sound_t   *sequence;
      tiny_timer_t     timer;
      tiny_time_t      wait;

      note_t frequency_to_midi_note(const uint16_t frequency);

    public:
      static void set_volume(uint8_t volume);
      static uint8_t get_volume();

      static void play(effect_t effect, note_t note = NOTE_C4);
      static bool is_sound_playing();

      void play(const sound_t* seq, play_mode_t mode = PLAY_SYNCHRONOUS);
      void play_tone(const uint16_t frequency_hz, const uint16_t duration_ms);
      bool has_more_notes() {return sequence != 0;};

      void onIdle();
  };

  extern SoundPlayer sound;

  const PROGMEM SoundPlayer::sound_t silence[] = {
    {SILENCE,      END_SONG, 0}
  };
}
