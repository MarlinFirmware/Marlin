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

/**
 * Decoder for the MKS TFT preview images written by the MKS and Elegoo slicer plugins.
 * Each line holds one image row of RGB565 pixels as hex, low byte first, after a prefix:
 *   ;simage:<hex>    100 x 100 image
 *   ;;gimage:<hex>   200 x 200 image
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(GCODE_PREVIEW_MKS)

#include "mks_decoder.h"

namespace ImageDecoders {

  static int8_t hex_value(const int16_t c) {
    if (WITHIN(c, '0', '9')) return c - '0';
    if (WITHIN(c, 'A', 'F')) return c - 'A' + 10;
    if (WITHIN(c, 'a', 'f')) return c - 'a' + 10;
    return -1;
  }

  // Read a byte written as two hex digits
  static int16_t read_hex_byte(read_byte_t read) {
    const int8_t hi = hex_value(read()), lo = hex_value(read());
    return (hi < 0 || lo < 0) ? -1 : (hi << 4 | lo);
  }

  bool MKSDecoder::draw(read_byte_t read, draw_pixels_t draw_pixels, const uint16_t size, const uint16_t max_w, const uint16_t max_h, const uint16_t background, uint16_t * const row_buffer) {
    if (size > max_w || size > max_h) return false;
    const uint16_t x = (max_w - size) / 2, y = (max_h - size) / 2;  // Center the image

    for (uint16_t row = 0; row < size; ++row) {
      // Skip the line prefix, up to and including the ':'
      int16_t c;
      do { c = read(); } while (c >= 0 && c != ':');
      if (c < 0) return false;

      for (uint16_t col = 0; col < size; ++col) {
        const int16_t lo = read_hex_byte(read), hi = read_hex_byte(read);
        if (lo < 0 || hi < 0) return false;
        const uint16_t px = hi << 8 | lo;
        row_buffer[col] = px ?: background;
      }
      draw_pixels(x, y + row, size, 1, row_buffer);

      // Skip to the end of the line
      do { c = read(); } while (c >= 0 && c != '\n');
    }
    return true;
  }

} // namespace ImageDecoders

#endif // GCODE_PREVIEW_MKS
