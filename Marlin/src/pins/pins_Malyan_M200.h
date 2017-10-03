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

// ignore temp readings during develpment.
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

// Enable EEPROM Emulation for this board
#define FLASH_EEPROM_EMULATION

// PINCHECK - Looking at traces, 38 and 35 are to some endstop. Y is uncertain
//
// Limit Switches
//
#define X_MIN_PIN          14 //37
#define Y_MIN_PIN          15   // 41
#define Z_MIN_PIN          16  //38

//
// Steppers
//
#define X_STEP_PIN          29
#define X_DIR_PIN           28
#define X_ENABLE_PIN       27

#define Y_STEP_PIN         29 //40
#define Y_DIR_PIN          28 //26
#define Y_ENABLE_PIN       27 //25

#define Z_STEP_PIN         29 //22 // Z1 STP
#define Z_DIR_PIN          28 //20 // Z1 DIR
#define Z_ENABLE_PIN       27 //21 // Z1 ENA

// PINCHECK - E0 DIIR is unknown
#define E0_STEP_PIN        29 //21 // Z2 STP
#define E0_DIR_PIN         28 //30 // Z2 DIR
#define E0_ENABLE_PIN      27 //18 // Z2 ENA

//
// Temperature Sensors
//
#define TEMP_0_PIN         11   // Analog Input (HOTEND0 thermistor)
#define TEMP_BED_PIN        0   // Analog Input (BED thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN       13   //43 // HOTEND0 MOSFET
#define HEATER_BED_PIN     0 // 42 // BED MOSFET

// PINCHECK - Need to find pin assignment for these.
// This board has only the controller fan and the extruder fan
//#define FAN_PIN            -1 // FAN1 header on board - PRINT FAN
#define FAN1_PIN            -1 // FAN2 header on board - CONTROLLER FAN
#define FAN2_PIN           -1 // FAN3 header on board - EXTRUDER0 FAN
