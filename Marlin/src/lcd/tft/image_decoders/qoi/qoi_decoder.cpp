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
 * Streaming decoder for the QOI image format (https://qoiformat.org)
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(GCODE_PREVIEW_QOI)

#include "qoi_decoder.h"

namespace ImageDecoders {

  typedef struct { uint8_t r, g, b, a; } qoi_rgba_t;

  bool QOIDecoder::draw(read_byte_t read, draw_pixels_t draw_pixels, const uint16_t max_w, const uint16_t max_h, uint16_t * const row_buffer) {
    // Header: "qoif", width, height (big-endian 32-bit), channels, colorspace
    uint8_t head[14];
    for (uint8_t i = 0; i < sizeof(head); ++i) {
      const int16_t c = read();
      if (c < 0) return false;
      head[i] = uint8_t(c);
    }
    if (head[0] != 'q' || head[1] != 'o' || head[2] != 'i' || head[3] != 'f') return false;
    const uint32_t width  = uint32_t(head[4]) << 24 | uint32_t(head[5]) << 16 | uint32_t(head[6]) << 8 | head[7],
                   height = uint32_t(head[8]) << 24 | uint32_t(head[9]) << 16 | uint32_t(head[10]) << 8 | head[11];
    if (!width || !height || width > max_w || height > max_h) return false;
    const uint16_t w = width, h = height;
    const uint16_t x = (max_w - w) / 2, y = (max_h - h) / 2;  // Center the image

    qoi_rgba_t index[64] = {}, px = { 0, 0, 0, 255 };
    uint8_t run = 0;

    for (uint16_t row = 0; row < h; ++row) {
      for (uint16_t col = 0; col < w; ++col) {
        if (run)
          --run;
        else {
          const int16_t b1 = read();
          if (b1 < 0) return false;
          if (b1 == 0xFE || b1 == 0xFF) {                  // QOI_OP_RGB / QOI_OP_RGBA
            const int16_t r = read(), g = read(), b = read(), a = b1 == 0xFF ? read() : px.a;
            if (r < 0 || g < 0 || b < 0 || a < 0) return false;
            px = { uint8_t(r), uint8_t(g), uint8_t(b), uint8_t(a) };
          }
          else switch (b1 >> 6) {
            case 0: px = index[b1]; break;                 // QOI_OP_INDEX
            case 1:                                        // QOI_OP_DIFF
              px.r += ((b1 >> 4) & 3) - 2;
              px.g += ((b1 >> 2) & 3) - 2;
              px.b += (b1 & 3) - 2;
              break;
            case 2: {                                      // QOI_OP_LUMA
              const int16_t b2 = read();
              if (b2 < 0) return false;
              const int8_t vg = (b1 & 0x3F) - 32;
              px.r += vg - 8 + ((b2 >> 4) & 0x0F);
              px.g += vg;
              px.b += vg - 8 + (b2 & 0x0F);
            } break;
            case 3: run = b1 & 0x3F; break;                // QOI_OP_RUN (this pixel and 'run' more)
          }
          index[(px.r * 3 + px.g * 5 + px.b * 7 + px.a * 11) & 63] = px;
        }
        row_buffer[col] = (px.r & 0xF8) << 8 | (px.g & 0xFC) << 3 | px.b >> 3;
      }
      draw_pixels(x, y + row, w, 1, row_buffer);
    }
    return true;
  }

} // namespace ImageDecoders

#endif // GCODE_PREVIEW_QOI
