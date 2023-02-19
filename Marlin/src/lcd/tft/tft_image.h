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

#include "../../inc/MarlinConfigPre.h"

#include <stdint.h>

enum MarlinImage : uint8_t {
  imgBootScreen = 0x00,
  imgHotEnd,
  imgBed,
  imgBedHeated,
  imgChamber,
  imgChamberHeated,
  imgFanIdle,
  imgFanSlow0,
  imgFanSlow1,
  imgFanFast0,
  imgFanFast1,
  imgFeedRate,
  imgFlowRate,
  imgSD,
  imgMenu,
  imgSettings,
  imgDirectory,
  imgConfirm,
  imgCancel,
  imgIncrease,
  imgDecrease,
  imgBack,
  imgUp,
  imgDown,
  imgLeft,
  imgRight,
  imgRefresh,
  imgLeveling,
  imgSlider,
  imgHome,
  imgBtn52Rounded,
  imgBtn39Rounded,
  imgCount,
  noImage = imgCount,
  imgPageUp = imgLeft,
  imgPageDown = imgRight
};

enum colorMode_t : uint8_t {
  NOCOLORS = 0x00,
  MONOCHROME = 0x01,  // 1 bit per pixel
  GREYSCALE1 = 0x01,  // 1 bit per pixel
  GREYSCALE2,         // 2 bits per pixel
  GREYSCALE4,         // 4 bits per pixel
  HIGHCOLOR           // 16 bits per pixel
};

typedef colorMode_t ColorMode;

#ifdef __AVR__
  #define IMG_PACKED __attribute__((__packed__))
#else
  #define IMG_PACKED
#endif

typedef struct IMG_PACKED {
  void *data;
  uint16_t width;
  uint16_t height;
  colorMode_t colorMode;
} tImage;

extern const tImage Images[imgCount];
