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

#include "../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_TFT

#include "tft_image.h"
#include "ui_common.h"

extern const uint8_t marlin_logo_112x38x1[];
extern const uint8_t marlin_logo_228x255x2[];
extern const uint8_t marlin_logo_228x255x4[];
extern const uint16_t marlin_logo_195x59x16[];
extern const uint16_t marlin_logo_320x240x16[];
extern const uint16_t marlin_logo_480x320x16[];
extern const uint16_t background_320x30x16[];

extern const uint8_t hotend_64x64x4[];
extern const uint8_t bed_64x64x4[], bed_heated_64x64x4[];
extern const uint8_t chamber_64x64x4[], chamber_heated_64x64x4[];
extern const uint8_t fan0_64x64x4[];
extern const uint8_t fan_slow0_64x64x4[], fan_slow1_64x64x4[];
extern const uint8_t fan_fast0_64x64x4[], fan_fast1_64x64x4[];
extern const uint8_t sd_64x64x4[];
extern const uint8_t home_64x64x4[];
extern const uint8_t btn_rounded_64x52x4[];
extern const uint8_t btn_rounded_42x39x4[];
extern const uint8_t menu_64x64x4[];
extern const uint8_t settings_64x64x4[];
extern const uint8_t confirm_64x64x4[];
extern const uint8_t cancel_64x64x4[];
extern const uint8_t increase_64x64x4[];
extern const uint8_t decrease_64x64x4[];
extern const uint8_t pause_64x64x4[];

extern const uint8_t feedrate_32x32x4[];
extern const uint8_t flowrate_32x32x4[];
extern const uint8_t directory_32x32x4[];
extern const uint8_t back_32x32x4[];
extern const uint8_t up_32x32x4[];
extern const uint8_t down_32x32x4[];
extern const uint8_t left_32x32x4[];
extern const uint8_t right_32x32x4[];
extern const uint8_t refresh_32x32x4[];
extern const uint8_t leveling_32x32x4[];

extern const uint8_t slider_8x16x4[];

const tImage Images[imgCount] = {
  #if ENABLED(SHOW_BOOTSCREEN)                              // imgBootScreen
    #if ENABLED(BOOT_MARLIN_LOGO_SMALL)
      { (void *)marlin_logo_195x59x16,  195,  59, HIGHCOLOR },
    #else
      MARLIN_LOGO_FULL_SIZE,
    #endif
  #else
    { nullptr,     0,0, NOCOLORS },
  #endif

  { (void *)hotend_64x64x4,          64,  64, GREYSCALE4 }, // imgHotEnd
  { (void *)bed_64x64x4,             64,  64, GREYSCALE4 }, // imgBed
  { (void *)bed_heated_64x64x4,      64,  64, GREYSCALE4 }, // imgBedHeated
  { (void *)chamber_64x64x4,         64,  64, GREYSCALE4 }, // imgChamber
  { (void *)chamber_heated_64x64x4,  64,  64, GREYSCALE4 }, // imgChamberHeated
  { (void *)fan0_64x64x4,            64,  64, GREYSCALE4 }, // imgFanIdle
  { (void *)fan_slow0_64x64x4,       64,  64, GREYSCALE4 }, // imgFanSlow0
  { (void *)fan_slow1_64x64x4,       64,  64, GREYSCALE4 }, // imgFanSlow1
  { (void *)fan_fast0_64x64x4,       64,  64, GREYSCALE4 }, // imgFanFast0
  { (void *)fan_fast1_64x64x4,       64,  64, GREYSCALE4 }, // imgFanFast1
  { (void *)sd_64x64x4,              64,  64, GREYSCALE4 }, // imgFeedRate
  { (void *)home_64x64x4,            64,  64, GREYSCALE4 }, // imgFlowRate
  { (void *)btn_rounded_64x52x4,     64,  52, GREYSCALE4 }, // imgSD
  { (void *)btn_rounded_42x39x4,     42,  39, GREYSCALE4 }, // imgMenu
  { (void *)menu_64x64x4,            64,  64, GREYSCALE4 }, // imgSettings
  { (void *)settings_64x64x4,        64,  64, GREYSCALE4 }, // imgDirectory
  { (void *)confirm_64x64x4,         64,  64, GREYSCALE4 }, // imgConfirm
  { (void *)cancel_64x64x4,          64,  64, GREYSCALE4 }, // imgCancel
  { (void *)increase_64x64x4,        64,  64, GREYSCALE4 }, // imgIncrease
  { (void *)decrease_64x64x4,        64,  64, GREYSCALE4 }, // imgDecrease
  { (void *)pause_64x64x4,           64,  64, GREYSCALE4 }, // imgBack
  { (void *)feedrate_32x32x4,        32,  32, GREYSCALE4 }, // imgUp
  { (void *)flowrate_32x32x4,        32,  32, GREYSCALE4 }, // imgDown
  { (void *)directory_32x32x4,       32,  32, GREYSCALE4 }, // imgLeft
  { (void *)back_32x32x4,            32,  32, GREYSCALE4 }, // imgRight
  { (void *)up_32x32x4,              32,  32, GREYSCALE4 }, // imgRefresh
  { (void *)down_32x32x4,            32,  32, GREYSCALE4 }, // imgLeveling
  { (void *)left_32x32x4,            32,  32, GREYSCALE4 }, // imgSlider
  { (void *)right_32x32x4,           32,  32, GREYSCALE4 }, // imgHome
  { (void *)leveling_32x32x4,        32,  32, GREYSCALE4 }, // imgBtn52Rounded
  { (void *)slider_8x16x4,            8,  16, GREYSCALE4 }  // imgBtn39Rounded

};

#endif // HAS_GRAPHICAL_TFT
