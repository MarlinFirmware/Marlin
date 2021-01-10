/********************
 * sound_player.cpp *
 ********************/

/****************************************************************************
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

#include "ftdi_extended.h"

#if ENABLED(FTDI_EXTENDED)

namespace FTDI {
  SoundPlayer sound; // Global sound player object

  void SoundPlayer::set_volume(uint8_t vol) {
    CLCD::mem_write_8(REG::VOL_SOUND, vol);
  }

  uint8_t SoundPlayer::get_volume() {
    return CLCD::mem_read_8(REG::VOL_SOUND);
  }

  void SoundPlayer::play(effect_t effect, note_t note) {

    #if ENABLED(TOUCH_UI_DEBUG)
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR  ("Playing note ", int(note));
      SERIAL_ECHOLNPAIR(", instrument ", int(effect));
    #endif

    // Play the note
    CLCD::mem_write_16(REG::SOUND, (note == REST) ? 0 : (((note ? note : NOTE_C4) << 8) | effect));
    CLCD::mem_write_8(REG::PLAY, 1);
  }

  note_t SoundPlayer::frequency_to_midi_note(const uint16_t frequency_hz) {
    const float f0 = 440;
    return note_t(NOTE_A4 + (log(frequency_hz)-log(f0))*12/log(2) + 0.5);
  }

  // Plays a tone of a given frequency and duration. Since the FTDI FT810 only
  // supports MIDI notes, we round down to the nearest note.

  void SoundPlayer::play_tone(const uint16_t frequency_hz, const uint16_t duration_ms) {
    play(ORGAN, frequency_to_midi_note(frequency_hz));

    // Schedule silence to squelch the note after the duration expires.
    sequence = silence;
    wait = duration_ms;
    timer.start();
  }

  void SoundPlayer::play(const sound_t* seq, play_mode_t mode) {
    sequence = seq;
    wait     = 250; // Adding this delay causes the note to not be clipped, not sure why.
    timer.start();

    if (mode == PLAY_ASYNCHRONOUS) return;

    // If playing synchronously, then play all the notes here

    while (has_more_notes()) {
      onIdle();
      TERN_(TOUCH_UI_FTDI_EVE, ExtUI::yield());
    }
  }

  bool SoundPlayer::is_sound_playing() {
    return CLCD::mem_read_8( REG::PLAY ) & 0x1;
  }

  void SoundPlayer::onIdle() {
    if (!sequence) return;

    const bool ready_for_next_note = (wait == 0) ? !is_sound_playing() : timer.elapsed(wait);

    if (ready_for_next_note) {
      const effect_t fx = effect_t(pgm_read_byte(&sequence->effect));
      const note_t   nt =   note_t(pgm_read_byte(&sequence->note));
      const uint32_t ms = uint32_t(pgm_read_byte(&sequence->sixteenths)) * 1000 / 16;

      if (ms == 0 && fx == SILENCE && nt == END_SONG) {
        sequence = 0;
        play(SILENCE, REST);
      }
      else {
        wait = ms;
        timer.start();
        play(fx, nt);
        sequence++;
      }
    }
  }
} // namespace FTDI

#endif // FTDI_EXTENDED
