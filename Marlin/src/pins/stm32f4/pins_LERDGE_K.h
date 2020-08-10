/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if !defined(STM32F4) && !defined(STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "LERDGE K supports up to 2 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME      "Lerdge K"
#define DEFAULT_MACHINE_NAME "LERDGE"

#define I2C_EEPROM

// Ignore temp readings during develpment.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Servos
//
//#define SERVO0_PIN                        PD12

//
// Limit Switches
//
#define X_STOP_PIN                          PG3
#define Y_STOP_PIN                          PG4
#define Z_STOP_PIN                          PG5

//
// Z Probe (when not Z_MIN_PIN)
//
//#ifndef Z_MIN_PROBE_PIN
//  #define Z_MIN_PROBE_PIN  PB15
//#endif

//
// Filament runout
//
#define FIL_RUNOUT_PIN                      PE6
#define FIL_RUNOUT2_PIN                     PE7

//
// Steppers
//
#define X_STEP_PIN                          PG1
#define X_DIR_PIN                           PB10
#define X_ENABLE_PIN                        PG0
//#ifndef X_CS_PIN
//  #define X_CS_PIN       PE0
//#endif

#define Y_STEP_PIN                          PF14
#define Y_DIR_PIN                           PF15
#define Y_ENABLE_PIN                        PF13
//#ifndef Y_CS_PIN
//  #define Y_CS_PIN       PE1
//#endif

#define Z_STEP_PIN                          PF11
#define Z_DIR_PIN                           PF12
#define Z_ENABLE_PIN                        PC5
//#ifndef Z_CS_PIN
//  #define Z_CS_PIN       PE2
//#endif

#define E0_STEP_PIN                         PC14
#define E0_DIR_PIN                          PC13
#define E0_ENABLE_PIN                       PC15
//#ifndef E0_CS_PIN
//  #define E0_CS_PIN      PE3
//#endif

#define E1_STEP_PIN                         PF1
#define E1_DIR_PIN                          PF0
#define E1_ENABLE_PIN                       PF2
//#ifndef E1_CS_PIN
//  #define E1_CS_PIN      PE4
//#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC1   // Analog Input
#define TEMP_1_PIN                          PC2   // Analog Input
#define TEMP_BED_PIN                        PC0   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1
#define HEATER_1_PIN                        PA0
#define HEATER_BED_PIN                      PA2

#ifndef FAN_PIN
  #define FAN_PIN                           PC15
#endif
#define FAN1_PIN                            PF6
#define FAN2_PIN                            PF7

#define ORIG_E0_AUTO_FAN_PIN            FAN1_PIN  // Use this by NOT overriding E0_AUTO_FAN_PIN

//
// LED / Lighting
//
//#define CASE_LIGHT_PIN_CI                 -1
//#define CASE_LIGHT_PIN_DO                 -1
//#define NEOPIXEL_PIN                      -1

//
// Prusa i3 MK2 Multi-Material Multiplexer Support
//
//#define E_MUX0_PIN                        -1
//#define E_MUX1_PIN                        -1

//
// SD support
//
#define SDIO_SUPPORT

//
// Misc. Functions
//
#define SDSS                                PC11
#define LED_PIN                             PC7   // Alive
#define PS_ON_PIN                           -1
#define KILL_PIN                            -1
#define POWER_LOSS_PIN                      -1    // Power-loss / nAC_FAULT

#define SCK_PIN                             PC12
#define MISO_PIN                            PC8
#define MOSI_PIN                            PD2
#define SS_PIN                              PC11

//
// LCD / Controller
//

// TODO: Replace these with the correct FSMC pins, once known
#define SD_DETECT_PIN                       -1
#define BEEPER_PIN                          PD12
#define LCD_PINS_RS                         -1
#define LCD_PINS_ENABLE                     -1
#define LCD_PINS_D4                         -1
#define LCD_PINS_D5                         -1
#define LCD_PINS_D6                         -1
#define LCD_PINS_D7                         -1

#define BTN_EN1                             PE3
#define BTN_EN2                             PE4
#define BTN_ENC                             PE2

//
// ST7920 Delays
//
#if HAS_GRAPHICAL_LCD
  #define BOARD_ST7920_DELAY_1 DELAY_NS(96)
  #define BOARD_ST7920_DELAY_2 DELAY_NS(48)
  #define BOARD_ST7920_DELAY_3 DELAY_NS(715)
#endif
