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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Espressif ESP32 (Tensilica Xtensa LX6) pin assignments
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "PANDA_ZHU"
#define DEFAULT_MACHINE_NAME  BOARD_INFO_NAME

#define SERVO0_PIN                       0


//
// Limit Switches
//
#define X_MIN_PIN                             4
#define Y_MIN_PIN                             35
#define Z_MIN_PIN                             21

//
// Steppers
//
#define X_STEP_PIN                          100+1 
#define X_DIR_PIN                           100
#define X_ENABLE_PIN                        100+24+4
//#define X_CS_PIN                             0

#define Y_STEP_PIN                          100+3
#define Y_DIR_PIN                           100+2 
#define Y_ENABLE_PIN                        100+24+4
//#define Y_CS_PIN                            13

#define Z_STEP_PIN                           100+5
#define Z_DIR_PIN                            100+4
#define Z_ENABLE_PIN                         100+24+4
//#define Z_CS_PIN                             5  // SS_PIN

#define E0_STEP_PIN                          100+7
#define E0_DIR_PIN                           100+6
#define E0_ENABLE_PIN                        100+24+4
//#define E0_CS_PIN                           21

#define E1_STEP_PIN                          100+8+7
#define E1_DIR_PIN                           100+8+6
#define E1_ENABLE_PIN                        100+24+4

#define E2_STEP_PIN                          100+8+4
#define E2_DIR_PIN                           100+8+5
#define E2_ENABLE_PIN                        100+24+4

#define E3_STEP_PIN                          100+8+2
#define E3_DIR_PIN                           100+8+3
#define E3_ENABLE_PIN                        100+24+4

#define E4_STEP_PIN                          100+16+5
#define E4_DIR_PIN                           100+16+6
#define E4_ENABLE_PIN                        100+24+4


//
// Temperature Sensors
//
#define TEMP_0_PIN                            39  // Analog Input
#define TEMP_BED_PIN                          36  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           100+8
#define FAN_PIN                                100+16+2
#define HEATER_BED_PIN                         100+8+1
#define E0_AUTO_FAN_PIN_ESP                    100+16+4
//
// MicroSD card
//
#define SD_MOSI_PIN                           23
#define SD_MISO_PIN                           19
#define SD_SCK_PIN                            18
#define SDSS                                   5
#define SD_DETECT_PIN                          2

#define BEEPER_PIN                  100+24+5 
#define BTN_ENC                     12 

#define BTN_EN1                     33 
#define BTN_EN2                     32 

#define LCD_PINS_RS                    27
#define LCD_PINS_ENABLE                26
#define LCD_PINS_D4                    14
    