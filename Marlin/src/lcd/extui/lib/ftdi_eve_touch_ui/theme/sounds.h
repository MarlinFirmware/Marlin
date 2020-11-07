/************
 * sounds.h *
 ************/

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

namespace Theme {
  using namespace FTDI;

  extern const PROGMEM SoundPlayer::sound_t chimes[];
  extern const PROGMEM SoundPlayer::sound_t sad_trombone[];
  extern const PROGMEM SoundPlayer::sound_t twinkle[];
  extern const PROGMEM SoundPlayer::sound_t fanfare[];
  extern const PROGMEM SoundPlayer::sound_t media_inserted[];
  extern const PROGMEM SoundPlayer::sound_t media_removed[];
  extern const PROGMEM SoundPlayer::sound_t js_bach_toccata[];
  extern const PROGMEM SoundPlayer::sound_t js_bach_joy[];
  extern const PROGMEM SoundPlayer::sound_t big_band[];
  extern const PROGMEM SoundPlayer::sound_t beats[];
  extern const PROGMEM SoundPlayer::sound_t beeping[];
  extern const PROGMEM SoundPlayer::sound_t alarm[];
  extern const PROGMEM SoundPlayer::sound_t warble[];
  extern const PROGMEM SoundPlayer::sound_t carousel[];
  extern const PROGMEM SoundPlayer::sound_t all_instruments[];
}; // namespace Theme
