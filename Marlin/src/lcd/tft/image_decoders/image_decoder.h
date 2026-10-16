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
 * Streaming thumbnail decoders for the G-code preview
 *
 * The encoded image is read one byte at a time and the decoded pixels are
 * delivered in small rectangles, so no image buffer is needed.
 */

#include <stdint.h>

namespace ImageDecoders {

  // Return the next byte of the encoded image, or -1 at the end
  typedef int16_t (*read_byte_t)();

  // Draw decoded RGB565 pixels, row by row, to a rectangle relative to the preview area.
  // Decoders center the image in the area.
  typedef void (*draw_pixels_t)(const uint16_t x, const uint16_t y, const uint16_t w, const uint16_t h, uint16_t * const pixels);

} // namespace ImageDecoders
