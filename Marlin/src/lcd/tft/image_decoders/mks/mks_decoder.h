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

#include "../image_decoder.h"

namespace ImageDecoders {

  class MKSDecoder {
  public:
    // Decode a size x size MKS TFT image (";simage:" or ";;gimage:" lines) to a max_w x max_h area,
    // one row at a time into row_buffer. Transparent (0x0000) pixels become the background color.
    static bool draw(read_byte_t read, draw_pixels_t draw_pixels, const uint16_t size, const uint16_t max_w, const uint16_t max_h, const uint16_t background, uint16_t * const row_buffer);
  };

} // namespace ImageDecoders
