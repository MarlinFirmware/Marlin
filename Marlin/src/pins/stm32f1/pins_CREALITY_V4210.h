/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * CREALITY 4.2.10 (STM32F103RE / STM32F103RC) board pin assignments
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Creality v4.2.10 only supports 1 hotend / E stepper."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "Creality V4.2.10"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "3DPrintMill"
#endif

#define BOARD_NO_NATIVE_USB

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
#endif
#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA11
  #define IIC_EEPROM_SCL                    PA12
  #define MARLIN_EEPROM_SIZE               0x800  // 2K (24C16)
#else
  #define SDCARD_EEPROM_EMULATION                 // SD EEPROM until all EEPROM is BL24CXX
  #define MARLIN_EEPROM_SIZE               0x800  // 2K
#endif

//
// Servos
//
#define SERVO0_PIN                          PB0   // BLTouch OUT

//
// Limit Switches
//
#define X_STOP_PIN                          PA3
#define Y_STOP_PIN                          PA7
#define Z_STOP_PIN                          PA5

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA5   // BLTouch IN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA6   // "Pulled-high"
#endif

//
// Steppers
//
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PC2
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PB9
#endif
#define X_ENABLE_PIN                        PC3

#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB8
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PB7
#endif
#define Y_ENABLE_PIN                X_ENABLE_PIN

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB6
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB5
#endif
#define Z_ENABLE_PIN                X_ENABLE_PIN

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                       PB4
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                        PB3
#endif
#define E0_ENABLE_PIN               X_ENABLE_PIN

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC5   // TH1
#define TEMP_BED_PIN                        PC4   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA0   // HEATER1
#define HEATER_BED_PIN                      PA1   // HOT BED

#define FAN_PIN                             PA2   // FAN
#define FAN_SOFT_PWM_REQUIRED

//
// SD Card
//
#define SD_DETECT_PIN                       PC7
#define SDCARD_CONNECTION                ONBOARD
#define ONBOARD_SPI_DEVICE                     1
#define ONBOARD_SD_CS_PIN                   PA4   // SDSS
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE                          // This board's SD is only seen by the printer

#if ENABLED(CR10_STOCKDISPLAY)

  #if ENABLED(RET6_12864_LCD)

    /**
     *    RET6 12864 LCD
     *        ------
     *  PC6  |10  9 | PB2
     *  PB10 | 8  7 | PE8
     *  PB14   6  5 | PB13
     *  PB12 | 4  3 | PB15
     *   GND | 2  1 | 5V
     *        ------
     *         EXP1
     */
    #define EXP1_03_PIN                     PB15
    #define EXP1_04_PIN                     PB12
    #define EXP1_05_PIN                     PB13
    #define EXP1_06_PIN                     PB14
    #define EXP1_07_PIN                     PE8
    #define EXP1_08_PIN                     PB10
    #define EXP1_09_PIN                     PB2
    #define EXP1_10_PIN                     PC6

    #define BEEPER_PIN               EXP1_10_PIN

  #elif ENABLED(VET6_12864_LCD)

    /**
     *    VET6 12864 LCD
     *        ------
     *  ?    |10  9 | PC5
     *  PB10 | 8  7 | ?
     *  PA6    6  5 | PA5
     *  PA4  | 4  3 | PA7
     *   GND | 2  1 | 5V
     *        ------
     *         EXP1
     */
    #define EXP1_03_PIN                     PA7
    #define EXP1_04_PIN                     PA4
    #define EXP1_05_PIN                     PA5
    #define EXP1_06_PIN                     PA6
    #define EXP1_07_PIN                     -1
    #define EXP1_08_PIN                     PB10
    #define EXP1_09_PIN                     PC5
    #define EXP1_10_PIN                     -1

  #else
    #error "Define RET6_12864_LCD or VET6_12864_LCD to select pins for CR10_STOCKDISPLAY with the Creality V4 controller."
  #endif

  #define LCD_PINS_RS                EXP1_04_PIN
  #define LCD_PINS_ENABLE            EXP1_03_PIN
  #define LCD_PINS_D4                EXP1_05_PIN

  #define BTN_ENC                    EXP1_09_PIN
  #define BTN_EN1                    EXP1_08_PIN
  #define BTN_EN2                    EXP1_06_PIN

#elif HAS_DWIN_E3V2 || IS_DWIN_MARLINUI

  // RET6 DWIN ENCODER LCD
  #define BTN_ENC                           PB14
  #define BTN_EN1                           PB15
  #define BTN_EN2                           PB12

  //#define LCD_LED_PIN                     PB2
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                      PB13
  #endif

#elif ENABLED(DWIN_VET6_CREALITY_LCD)

  // VET6 DWIN ENCODER LCD
  #define BTN_ENC                           PA6
  #define BTN_EN1                           PA7
  #define BTN_EN2                           PA4

  #define BEEPER_PIN                        PA5

#endif
