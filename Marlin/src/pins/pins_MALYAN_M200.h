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
 * MALYAN M200 pin assignments
 */

#ifndef __STM32F1__
  #error "Oops! You must be compiling for STM32."
#endif

#define BOARD_NAME "MALYANM200"

// Enable EEPROM Emulation for this board
// This setting should probably be in configuration.h
// but it is literally the only board which uses it.
#define FLASH_EEPROM_EMULATION

#define SDSS SS_PIN

//
// Limit Switches
//
#define X_MIN_PIN          PB4
#define Y_MIN_PIN          PA15
#define Z_MIN_PIN          PB5

//
// Steppers
//
// X & Y enable are the same
#define X_STEP_PIN         PB14
#define X_DIR_PIN          PB15
#define X_ENABLE_PIN       PA8

#define Y_STEP_PIN         PB12
#define Y_DIR_PIN          PB13
#define Y_ENABLE_PIN       PA8

#define Z_STEP_PIN         PB10
#define Z_DIR_PIN          PB2
#define Z_ENABLE_PIN       PB11

#define E0_STEP_PIN        PB0
#define E0_DIR_PIN         PC13
#define E0_ENABLE_PIN      PB1

//
// Temperature Sensors
//
#define TEMP_0_PIN         PA0   // Analog Input (HOTEND0 thermistor)
#define TEMP_BED_PIN       PA1   // Analog Input (BED thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN       PB6   // HOTEND0 MOSFET
#define HEATER_BED_PIN     PB7   // BED MOSFET

// FAN_PIN is commented out here because the M200 example
// Configuration_adv.h does NOT override E0_AUTO_FAN_PIN.
#ifndef FAN_PIN
  //#define FAN_PIN        PB8   // FAN1 header on board - PRINT FAN
#endif
#define FAN1_PIN           PB3   // FAN2 header on board - CONTROLLER FAN
#define FAN2_PIN           -1    // FAN3 header on board - EXTRUDER0 FAN

// This board has only the controller fan and the extruder fan
// If someone hacks to put a direct power fan on the controller, PB3 could
// be used as a separate print cooling fan.
#define ORIG_E0_AUTO_FAN_PIN PB8
