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
 * Creality v2.4.S1_200 (STM32F401RC) board pin assignments
 * Board silkscreen: CR-FDM-v2.4.S1_200, as found in the Sermoon V1 / V1 Pro
 * From Creality's Sermoon V1 source: pins_CREALITY_F401RE.h
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Creality v2.4.S1_200 only supports 1 hotend / E stepper."
#endif

#define BOARD_INFO_NAME      "Creality V2.4.S1_200"
#define DEFAULT_MACHINE_NAME "Sermoon V1"

#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA11
  #define IIC_EEPROM_SCL                    PA12
  #define MARLIN_EEPROM_SIZE              0x800U  // 2K (24C16)
#endif

#define BOARD_LCD_SERIAL_PORT 2                   // Touchscreen

//
// Servos
//
#define SERVO0_PIN                          PC2   // BLTouch OUT

//
// Limit Switches
//
#define X_STOP_PIN                          PC4   // X homes to max
#define Y_STOP_PIN                          PB13
#define Z_STOP_PIN                          PB3   // Z homes to max

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PC3   // BLTouch IN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC6
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PB8   // Shared
#define X_STEP_PIN                          PA7
#define X_DIR_PIN                           PA4

#define Y_ENABLE_PIN                X_ENABLE_PIN
#define Y_STEP_PIN                          PB0
#define Y_DIR_PIN                           PB10

#define Z_ENABLE_PIN                X_ENABLE_PIN
#define Z_STEP_PIN                          PB7
#define Z_DIR_PIN                           PB6

#define E0_ENABLE_PIN               X_ENABLE_PIN
#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PB12

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // TH1
#define TEMP_BED_PIN                        PC0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC5   // HEATER1
#define HEATER_BED_PIN                      PB9   // HOT BED
#define FAN0_PIN                            PA5   // FAN
#define FAN1_PIN                            PC15  // Enclosure (box) fan

#define FAN_SOFT_PWM_REQUIRED

//
// Misc. Functions
//
#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                    PC14  // Enclosure LED
#endif
//#define DOOR_PIN                          PB14  // Door switch (Sermoon V1 Pro), HIGH = open

//
// SD Card
//
#define SD_DETECT_PIN                       PC7
#define SDCARD_CONNECTION                ONBOARD
#define ONBOARD_SDIO
#define BOARD_NO_HOST_DRIVE                       // SD is only seen by the printer
