/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Creality MFL GD32 V4.2.2 (GD32F303RE) board pin assignments
 */

#include "env_validate.h"

#if !E_ERROR && (HAS_MULTI_HOTEND || E_STEPPERS > 1)
  #error "Creality V4.2.2 MFL only supports 1 hotend / E stepper."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "Creality V4.2.2 MFL"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender-3 MFL"
#endif

#define BOARD_NO_NATIVE_USB

//
// Release pin 19 (E0_STEP_PIN) from JTAG NRST role
//
#ifndef DISABLE_DEBUG
  #define DISABLE_DEBUG
#endif

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM  // EEPROM on I2C0
  #undef NO_EEPROM_SELECTED
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #ifndef IIC_EEPROM_SDA
    #define IIC_EEPROM_SDA                    11
  #endif
  #ifndef IIC_EEPROM_SCL
    #define IIC_EEPROM_SCL                    12
  #endif
  #define MARLIN_EEPROM_SIZE  0x800U  // 2K (24C16)
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                          16  // BLTouch/CRTouch/MicroProbeV1/V2 OUT
#endif

//
// Limit Switches
//
#ifndef X_STOP_PIN
  #define X_STOP_PIN                           5
#endif
#ifndef Y_STOP_PIN
  #define Y_STOP_PIN                           6
#endif
#ifndef Z_STOP_PIN
  #define Z_STOP_PIN                           7
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     17  // BLTouch/CRTouch/MicroProbeV1/V2 IN
#endif

//
// Probe enable
//
#if ENABLED(PROBE_ENABLE_DISABLE) && !defined(PROBE_ENABLE_PIN)
  #define PROBE_ENABLE_PIN            SERVO0_PIN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                       4  // "Pulled-high"
#endif

//
// Steppers
//
#ifndef X_STEP_PIN
  #define X_STEP_PIN                          34
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                           25
#endif
#define X_ENABLE_PIN                          35  // Shared

#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                          24
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                           23
#endif
#define Y_ENABLE_PIN                X_ENABLE_PIN

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                          22
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                           21
#endif
#define Z_ENABLE_PIN                X_ENABLE_PIN

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                         20
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                          19
#endif
#define E0_ENABLE_PIN               X_ENABLE_PIN

//
// Temperature Sensors
//
#define TEMP_0_PIN                            37  // TH1
#define TEMP_BED_PIN                          36  // TB1

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                         1  // HEATER1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                       2  // HOT BED
#endif
#ifndef FAN0_PIN
  #define FAN0_PIN                             0  // FAN
#endif
#define FAN_SOFT_PWM_REQUIRED

//
// SD Card
//
#define SD_DETECT_PIN                         39
#define SDCARD_CONNECTION ONBOARD
#define ONBOARD_SDIO
#define NO_SD_HOST_DRIVE  // This board's SD is only seen by the printer

/**
 * Debug port
 *       -----
 *       | 1 | VCC
 *       | 2 | PA13
 *       | 3 | PA14
 *       | 4 | GND
 *       -----
 */

#define DEBUG_02_PIN                          13
#define DEBUG_03_PIN                          14

#if ANY(RET6_12864_LCD, HAS_DWIN_E3V2, IS_DWIN_MARLINUI)

  /**
   * RET6 12864 LCD
   *        ------
   *  PC6  | 1  2 | PB2
   *  PB10 | 3  4 | PB11
   *  PB14   5  6 | PB13
   *  PB12 | 7  8 | PB15
   *   GND | 9 10 | 5V
   *        ------
   */
  #ifndef EXP3_01_PIN
    #define EXP3_01_PIN                       38
  #endif
  #ifndef EXP3_02_PIN
    #define EXP3_02_PIN                       18
  #endif
  #ifndef EXP3_03_PIN
    #define EXP3_03_PIN                       26
  #endif
  #ifndef EXP3_04_PIN
    #define EXP3_04_PIN                       27
  #endif
  #ifndef EXP3_05_PIN
    #define EXP3_05_PIN                       30
  #endif
  #ifndef EXP3_06_PIN
    #define EXP3_06_PIN                       29
  #endif
  #ifndef EXP3_07_PIN
    #define EXP3_07_PIN                       28
  #endif
  #ifndef EXP3_08_PIN
    #define EXP3_08_PIN                       31
  #endif

#elif ANY(VET6_12864_LCD, DWIN_VET6_CREALITY_LCD)

  /**
   * VET6 12864 LCD
   *        ------
   *  ?    | 1  2 | PC5
   *  PB10 | 3  4 | ?
   *  PA6    5  6 | PA5
   *  PA4  | 7  8 | PA7
   *   GND | 9 10 | 5V
   *        ------
   */
  #define EXP3_01_PIN                         -1
  #define EXP3_02_PIN                         37
  #define EXP3_03_PIN                         26
  #define EXP3_04_PIN                         -1
  #define EXP3_05_PIN                          6
  #define EXP3_06_PIN                          5
  #define EXP3_07_PIN                          4
  #define EXP3_08_PIN                          7

#elif ENABLED(CR10_STOCKDISPLAY)
  #error "Define RET6_12864_LCD or VET6_12864_LCD to select pins for the LCD with the Creality V4 controller."
#endif

#if ENABLED(CR10_STOCKDISPLAY)

  #define LCD_PINS_RS                EXP3_07_PIN
  #define LCD_PINS_EN                EXP3_08_PIN
  #define LCD_PINS_D4                EXP3_06_PIN

  #define BTN_ENC                    EXP3_02_PIN
  #define BTN_EN1                    EXP3_03_PIN
  #define BTN_EN2                    EXP3_05_PIN

  #ifndef BEEPER_PIN
    #define BEEPER_PIN               EXP3_01_PIN
  #endif

#elif ANY(HAS_DWIN_E3V2, IS_DWIN_MARLINUI, DWIN_VET6_CREALITY_LCD)

  #define BTN_ENC                    EXP3_05_PIN
  #define BTN_EN1                    EXP3_08_PIN
  #define BTN_EN2                    EXP3_07_PIN

  #ifndef BEEPER_PIN
    #define BEEPER_PIN               EXP3_06_PIN
  #endif

#endif

// Pins for documentation and sanity checks only.
// Changing these will not change the pin they are on.

// Hardware UART pins
#define UART0_TX_PIN                           9  // default uses CH340 RX
#define UART0_RX_PIN                          10  // default uses CH340 TX
#define UART1_TX_PIN                           2  // default uses HEATER_BED_PIN
#define UART1_RX_PIN                           3  // not connected
#define UART2_TX_PIN                          26  // default uses LCD connector
#define UART2_RX_PIN                          27  // default uses LCD connector
#define UART3_TX_PIN                          42  // default uses sdcard SDIO_D2
#define UART3_RX_PIN                          43  // default uses sdcard SDIO_D3
#define UART4_TX_PIN                          44  // default uses sdcard SDIO_CK
#define UART4_RX_PIN                          50  // default uses sdcard SDIO_CMD
