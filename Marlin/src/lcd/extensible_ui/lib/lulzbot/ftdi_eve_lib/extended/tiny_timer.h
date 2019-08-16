/****************
 * tiny_timer.h *
 ****************/

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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

/* Helpful Reference:
 *
 *  https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover
 */

/* tiny_interval_t downsamples a 32-bit millis() value
   into a 8-bit value which can record periods of
   a few seconds with a rougly 1/16th of second
   resolution. This allows us to measure small
   intervals without needing to use four-byte counters.
 */
class tiny_time_t {
  private:
    friend class tiny_timer_t;
    uint8_t _duration;

    static uint8_t tiny_time(uint32_t ms) {return ceil(float(ms) / 64);};

  public:
    tiny_time_t()            : _duration(0) {}
    tiny_time_t(uint32_t ms) : _duration(tiny_time(ms)) {}
    tiny_time_t & operator=   (uint32_t ms) {_duration = tiny_time(ms); return *this;}
    bool          operator == (uint32_t ms) {return _duration == tiny_time(ms);}
};

class tiny_timer_t {
  private:
    uint8_t _start;

  public:
    void start();
    bool elapsed(tiny_time_t interval);
};
