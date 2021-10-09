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

/********************************************************************************
 * @file     dwin_lcd.h
 * @author   LEO / Creality3D
 * @date     2019/07/18
 * @version  2.0.1
 * @brief    迪文屏控制操作函数
 ********************************************************************************/

#define DWIN_WIDTH  272
#define DWIN_HEIGHT 480

#include "../common/dwin_api.h"
#include "../common/dwin_set.h"
#include "../common/dwin_font.h"
#include "../common/dwin_color.h"

#define DWIN_FONT_MENU  font10x20
#define DWIN_FONT_STAT  font10x20
#define DWIN_FONT_HEAD  font10x20
#define DWIN_FONT_ALERT font14x28

// Copy area from virtual display area to current screen
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y);
