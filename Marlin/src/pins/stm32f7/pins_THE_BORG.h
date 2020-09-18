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

#ifndef STM32F7
  #error "Oops! Select an STM32F7 board in 'Tools > Board.'"
#elif HOTENDS > 3 || E_STEPPERS > 3
  #error "The-Borg supports up to 3 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME      "The-Borge"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE              0x1000
#endif

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// Limit Switches
//
#define X_MIN_PIN                           PE9
#define X_MAX_PIN                           PE10
#define Y_MIN_PIN                           PE7
#define Y_MAX_PIN                           PE8
#define Z_MIN_PIN                           PF15
#define Z_MAX_PIN                           PG0
#define E_MIN_PIN                           PE2
#define E_MAX_PIN                           PE3

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA4
#endif

//
// Steppers
//
#define STEPPER_ENABLE_PIN                  PE0

#define X_STEP_PIN                          PC6   // 96, 39 in Arduino
#define X_DIR_PIN                           PC7
#define X_ENABLE_PIN                        PC8

#define Y_STEP_PIN                          PD9
#define Y_DIR_PIN                           PD10
#define Y_ENABLE_PIN                        PD11

#define Z_STEP_PIN                          PE15
#define Z_DIR_PIN                           PG1
#define Z_ENABLE_PIN                        PD8

#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PB2
#define E0_ENABLE_PIN                       PE11

#define E1_STEP_PIN                         PC4
#define E1_DIR_PIN                          PC5
#define E1_ENABLE_PIN                       PB0

#define E2_STEP_PIN                         PC13
#define E2_DIR_PIN                          PC14
#define E2_ENABLE_PIN                       PC15

#define Z2_STEP_PIN                         PC13
#define Z2_DIR_PIN                          PC14
#define Z2_ENABLE_PIN                       PC15

#define SCK_PIN                             PA5
#define MISO_PIN                            PA6
#define MOSI_PIN                            PA7

#define SPI1_SCK_PIN                        PA5
#define SPI1_MISO_PIN                       PA6
#define SPI1_MOSI_PIN                       PA7

#define SPI6_SCK_PIN                        PG13
#define SPI6_MISO_PIN                       PG12
#define SPI6_MOSI_PIN                       PG14

//
// Temperature Sensors
//

#define TEMP_0_PIN                          PC3   // Analog Input
#define TEMP_1_PIN                          PC2   // Analog Input
#define TEMP_2_PIN                          PC1   // Analog Input
#define TEMP_3_PIN                          PC0   // Analog Input

#define TEMP_BED_PIN                        PF10  // Analog Input

#define TEMP_5_PIN                          PE12  // Analog Input, Probe temp

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PD15
#define HEATER_1_PIN                        PD14
#define HEATER_BED_PIN                      PF6

#ifndef FAN_PIN
  #define FAN_PIN                           PD13
#endif
#define FAN1_PIN                            PA0
#define FAN2_PIN                            PA1

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                   PA1
#endif

//
// Misc. Functions
//

//#define CASE_LIGHT_PIN_CI                 PF13
//#define CASE_LIGHT_PIN_DO                 PF14
//#define NEOPIXEL_PIN                      PF13

//
// Průša i3 MK2 Multi Material Multiplexer Support
//

#define E_MUX0_PIN                          PG3
#define E_MUX1_PIN                          PG4

//
// Servos
//

#define SERVO0_PIN                          PE13
#define SERVO1_PIN                          PE14

#define SDSS                                PA8
#define SS_PIN                              PA8
#define LED_PIN                             PA2   // Alive
#define PS_ON_PIN                           PA3
#define KILL_PIN                            -1    //PD5   // EXP2-10
#define PWR_LOSS                            PG5   // Power loss / nAC_FAULT

//
// MAX7219_DEBUG
//
#define MAX7219_CLK_PIN                     PG10  // EXP1-1
#define MAX7219_DIN_PIN                     PD7   // EXP1-3
#define MAX7219_LOAD_PIN                    PD1   // EXP1-5

//
// LCD / Controller
//
//#define SD_DETECT_PIN                     -1    //PB6)     // EXP2-4
#define BEEPER_PIN                          PG10  // EXP1-1
#define LCD_PINS_RS                         PG9   // EXP1-4
#define LCD_PINS_ENABLE                     PD7   // EXP1-3
#define LCD_PINS_D4                         PD1   // EXP1-5
#define LCD_PINS_D5                         PF0   // EXP1-6
#define LCD_PINS_D6                         PD3   // EXP1-7
#define LCD_PINS_D7                         PD4   // EXP1-8
#define BTN_EN1                             PD6   // EXP2-5
#define BTN_EN2                             PD0   // EXP2-3
#define BTN_ENC                             PG11  // EXP1-2
