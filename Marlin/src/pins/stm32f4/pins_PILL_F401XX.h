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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * To build with Arduino IDE use "Discovery F407VG"
 * To build with PlatformIO use environment "STM32F4"
 */
#if NOT_TARGET(STM32F4)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "STM32F4 supports up to 2 hotends / E-steppers."
#endif

#define DEFAULT_MACHINE_NAME  "STM32F401CC"
#define BOARD_INFO_NAME       "BlackPill STM32F4x"

//
// Limit Switches
//
#define X_MIN_PIN              PB12
#define Y_MIN_PIN              PB13
#define Z_MIN_PIN              PB14

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN      PB14
#endif

//
// Steppers
//
// X & Y enable are the same
#define X_STEP_PIN             PB9
#define X_DIR_PIN              PC15
#define X_ENABLE_PIN           PC13

#define Y_STEP_PIN             PB8
#define Y_DIR_PIN              PC14
#define Y_ENABLE_PIN           PC13

#define Z_STEP_PIN             PB5
#define Z_DIR_PIN              PB3
#define Z_ENABLE_PIN           PC13

#define E0_STEP_PIN            PB4
#define E0_DIR_PIN             PA15
#define E0_ENABLE_PIN          PC13

//
// Temperature Sensors
//
#define TEMP_0_PIN             PB0   // Analog Input (HOTEND thermistor)
#define TEMP_BED_PIN           PB1   // Analog Input (BED thermistor)

//
// Heaters / Fans
//
#define HEATER_0_PIN           PA0   // HOTEND MOSFET
#define HEATER_BED_PIN         PA1   // BED MOSFET

#define FAN_PIN                PB10  // FAN1

//
// For Spindle/Laser
//
#define SPINDLE_LASER_PWM_PIN  E0_STEP_PIN
#define SPINDLE_LASER_ENA_PIN  E0_ENABLE_PIN
#define TEMP_COOLER_PIN        TEMP_BED_PIN
#define FLOWMETER_PIN          E0_DIR_PIN

//
// Misc.
//
//#define LED_PIN                PC13

#define SDSS                   PA3
#define TFTGLCD_CS             PA4
#define SD_DETECT_PIN          PB15

// On-board LED pin number
//#define LED_BUILTIN            PC13
//#define LED_BLUE               LED_BUILTIN

//PA5-PA7 - SPI
//PA11, PA12 - USB OTG
//PB2 - BOOT1
//PB6, PB7 - I2C_SCL, I2C_SDA
