/******************
 * tiny_timer.cpp *
 ******************/

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

bool tiny_timer_t::elapsed(tiny_time_t duration) {
  uint8_t now = tiny_time_t::tiny_time(
    TERN(__MARLIN_FIRMWARE__, ExtUI::safe_millis(), millis())
  );
  uint8_t elapsed = now - _start;
  return elapsed >= duration._duration;
}

void tiny_timer_t::start() {
  _start = tiny_time_t::tiny_time(
    TERN(__MARLIN_FIRMWARE__, ExtUI::safe_millis(), millis())
  );
}
#endif // FTDI_EXTENDED
