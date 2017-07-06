/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MarlinConfig.h"

#if ENABLED(CHUNK_SUPPORT)

#include "serial.h"

//wavetables, 3 bit move, 1 bit direction
//    +/- 7 pulses over 8 steps
uint8_t segment_moves[16][8] = {
  { 1, 1, 1, 1, 1, 1, 1, 0 }, //  0 = -7
  { 1, 1, 1, 0, 1, 1, 1, 0 }, //  1 = -6
  { 0, 1, 1, 0, 1, 0, 1, 1 }, //  2 = -5
  { 0, 1, 0, 1, 0, 1, 0, 1 }, //  3 = -4
  { 0, 1, 0, 0, 1, 0, 0, 1 }, //  4 = -3
  { 0, 0, 1, 0, 0, 0, 1, 0 }, //  5 = -2
  { 0, 0, 0, 0, 1, 0, 0, 0 }, //  6 = -1

  { 0, 0, 0, 0, 0, 0, 0, 0 }, //  7 =  0

  { 0, 0, 0, 0, 1, 0, 0, 0 }, //  8 =  1
  { 0, 0, 1, 0, 0, 0, 1, 0 }, //  9 =  2
  { 0, 1, 0, 0, 1, 0, 0, 1 }, // 10 =  3
  { 0, 1, 0, 1, 0, 1, 0, 1 }, // 11 =  4
  { 0, 1, 1, 0, 1, 0, 1, 1 }, // 12 =  5
  { 1, 1, 1, 0, 1, 1, 1, 0 }, // 13 =  6
  { 1, 1, 1, 1, 1, 1, 1, 0 }, // 14 =  7

  { 0 }
};

FORCE_INLINE bool send_one_ok() {
  static uint8_t response_index = 0;

  switch (chunk_response[response_index]) {
    case CHUNK_RESPONSE_NONE:
    case CHUNK_RESPONSE_PENDING:
      return false;

    case CHUNK_RESPONSE_OK:
      chunk_response[response_index] = CHUNK_RESPONSE_PENDING;
      SERIAL_PROTOCOLLNPAIR("!ok ", response_index);
      break;

    case CHUNK_RESPONSE_FAIL:
      chunk_response[response_index] = CHUNK_RESPONSE_NONE;
      SERIAL_PROTOCOLLNPAIR("!fail ", response_index);
      break;
  }

  //keep iterating
  response_index = (response_index + 1) & (NUM_CHUNK_BUFFERS - 1);

  return true;
}

void send_chunk_ok() {
  static uint8_t chunk_respond_busy_done = 0;

  //send oks first
  while(send_one_ok());

  //concurrently keep up with response counter, should auto roll over
  while (chunk_respond_busy_done != chunk_respond_busy) {
    SERIAL_PROTOCOLLNPGM("!busy");
    chunk_respond_busy_done++;
  }
}

#endif // CHUNK_SUPPORT
