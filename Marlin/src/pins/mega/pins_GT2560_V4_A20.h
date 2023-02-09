/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Geeetech A20 GT2560 V4.x board pin assignments
 * ATmega2560
 */

#define BOARD_INFO_NAME "GT2560 4.x"

#define LCD_PINS_RS                           PinE3
#define LCD_PINS_ENABLE                       PinC1
#define LCD_PINS_D4                           PinD0
#define LCD_PINS_D7                           PinH3

#define SPEAKER                                  // The speaker can produce tones

#if IS_NEWPANEL
  #define BTN_EN1                             PinH1
  #define BTN_EN2                             PinH0
  #define BTN_ENC                             PinD2
#endif

#include "pins_GT2560_V3.h"
