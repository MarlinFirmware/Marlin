/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#ifndef GRAPHICAL_TFT_UPSCALE
  #define GRAPHICAL_TFT_UPSCALE 2
#endif

#ifndef TFT_WIDTH
  #if GRAPHICAL_TFT_UPSCALE == 3
    #define TFT_WIDTH 480
  #else
    #define TFT_WIDTH 320
  #endif
#endif
#ifndef TFT_HEIGHT
  #if GRAPHICAL_TFT_UPSCALE == 3
    #define TFT_HEIGHT 320
  #else
    #define TFT_HEIGHT 240
  #endif
#endif

#ifndef TFT_PIXEL_OFFSET_X
  #if GRAPHICAL_TFT_UPSCALE == 2
    #define TFT_PIXEL_OFFSET_X 32
  #else
    #define TFT_PIXEL_OFFSET_X 48
  #endif
#endif

#ifndef TFT_PIXEL_OFFSET_Y
  #define TFT_PIXEL_OFFSET_Y 32 // 32 is best for both 320x240 and 480x320
#endif
