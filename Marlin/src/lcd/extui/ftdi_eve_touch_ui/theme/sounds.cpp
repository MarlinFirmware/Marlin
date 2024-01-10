/**************
 * sounds.cpp *
 **************/

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

#include "../ftdi_eve_lib/ftdi_eve_lib.h"

#include "sounds.h"

namespace Theme {
  using namespace FTDI;

  const PROGMEM SoundPlayer::sound_t chimes[] = {
    {CHIMES,       NOTE_G3,  5},
    {CHIMES,       NOTE_E4,  5},
    {CHIMES,       NOTE_C4,  5},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t sad_trombone[] = {
    {TRUMPET,      NOTE_A3S, 10},
    {TRUMPET,      NOTE_A3 , 10},
    {TRUMPET,      NOTE_G3S, 10},
    {TRUMPET,      NOTE_G3,  20},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t twinkle[] = {
    {GLOCKENSPIEL, NOTE_C4,  1},
    {GLOCKENSPIEL, NOTE_E4,  1},
    {GLOCKENSPIEL, NOTE_G4,  16},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t fanfare[] = {
    {TRUMPET,      NOTE_A3,  4},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_A3,  2},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_A3,  2},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_E4,  10},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t media_inserted[] = {
    {MUSIC_BOX,    NOTE_C4,  2},
    {MUSIC_BOX,    NOTE_E4,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t media_removed[] = {
    {MUSIC_BOX,    NOTE_E4,  2},
    {MUSIC_BOX,    NOTE_C4,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t js_bach_toccata[] = {
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  35},
    {SILENCE,      REST,     12},
    {ORGAN,        NOTE_G4,  4},
    {ORGAN,        NOTE_F4,  4},
    {ORGAN,        NOTE_E4,  4},
    {ORGAN,        NOTE_D4,  4},
    {ORGAN,        NOTE_C4S, 16},
    {ORGAN,        NOTE_D4,  32},
    {SILENCE,      REST,     42},

    {ORGAN,        NOTE_A3,  2},
    {ORGAN,        NOTE_G3,  2},
    {ORGAN,        NOTE_A3,  35},
    {SILENCE,      REST,     9},
    {ORGAN,        NOTE_E3,  8},
    {ORGAN,        NOTE_F3,  8},
    {ORGAN,        NOTE_C3S, 16},
    {ORGAN,        NOTE_D3,  27},
    {SILENCE,      REST,     42},

    {ORGAN,        NOTE_A2,  2},
    {ORGAN,        NOTE_G2,  2},
    {ORGAN,        NOTE_A2,  35},
    {SILENCE,      REST,     12},
    {ORGAN,        NOTE_G2,  4},
    {ORGAN,        NOTE_F2,  4},
    {ORGAN,        NOTE_E2,  4},
    {ORGAN,        NOTE_D2,  4},
    {ORGAN,        NOTE_C2S, 16},
    {ORGAN,        NOTE_D2,  32},
    {SILENCE,      REST,     52},

    //{ORGAN,        NOTE_D1,  28},
    {ORGAN,        NOTE_C3S, 9},
    {ORGAN,        NOTE_E3,  9},
    {ORGAN,        NOTE_G3,  9},
    {ORGAN,        NOTE_A3S, 9},
    {ORGAN,        NOTE_C4S, 9},
    {ORGAN,        NOTE_E4,  9},
    {ORGAN,        NOTE_D4,  20},
    {SILENCE,      REST,     30},

    {ORGAN,        NOTE_C4S, 4},
    {ORGAN,        NOTE_D4,  2},
    {ORGAN,        NOTE_E4,  2},

    {ORGAN,        NOTE_C4S, 2},
    {ORGAN,        NOTE_D4,  2},
    {ORGAN,        NOTE_E4,  2},

    {ORGAN,        NOTE_C4S, 2},
    {ORGAN,        NOTE_D4,  2},
    {ORGAN,        NOTE_E4,  2},

    {ORGAN,        NOTE_C4S, 2},
    {ORGAN,        NOTE_D4,  4},
    {ORGAN,        NOTE_E4,  4},
    {ORGAN,        NOTE_F4,  2},
    {ORGAN,        NOTE_G4,  2},

    {ORGAN,        NOTE_E4,  2},
    {ORGAN,        NOTE_F4,  2},
    {ORGAN,        NOTE_G4,  2},

    {ORGAN,        NOTE_E4,  2},
    {ORGAN,        NOTE_F4,  2},
    {ORGAN,        NOTE_G4,  2},

    {ORGAN,        NOTE_E4,  2},
    {ORGAN,        NOTE_F4,  4},
    {ORGAN,        NOTE_G4,  4},
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_A4S, 2},

    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_A4S, 2},

    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  2},
    {ORGAN,        NOTE_A4S, 2},

    {ORGAN,        NOTE_G4,  2},
    {ORGAN,        NOTE_A4,  4},
    {SILENCE,      REST,     36},


    {ORGAN,        NOTE_C5S, 4},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_C5S, 2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_C5S, 2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_C5S, 2},
    {ORGAN,        NOTE_D5,  4},
    {ORGAN,        NOTE_E5,  4},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  4},
    {ORGAN,        NOTE_G5,  4},
    {ORGAN,        NOTE_A5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5,  4},
    {SILENCE,      REST,     32},

    {ORGAN,        NOTE_A5,  4},
    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_G5,  2},
    {ORGAN,        NOTE_A5S, 2},

    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_A5,  2},

    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_F5,  2},
    {ORGAN,        NOTE_G5,  2},

    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_A5,  2},

    {ORGAN,        NOTE_C5,  2},
    {ORGAN,        NOTE_E5,  2},
    {ORGAN,        NOTE_A5,  2},

    {ORGAN,        NOTE_C5,  2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_F5,  2},

    {ORGAN,        NOTE_A4S, 2},
    {ORGAN,        NOTE_D5,  2},
    {ORGAN,        NOTE_E5,  2},

    {ORGAN,        NOTE_A4S, 2},
    {ORGAN,        NOTE_C5,  2},
    {ORGAN,        NOTE_E5,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t js_bach_joy[] = {
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_A3,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_D4,  3},
    {SILENCE,      REST,     1},

    {PIANO,        NOTE_C4,  3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_E4,  3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_D4,  2},
    {SILENCE,      REST,     2},

    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_G4 , 3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_F4S, 4},
    {PIANO,        NOTE_G4,  4},

    {PIANO,        NOTE_D4,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_B3,  3},
    {SILENCE,      REST,     1},
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_A3,  2},
    {SILENCE,      REST,     2},

    {PIANO,        NOTE_B3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_D4,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_E4,  2},
    {SILENCE,      REST,     2},

    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_C4,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_B3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_A3,  4},

    {PIANO,        NOTE_B3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_G3,  2},
    {SILENCE,      REST,     2},
    {PIANO,        NOTE_G3,  8},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t flagpole[] = {
    {TRIANGLE_WAVE, NOTE_G3,  2},
    {TRIANGLE_WAVE, NOTE_C4,  2},
    {TRIANGLE_WAVE, NOTE_E4,  2},
    {TRIANGLE_WAVE, NOTE_G4,  2},
    {TRIANGLE_WAVE, NOTE_C5,  2},
    {TRIANGLE_WAVE, NOTE_E5,  2},
    {TRIANGLE_WAVE, NOTE_G5,  5},
    {TRIANGLE_WAVE, NOTE_E5,  2},

    {TRIANGLE_WAVE, NOTE_G3S, 2},
    {TRIANGLE_WAVE, NOTE_C4,  2},
    {TRIANGLE_WAVE, NOTE_D4S, 2},
    {TRIANGLE_WAVE, NOTE_G4S, 2},
    {TRIANGLE_WAVE, NOTE_C5,  2},
    {TRIANGLE_WAVE, NOTE_D5S, 2},
    {TRIANGLE_WAVE, NOTE_G5S, 5},
    {TRIANGLE_WAVE, NOTE_D5S, 5},

    {TRIANGLE_WAVE, NOTE_A3S, 2},
    {TRIANGLE_WAVE, NOTE_D4,  2},
    {TRIANGLE_WAVE, NOTE_F4,  2},
    {TRIANGLE_WAVE, NOTE_A4S, 2},
    {TRIANGLE_WAVE, NOTE_D5,  2},
    {TRIANGLE_WAVE, NOTE_F5,  2},
    {TRIANGLE_WAVE, NOTE_A5S, 5},

    {SILENCE,       REST,     1},
    {TRIANGLE_WAVE, NOTE_A5S, 1},
    {SILENCE,       REST,     1},
    {TRIANGLE_WAVE, NOTE_A5S, 1},
    {SILENCE,       REST,     1},
    {TRIANGLE_WAVE, NOTE_A5S, 1},
    {SILENCE,       REST,     1},
    {TRIANGLE_WAVE, NOTE_A5S, 8},
    {SILENCE,       END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t big_band[] = {
    {XYLOPHONE,    NOTE_F4,  3},
    {XYLOPHONE,    NOTE_G4,  3},
    {XYLOPHONE,    NOTE_F4,  3},
    {XYLOPHONE,    NOTE_D4,  3},
    {XYLOPHONE,    NOTE_A3S, 3},
    {SILENCE, REST,     3},

    {TRUMPET,    NOTE_F4,  3},
    {TRUMPET,    NOTE_G4,  3},
    {TRUMPET,    NOTE_F4,  3},
    {TRUMPET,    NOTE_D4,  3},
    {TRUMPET,    NOTE_A3S, 3},
    {SILENCE, REST,     3},

    {TUBA,    NOTE_A2S, 6},
    {TUBA,    NOTE_A2S, 6},
    {TUBA,    NOTE_A2S, 4},
    {TUBA,    NOTE_A2S, 6},
    {TUBA,    NOTE_A2S, 6},
    {SILENCE, END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t beats[] = {
    {SILENCE,      REST,     8},
    {NOTCH,        NOTE_C4,  8},
    {KICKDRUM,     NOTE_C4,  8},
    {HIHAT,        NOTE_C4,  8},
    {COWBELL,      NOTE_C4,  8},
    {SILENCE,      REST,     8},
    {NOTCH,        NOTE_C4,  8},
    {KICKDRUM,     NOTE_C4,  8},
    {HIHAT,        NOTE_C4,  8},
    {COWBELL,      NOTE_C4,  8},
    {SILENCE,      REST,     8},
    {NOTCH,        NOTE_C4,  8},
    {KICKDRUM,     NOTE_C4,  8},
    {HIHAT,        NOTE_C4,  8},
    {COWBELL,      NOTE_C4,  8},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t beeping[] = {
    {BEEPING,      NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t alarm[] = {
    {ALARM,        NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t warble[] = {
    {WARBLE,       NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t carousel[] = {
    {CAROUSEL,     NOTE_C4,  64},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t all_instruments[] = {
    {HARP},
    {XYLOPHONE},
    {TUBA},
    {GLOCKENSPIEL},
    {ORGAN},
    {TRUMPET},
    {PIANO},
    {CHIMES},
    {MUSIC_BOX},
    {BELL},
    {CLICK},
    {SWITCH},
    {COWBELL},
    {NOTCH},
    {HIHAT},
    {KICKDRUM},
    {SWITCH},
    {POP},
    {CLACK},
    {CHACK},
    {SILENCE, END_SONG, 0}
  };
} // namespace Theme

#define N_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

const SoundList::list_t SoundList::list[] = {
  {"Silence",       FTDI::silence},
  {"Twinkle",      Theme::twinkle},
  {"Chimes",       Theme::chimes},
  {"Fanfare",      Theme::fanfare},
  {"Sad Trombone", Theme::sad_trombone},
  {"Big Band",     Theme::big_band},
  {"Beeping",      Theme::beeping},
  {"Alarm",        Theme::alarm},
  {"Warble",       Theme::warble},
  {"Carousel",     Theme::carousel},
  {"Beats",        Theme::beats},
  {"Bach Joy",     Theme::js_bach_joy},
  {"Bach Toccata", Theme::js_bach_toccata},
  {"Flagpole",     Theme::flagpole}
};

const uint8_t SoundList::n = N_ELEMENTS(SoundList::list);

#endif // TOUCH_UI_FTDI_EVE
