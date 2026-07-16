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
 */

#pragma once

/**
 * Wanhao 0327001 v2.0 pin assignments
 * MCU: AT32F403ARGT7
 */

#ifndef ARDUINO_ARCH_AT32
  #error "AT32 HAL required for this board"
#endif

// Stepper motor pins
#define X_STEP_PIN                          PA0
#define X_DIR_PIN                           PA1
#define X_ENABLE_PIN                        PA2

#define Y_STEP_PIN                          PA3
#define Y_DIR_PIN                           PA4
#define Y_ENABLE_PIN                        PA5

#define Z_STEP_PIN                          PA6
#define Z_DIR_PIN                           PA7
#define Z_ENABLE_PIN                        PB0

#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PB2
#define E0_ENABLE_PIN                       PB3

// Endstops
#define X_MIN_PIN                           PB4
#define X_MAX_PIN                           PB5
#define Y_MIN_PIN                           PB6
#define Y_MAX_PIN                           PB7
#define Z_MIN_PIN                           PC0
#define Z_MAX_PIN                           PC1

// Temperature sensors
#define TEMP_0_PIN                          PC2   // HE0
#define TEMP_BED_PIN                        PC3   // BED

// Heaters
#define HEATER_0_PIN                        PC4
#define HEATER_BED_PIN                      PC5

// Fans
#define FAN0_PIN                            PC6
#define FAN1_PIN                            PC7

// LCD / Controller
#define BEEPER_PIN                          PC8
#define LCD_BACKLIGHT_PIN                   PC9
#define LCD_RESET_PIN                       PC10

// SD Card
#define SD_DETECT_PIN                       PC11
#define SD_SS_PIN                           PC12  // CS

// SPI pins
#define SCK_PIN                             PC13
#define MISO_PIN                            PC14
#define MOSI_PIN                            PC15

// EEPROM emulation on SD card (sdsupport is enabled for this board)
#define SDCARD_EEPROM_EMULATION

// Note: Pin mappings are approximate and need verification from schematics
// The actual pin assignments should be verified against the Wanhao 0327001 v2.0 board
