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

#include "tft_image.h"
#include "cstddef"

const tImage NoLogo                 = { (void *)NULL, 0, 0, NOCOLORS };

const tImage MarlinLogo112x38x1     = { (void *)marlin_logo_112x38x1, 112, 38, GREYSCALE1 };
const tImage MarlinLogo228x255x2    = { (void *)marlin_logo_228x255x2, 228, 255, GREYSCALE2 };
const tImage MarlinLogo228x255x4    = { (void *)marlin_logo_228x255x4, 228, 255, GREYSCALE4 };
const tImage MarlinLogo195x59x16    = { (void *)marlin_logo_195x59x16,  195,  59, HIGHCOLOR };
const tImage MarlinLogo320x240x16   = { (void *)marlin_logo_320x240x16, 320, 240, HIGHCOLOR };
const tImage MarlinLogo480x320x16   = { (void *)marlin_logo_480x320x16, 480, 320, HIGHCOLOR };
const tImage Background320x30x16    = { (void *)background_320x30x16, 320, 30, HIGHCOLOR };

const tImage HotEnd_64x64x4         = { (void *)hotend_64x64x4, 64, 64, GREYSCALE4 };
const tImage Bed_64x64x4            = { (void *)bed_64x64x4, 64, 64, GREYSCALE4 };
const tImage Bed_Heated_64x64x4     = { (void *)bed_heated_64x64x4, 64, 64, GREYSCALE4 };
const tImage Chamber_64x64x4        = { (void *)chamber_64x64x4, 64, 64, GREYSCALE4 };
const tImage Chamber_Heated_64x64x4 = { (void *)chamber_heated_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan0_64x64x4           = { (void *)fan0_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan1_64x64x4           = { (void *)fan1_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Slow0_64x64x4      = { (void *)fan_slow0_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Slow1_64x64x4      = { (void *)fan_slow1_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Fast0_64x64x4      = { (void *)fan_fast0_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Fast1_64x64x4      = { (void *)fan_fast1_64x64x4, 64, 64, GREYSCALE4 };
const tImage SD_64x64x4             = { (void *)sd_64x64x4, 64, 64, GREYSCALE4 };
const tImage Menu_64x64x4           = { (void *)menu_64x64x4, 64, 64, GREYSCALE4 };
const tImage Settings_64x64x4       = { (void *)settings_64x64x4, 64, 64, GREYSCALE4 };
const tImage Confirm_64x64x4        = { (void *)confirm_64x64x4, 64, 64, GREYSCALE4 };
const tImage Cancel_64x64x4         = { (void *)cancel_64x64x4, 64, 64, GREYSCALE4 };
const tImage Increase_64x64x4       = { (void *)increase_64x64x4, 64, 64, GREYSCALE4 };
const tImage Decrease_64x64x4       = { (void *)decrease_64x64x4, 64, 64, GREYSCALE4 };
const tImage Pause_64x64x4          = { (void *)pause_64x64x4, 64, 64, GREYSCALE4 };

const tImage Feedrate_32x32x4       = { (void *)feedrate_32x32x4, 32, 32, GREYSCALE4 };
const tImage Flowrate_32x32x4       = { (void *)flowrate_32x32x4, 32, 32, GREYSCALE4 };
const tImage Directory_32x32x4      = { (void *)directory_32x32x4, 32, 32, GREYSCALE4 };
const tImage Back_32x32x4           = { (void *)back_32x32x4, 32, 32, GREYSCALE4 };
const tImage Up_32x32x4             = { (void *)up_32x32x4, 32, 32, GREYSCALE4 };
const tImage Down_32x32x4           = { (void *)down_32x32x4, 32, 32, GREYSCALE4 };
const tImage Left_32x32x4           = { (void *)left_32x32x4, 32, 32, GREYSCALE4 };
const tImage Right_32x32x4          = { (void *)right_32x32x4, 32, 32, GREYSCALE4 };
const tImage Refresh_32x32x4        = { (void *)refresh_32x32x4, 32, 32, GREYSCALE4 };
const tImage Leveling_32x32x4       = { (void *)leveling_32x32x4, 32, 32, GREYSCALE4 };

const tImage Slider8x16x4           = { (void *)slider_8x16x4, 8, 16, GREYSCALE4 };

extern const tImage Images[imgCount];
