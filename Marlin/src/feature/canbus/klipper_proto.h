/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * feature/canbus/klipper_proto.h - Klipper MCU protocol encodings (docs/Protocol.md)
 *
 * Message blocks are <len><seq><payload...><crc16 hi><crc16 lo><0x7E>. Integers in the payload
 * are variable length quantities. These follow Klipper's src/command.c (GPLv3, Kevin O'Connor).
 * No hardware dependencies, so they can be unit tested natively.
 */

#include <stdint.h>
#include <string.h>

namespace KlipperProto {

  constexpr uint8_t MESSAGE_MIN = 5, MESSAGE_MAX = 64,
                    MESSAGE_HEADER_SIZE = 2, MESSAGE_TRAILER_SIZE = 3,
                    MESSAGE_SEQ_MASK = 0x0F, MESSAGE_DEST = 0x10, MESSAGE_SYNC = 0x7E;

  // CRC-16/MCRF4XX (poly 0x8408 reflected, init 0xFFFF), as Klipper computes it
  inline uint16_t crc16_ccitt(const uint8_t *buf, uint8_t len) {
    uint16_t crc = 0xFFFF;
    while (len--) {
      uint8_t data = *buf++;
      data ^= crc & 0xFF;
      data ^= data << 4;
      crc = ((uint16_t(data) << 8) | (crc >> 8)) ^ uint8_t(data >> 4) ^ (uint16_t(data) << 3);
    }
    return crc;
  }

  // Append a variable length quantity. Return the new end of the buffer.
  inline uint8_t* encode_vlq(uint8_t *p, const uint32_t v) {
    const int32_t sv = v;
    if (!(sv < (3L << 5) && sv >= -(1L << 5))) {
      if (!(sv < (3L << 12) && sv >= -(1L << 12))) {
        if (!(sv < (3L << 19) && sv >= -(1L << 19))) {
          if (!(sv < (3L << 26) && sv >= -(1L << 26))) *p++ = ((sv >> 28) & 0x7F) | 0x80; // Sign-extended, as klippy encodes it
          *p++ = ((v >> 21) & 0x7F) | 0x80;
        }
        *p++ = ((v >> 14) & 0x7F) | 0x80;
      }
      *p++ = ((v >> 7) & 0x7F) | 0x80;
    }
    *p++ = v & 0x7F;
    return p;
  }

  // Read a variable length quantity and advance the pointer past it
  inline uint32_t decode_vlq(const uint8_t *&p) {
    uint8_t c = *p++;
    uint32_t v = c & 0x7F;
    if ((c & 0x60) == 0x60) v |= -0x20;
    while (c & 0x80) { c = *p++; v = (v << 7) | (c & 0x7F); }
    return v;
  }

  // Wrap a payload in a message block. Return the block length, or 0 if the payload is too long.
  inline uint8_t make_block(uint8_t (&block)[MESSAGE_MAX], const uint8_t seq, const uint8_t *payload, const uint8_t len) {
    if (len > MESSAGE_MAX - MESSAGE_MIN) return 0;
    const uint8_t blen = len + MESSAGE_MIN;
    block[0] = blen;
    block[1] = MESSAGE_DEST | (seq & MESSAGE_SEQ_MASK);
    memcpy(&block[MESSAGE_HEADER_SIZE], payload, len);
    const uint16_t crc = crc16_ccitt(block, blen - MESSAGE_TRAILER_SIZE);
    block[blen - 3] = crc >> 8;
    block[blen - 2] = crc & 0xFF;
    block[blen - 1] = MESSAGE_SYNC;
    return blen;
  }

  // True if 'block' starts with a complete, valid message block
  inline bool block_valid(const uint8_t *block, const uint8_t have) {
    if (have < MESSAGE_MIN) return false;
    const uint8_t blen = block[0];
    if (blen < MESSAGE_MIN || blen > MESSAGE_MAX || have < blen) return false;
    if ((block[1] & ~MESSAGE_SEQ_MASK) != MESSAGE_DEST || block[blen - 1] != MESSAGE_SYNC) return false;
    const uint16_t crc = crc16_ccitt(block, blen - MESSAGE_TRAILER_SIZE);
    return block[blen - 3] == (crc >> 8) && block[blen - 2] == (crc & 0xFF);
  }

} // KlipperProto
