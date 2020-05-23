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
 * Common pin assignments for all RUMBA32 boards
 */

#ifndef STM32F4
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "RUMBA32 boards support up to 3 hotends / E-steppers."
#endif

#define RUMBA32_V1_0
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE 0x1000                 // 4KB

//
// Limit Switches
//
#define X_MIN_PIN                           PB12
#define X_MAX_PIN                           PB13
#define Y_MIN_PIN                           PB15
#define Y_MAX_PIN                           PD8
#define Z_MIN_PIN                           PD9
#define Z_MAX_PIN                           PD10

//
// Steppers
//
#define X_STEP_PIN                          PA0
#define X_DIR_PIN                           PC15
#define X_ENABLE_PIN                        PC11
#define X_CS_PIN                            PC14

#define Y_STEP_PIN                          PE5
#define Y_DIR_PIN                           PE6
#define Y_ENABLE_PIN                        PE3
#define Y_CS_PIN                            PE4

#define Z_STEP_PIN                          PE1
#define Z_DIR_PIN                           PE2
#define Z_ENABLE_PIN                        PB7
#define Z_CS_PIN                            PE0

#define E0_STEP_PIN                         PB5
#define E0_DIR_PIN                          PB6
#define E0_ENABLE_PIN                       PC12
#define E0_CS_PIN                           PC13

#define E1_STEP_PIN                         PD6
#define E1_DIR_PIN                          PD7
#define E1_ENABLE_PIN                       PD4
#define E1_CS_PIN                           PD5

#define E2_STEP_PIN                         PD2
#define E2_DIR_PIN                          PD3
#define E2_ENABLE_PIN                       PD0
#define E2_CS_PIN                           PD1

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC4
#define TEMP_1_PIN                          PC3
#define TEMP_2_PIN                          PC2
#define TEMP_3_PIN                          PC1
#define TEMP_BED_PIN                        PC0

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC6
#define HEATER_1_PIN                        PC7
#define HEATER_2_PIN                        PC8
#define HEATER_BED_PIN                      PA1

#define FAN_PIN                             PC9
#define FAN1_PIN                            PA8

//
// I2C
//
#define SCK_PIN                             PA5
#define MISO_PIN                            PA6
#define MOSI_PIN                            PA7

//
// Misc. Functions
//
#define LED_PIN                             PB14
#define BTN_PIN                             PC10
#define PS_ON_PIN                           PE11
#define KILL_PIN                            PC5

#define SDSS                                PA2
#define SD_DETECT_PIN                       PB0
#define BEEPER_PIN                          PE8

//
// LCD / Controller
//
#if HAS_SPI_LCD

  #define BTN_EN1                           PB2
  #define BTN_EN2                           PB1
  #define BTN_ENC                           PE7

  #define LCD_PINS_RS                       PE10
  #define LCD_PINS_ENABLE                   PE9
  #define LCD_PINS_D4                       PE12

  #if ENABLED(MKS_MINI_12864)
    #define DOGLCD_CS                       PE13
    #define DOGLCD_A0                       PE14
  #endif

  #if ENABLED(ULTIPANEL)
    #define LCD_PINS_D5                     PE13
    #define LCD_PINS_D6                     PE14
    #define LCD_PINS_D7                     PE15
  #endif

#endif
