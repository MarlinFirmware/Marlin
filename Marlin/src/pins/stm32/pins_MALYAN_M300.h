/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * MALYAN M300 pin assignments
 */

#if !defined(__STM32F1__) && !defined(STM32F1xx) && !defined(STM32F0xx)
  #error "Oops! You must be compiling for STM32F070."
#endif

#define BOARD_INFO_NAME "Malyan M300"

// Enable EEPROM Emulation for this board
// This setting should probably be in configuration.h
// but it is literally the only board which uses it.
#define FLASH_EEPROM_EMULATION

//
// SD CARD SPI
//
#define SDSS      SS_PIN

// Based on PWM timer usage, we have to use these timers and soft PWM for the fans
// On STM32F103:
// PB3, PB6, PB7, and PB8 can be used with pwm, which rules out TIM2 and TIM4.
// On STM32F070, 16 and 17 are in use, but 1 and 3 are available.
#define STEP_TIMER 6
#define TEMP_TIMER 7

//
// Limit Switches
//
#define X_MAX_PIN          PC13
#define Y_MAX_PIN          PC14
#define Z_MAX_PIN          PC15
#define Z_MIN_PIN          PB7

//
// Steppers
//
// X & Y enable are the same
#define X_STEP_PIN         PB14
#define X_DIR_PIN          PB13
#define X_ENABLE_PIN       PB10

#define Y_STEP_PIN         PB12
#define Y_DIR_PIN          PB11
#define Y_ENABLE_PIN       PB10

#define Z_STEP_PIN         PB2
#define Z_DIR_PIN          PB1
#define Z_ENABLE_PIN       PB10

#define E0_STEP_PIN        PA7
#define E0_DIR_PIN         PA6
#define E0_ENABLE_PIN      PB0

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // Analog Input (HOTEND0 thermistor)
#define TEMP_BED_PIN       PA4   // Analog Input (BED thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN       PA1   // HOTEND0 MOSFET
#define HEATER_BED_PIN     PA5   // BED MOSFET

#define ORIG_E0_AUTO_FAN_PIN PA8  // The fan work but homing does not anymore...

