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
 * PancakeBot (RAMPS 1.3 based) board pin assignments
 *
 * The PancakeBot is a batter-dispensing XY plotter. It has no Z axis, no extruder
 * and no heaters. A vacuum / pressure pump on the (RAMPS D9) fan output is switched
 * by M106 / M107 to start and stop the batter flow.
 *
 * Ported from https://github.com/PancakeBot/PancakeBot-firmware
 *
 * Front panel (not supported by Marlin: 128x32 ST7565 graphic display with a
 * 3-button / encoder panel, status LEDs and a latching power key):
 *   Display SCK 23, MOSI 64, CS 63, A0 66, backlight 65
 *   Buttons EN1 10, EN2 3, ENC 13, END 11 (the power key shares pin 11)
 *   LEDs: Power On 5, Power Off 6, Working 7, SD Error 8
 */

#include "env_validate.h"

#define BOARD_INFO_NAME      "PancakeBot"
#define DEFAULT_MACHINE_NAME "PancakeBot"

//
// Limit Switches
//
#define X_MIN_PIN                             28
#define Y_MIN_PIN                             40

//
// Steppers
//
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38

#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56

//
// Pump (M106 / M107)
//
#define FAN0_PIN                               9

//
// Power key. Idles low, switches the power supply off when driven high.
//
#define PS_ON_PIN                             11

//
// SD Card
//
#define SD_SS_PIN                             53
#define SD_DETECT_PIN                         49
