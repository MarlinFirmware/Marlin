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

#define ALLOW_STM32DUINO
#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "STM32F4 VAkE supports up to 2 hotends / E steppers."
#endif

#define DEFAULT_MACHINE_NAME "STM32F446VET6"
#define BOARD_INFO_NAME      "STM32F4 VAkE"

//#define I2C_EEPROM
#define MARLIN_EEPROM_SIZE                0x1000  // 4K

//
// Servos
//
//#define SERVO0_PIN                        PE13
//#define SERVO1_PIN                        PE14

//
// Limit Switches
//
#define X_STOP_PIN                          PE10
#define Y_STOP_PIN                          PE9
#define Z_STOP_PIN                          PE8

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA4
#endif

//
// Filament runout
//
#define FIL_RUNOUT_PIN                      PA3

//
// Steppers
//

#define STEPPER_ENABLE_PIN                  PB2

#define X_STEP_PIN                          PC6   // X_STEP
#define X_DIR_PIN                           PC7   // X_DIR
#define X_ENABLE_PIN                        PB2   //
#ifndef X_CS_PIN
  #define X_CS_PIN                          PC8   // X_CS
#endif

#define Y_STEP_PIN                          PD9   // Y_STEP
#define Y_DIR_PIN                           PD10  // Y_DIR
#define Y_ENABLE_PIN                        PB2   //
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PD11  // Y_CS
#endif

#define Z_STEP_PIN                          PE15  // Z_STEP
#define Z_DIR_PIN                           PB10  // Z_DIR
#define Z_ENABLE_PIN                        PB2
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PD8
#endif

#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PB13
#define E0_ENABLE_PIN                       PB2
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PE11
#endif

#define E1_STEP_PIN                         PC4
#define E1_DIR_PIN                          PC5
#define E1_ENABLE_PIN                       PB2
#ifndef E1_CS_PIN
  #define E1_CS_PIN                         PB0
#endif

#define SD_SCK_PIN                          PE12  // PA5   // SPI1 for SD card
#define SD_MISO_PIN                         PE13  // PA6
#define SD_MOSI_PIN                         PE14  // PA7

// added for SD card : optional or not ???
//#define SD_CHIP_SELECT_PIN                SDSS  // The default chip select pin for the SD card is SS.
// The following three pins must not be redefined for hardware SPI.
//#define SPI_MOSI_PIN               SD_MOSI_PIN  // SPI Master Out Slave In pin
//#define SPI_MISO_PIN               SD_MISO_PIN  // SPI Master In Slave Out pin
//#define SPI_SCK_PIN                 SD_SCK_PIN  // SPI Clock pin

//
// Temperature Sensors (Analog inputs)
//

#define TEMP_0_PIN                          PC0   // Analog Input
#define TEMP_1_PIN                          PC1   // Analog Input
#define TEMP_2_PIN                          PC2   // Analog Input
#define TEMP_3_PIN                          PC3   // Analog Input
#define TEMP_BED_PIN                        PC3   // Analog Input

//
// Heaters / Fans
//

#define HEATER_0_PIN                        PD15
#define HEATER_1_PIN                        PD14
#define HEATER_BED_PIN                      PD12

#ifndef FAN_PIN
  #define FAN_PIN                           PD13
#endif
#define FAN1_PIN                            PB5   // PA0
#define FAN2_PIN                            PB4   // PA1

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                   PD13
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
#if HAS_PRUSA_MMU1
  //#define E_MUX0_PIN                      PG3
  //#define E_MUX1_PIN                      PG4
#endif

#define LED_PIN                             PB14  // Alive
#define PS_ON_PIN                           PE0
#define KILL_PIN                            PD5
#define POWER_LOSS_PIN                      PA4   // ?? Power loss / nAC_FAULT

#if ENABLED(SDSUPPORT)
  #define SD_DETECT_PIN              EXP2_04_PIN
  #define SD_SS_PIN                         PB15  // USD_CS -> CS for onboard SD
#endif

/**
 *        ------                  ------
 *   PC9 |10  9 | PB12         ? |10  9 | ?
 *   PD7 | 8  7 | PC12       PD6 | 8  7 | ?
 *   PD1 | 6  5   PD2        PD0 | 6  5   ?
 *   PD3 | 4  3 | PD4        PB7 | 4  3 | RESET
 *   GND | 2  1 | 5V         GND | 2  1 | 3.3V
 *        ------                  ------
 *         EXP1                    EXP2
 */
#define EXP1_03_PIN                         PD4
#define EXP1_04_PIN                         PD3
#define EXP1_05_PIN                         PD2
#define EXP1_06_PIN                         PD1
#define EXP1_07_PIN                         PC12
#define EXP1_08_PIN                         PD7
#define EXP1_09_PIN                         PB12
#define EXP1_10_PIN                         PC9

#define EXP2_03_PIN                         -1
#define EXP2_04_PIN                         PB7
//#define EXP2_05_PIN                       ?
#define EXP2_06_PIN                         PD0
//#define EXP2_07_PIN                       ?
#define EXP2_08_PIN                         PD6
//#define EXP2_09_PIN                       ?
//#define EXP2_10_PIN                       ?

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #if ENABLED(SDSUPPORT)
    #define SDSS                            PB6   // CS for SD card in LCD
  #endif

  #define BEEPER_PIN                 EXP1_10_PIN

  #define BTN_EN1                    EXP2_08_PIN
  #define BTN_EN2                    EXP2_06_PIN
  #define BTN_ENC                    EXP1_09_PIN

  #define LCD_PINS_ENABLE            EXP1_08_PIN
  #define LCD_PINS_RS                EXP1_07_PIN

  #define LCD_PINS_D4                EXP1_06_PIN
  #define LCD_PINS_D5                EXP1_05_PIN
  #define LCD_PINS_D6                EXP1_04_PIN
  #define LCD_PINS_D7                EXP1_03_PIN

#endif

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                96
  #define BOARD_ST7920_DELAY_2                48
  #define BOARD_ST7920_DELAY_3               715
#endif
