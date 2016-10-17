/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Board 99 pin assignments
 */

#define BOARD_NAME          "99 Unknown"

//
// Limit Switches
//
#define X_STOP_PIN         16
#define Y_STOP_PIN         67
#define Z_STOP_PIN         59

//
// Steppers
//
#define X_STEP_PIN          2
#define X_DIR_PIN           3
#define X_ENABLE_PIN       -1

#define Y_STEP_PIN          5
#define Y_DIR_PIN           6
#define Y_ENABLE_PIN       -1

#define Z_STEP_PIN         62
#define Z_DIR_PIN          63
#define Z_ENABLE_PIN       -1

#define E0_STEP_PIN        65
#define E0_DIR_PIN         66
#define E0_ENABLE_PIN      -1

//
// Temperature Sensors
//
#define TEMP_0_PIN          6   // ANALOG INPUT - NOT DIGITAL OUTPUT
#define TEMP_BED_PIN       10

//
// Heaters / Fans
//
#define HEATER_0_PIN       13
#define HEATER_BED_PIN      4

#define SDSS               53
#define PS_ON_PIN           9
