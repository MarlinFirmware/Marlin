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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(GCODE_PREVIEW_JPEG)

#include "jpeg_decoder.h"
#include "tjpgd.h"

namespace ImageDecoders {

  struct JPEGStream {
    read_byte_t read;
    draw_pixels_t draw_pixels;
    uint16_t x, y;  // Offset to center the image
  };

  // Supply input bytes, or skip them when buff is null
  static size_t jpeg_input(JDEC *jd, uint8_t *buff, size_t nbyte) {
    const JPEGStream &stream = *(JPEGStream*)jd->device;
    size_t n = 0;
    for (; n < nbyte; ++n) {
      const int16_t c = stream.read();
      if (c < 0) break;
      if (buff) buff[n] = uint8_t(c);
    }
    return n;
  }

  // Draw a decoded block of RGB565 pixels
  static int jpeg_output(JDEC *jd, void *bitmap, JRECT *rect) {
    const JPEGStream &stream = *(JPEGStream*)jd->device;
    stream.draw_pixels(stream.x + rect->left, stream.y + rect->top, rect->right - rect->left + 1, rect->bottom - rect->top + 1, (uint16_t*)bitmap);
    return 1;
  }

  bool JPEGDecoder::draw(read_byte_t read, draw_pixels_t draw_pixels, const uint16_t max_w, const uint16_t max_h) {
    uint8_t work[3100];   // TJpgDec work area for JD_SZBUF 512, no fast decode
    JPEGStream stream = { read, draw_pixels, 0, 0 };
    JDEC jdec;
    if (jd_prepare(&jdec, jpeg_input, work, sizeof(work), &stream) != JDR_OK) return false;

    // Use the smallest scale-down (1, 1/2, 1/4, 1/8) that fits
    uint8_t scale = 0;
    while (scale < 3 && ((jdec.width >> scale) > max_w || (jdec.height >> scale) > max_h)) ++scale;
    const uint16_t w = jdec.width >> scale, h = jdec.height >> scale;
    if (w > max_w || h > max_h) return false;
    stream.x = (max_w - w) / 2;
    stream.y = (max_h - h) / 2;

    return jd_decomp(&jdec, jpeg_output, scale) == JDR_OK;
  }

} // namespace ImageDecoders

#endif // GCODE_PREVIEW_JPEG
