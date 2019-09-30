/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#if NONE(__STM32F1__, STM32F1xx, __STM32F4__)
  #error "Oops! Select an STM32F1/4 board in 'Tools > Board.'"
#endif

/**
 * 2017 Victor Perez Marlin for stm32f1 test
 */

#define BOARD_INFO_NAME      "Chitu3D"
#define DEFAULT_MACHINE_NAME "STM32F103RET6"

#define STEP_TIMER 1
#define TEMP_TIMER 3

//
// Steppers
//
#define X_STEP_PIN         PE5
#define X_DIR_PIN          PE6
#define X_ENABLE_PIN       PC13
#define X_MIN_PIN          PG10

#define Y_STEP_PIN         PE2
#define Y_DIR_PIN          PE3
#define Y_ENABLE_PIN       PE4
#define Y_MIN_PIN          PA12

#define Z_STEP_PIN         PB9
#define Z_DIR_PIN          PE0
#define Z_ENABLE_PIN       PE1
#define Z_MIN_PIN          PA14

#define E0_STEP_PIN        PB4
#define E0_DIR_PIN         PB5
#define E0_ENABLE_PIN      PB8

//
// Heaters / Fans
//
#define HEATER_0_PIN       PD12     // HOT-END

#define HEATER_BED_PIN     PG11     // HOT-BED

#ifndef FAN_PIN
  #define FAN_PIN          PG14     // MAIN BOARD FAN
#endif

#define FAN_SOFT_PWM

//
// Temperature Sensors
//
#define TEMP_BED_PIN       PA0   // Analog Input
#define TEMP_0_PIN         PA1   // Analog Input
