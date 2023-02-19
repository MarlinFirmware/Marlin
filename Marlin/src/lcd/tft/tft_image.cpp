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

const tImage Images[imgCount]={ 
  
{ nullptr,     0,0, NOCOLORS },

#if ENABLED(SHOW_BOOTSCREEN)
  
{ (void *)marlin_logo_112x38x1,    112, 38, GREYSCALE1 },
{ (void *)marlin_logo_228x255x2,  228, 255, GREYSCALE2 },
{ (void *)marlin_logo_228x255x4,  228, 255, GREYSCALE4 },
{ (void *)marlin_logo_195x59x16,  195,  59, HIGHCOLOR  },
{ (void *)marlin_logo_320x240x16, 320, 240, HIGHCOLOR  },
{ (void *)marlin_logo_480x320x16, 480, 320, HIGHCOLOR  },
  
#endif
  
{ (void *)background_320x30x16,   320,  30, HIGHCOLOR  },
{ (void *)hotend_64x64x4,          64,  64, GREYSCALE4 },
{ (void *)bed_64x64x4,             64,  64, GREYSCALE4 },
{ (void *)bed_heated_64x64x4,      64,  64, GREYSCALE4 },
{ (void *)chamber_64x64x4,         64,  64, GREYSCALE4 },
{ (void *)chamber_heated_64x64x4,  64,  64, GREYSCALE4 },
{ (void *)fan0_64x64x4,            64,  64, GREYSCALE4 },
{ (void *)fan1_64x64x4,            64,  64, GREYSCALE4 },
{ (void *)fan_slow0_64x64x4,       64,  64, GREYSCALE4 },
{ (void *)fan_slow1_64x64x4,       64,  64, GREYSCALE4 },
{ (void *)fan_fast0_64x64x4,       64,  64, GREYSCALE4 },
{ (void *)fan_fast1_64x64x4,       64,  64, GREYSCALE4 },
{ (void *)sd_64x64x4,              64,  64, GREYSCALE4 },
{ (void *)home_64x64x4,            64,  64, GREYSCALE4 },
{ (void *)btn_rounded_64x52x4,     64,  52, GREYSCALE4 },
{ (void *)btn_rounded_42x39x4,     42,  39, GREYSCALE4 },
{ (void *)menu_64x64x4,            64,  64, GREYSCALE4 },
{ (void *)settings_64x64x4,        64,  64, GREYSCALE4 },
{ (void *)confirm_64x64x4,         64,  64, GREYSCALE4 },
{ (void *)cancel_64x64x4,          64,  64, GREYSCALE4 },
{ (void *)increase_64x64x4,        64,  64, GREYSCALE4 },
{ (void *)decrease_64x64x4,        64,  64, GREYSCALE4 },
{ (void *)pause_64x64x4,           64,  64, GREYSCALE4 },
{ (void *)feedrate_32x32x4,        32,  32, GREYSCALE4 },
{ (void *)flowrate_32x32x4,        32,  32, GREYSCALE4 },
{ (void *)directory_32x32x4,       32,  32, GREYSCALE4 },
{ (void *)back_32x32x4,            32,  32, GREYSCALE4 },
{ (void *)up_32x32x4,              32,  32, GREYSCALE4 },
{ (void *)down_32x32x4,            32,  32, GREYSCALE4 },
{ (void *)left_32x32x4,            32,  32, GREYSCALE4 },
{ (void *)right_32x32x4,           32,  32, GREYSCALE4 },
{ (void *)leveling_32x32x4,        32,  32, GREYSCALE4 },
{ (void *)slider_8x16x4,            8,  16, GREYSCALE4 } 

};     // END OF array 

#endif // HAS_GRAPHICAL_TFT
