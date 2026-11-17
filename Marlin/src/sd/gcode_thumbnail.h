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
 * G-code thumbnail reader
 *
 * Finds an embedded image block in the header of the open SD file:
 *
 *   ; <tag> begin <W><x or *><H> <size> [other fields]
 *   ; <base64 text>
 *   ; <base64 text>
 *   ; <tag> end
 *
 * and decodes it as a stream, so a display driver can send it on in
 * pieces without holding the image in RAM. Examples of the header:
 *
 *   ; jpg begin 300*300 18420 0 100 25     (Creality Print, size = image bytes)
 *   ; thumbnail begin 200x200 24560         (Prusa-style, size = base64 chars)
 */

#include "../inc/MarlinConfig.h"

#if HAS_GCODE_THUMBNAIL

#include <stdint.h>

class GCodeThumbnail {
public:
  /**
   * Search the header of the file that is already open on 'card' for a
   * block with the given tag and dimensions. On success the stream is
   * positioned at the first image line, ready for read().
   *
   *   tag             : Text after "; " (e.g. "jpg" or "thumbnail")
   *   size_is_encoded : True if the size in the header counts base64
   *                     characters rather than decoded image bytes.
   */
  static bool find(PGM_P const tag, const uint16_t w, const uint16_t h, const bool size_is_encoded=false);

  // Decoded size in bytes of the image found by find()
  static uint32_t size() { return decoded_size; }

  // Decode up to 'len' bytes into 'buf'. Returns the number of bytes decoded
  // (less than 'len' at the end of the image or on a read error).
  static uint16_t read(uint8_t * const buf, const uint16_t len);

private:
  static uint32_t decoded_size, remaining;
  static uint8_t pending[3], pending_pos, pending_end;
};

#endif // HAS_GCODE_THUMBNAIL
