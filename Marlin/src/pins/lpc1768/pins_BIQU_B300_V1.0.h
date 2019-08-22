/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * BIQU BQ111-A4
 *
 * Applies to the following boards:
 *
 *  BOARD_BIQU_BQ111_A4 (Hotend, Fan, Bed)
 *
 */

#ifndef TARGET_LPC1768
  #error "Oops!  Make sure you have the LPC1768 environment selected in your IDE."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BIQU Thunder B300 V1.0"
#endif

//
// Limit Switches
//
#define X_MIN_PIN          P1_24   // 10k pullup to 3.3V, 1K series
#define X_MAX_PIN          P1_25   // 10k pullup to 3.3V, 1K series
#define Y_MIN_PIN          P1_26   // 10k pullup to 3.3V, 1K series
#define Y_MAX_PIN          P1_27   // 10k pullup to 3.3V, 1K series
#define Z_MIN_PIN          P1_28   // 10k pullup to 3.3V, 1K series
#define Z_MAX_PIN          P1_29   // 10k pullup to 3.3V, 1K series

//
// Steppers
//
#define X_STEP_PIN         P2_00
#define X_DIR_PIN          P0_05
#define X_ENABLE_PIN       P0_04
#ifndef X_CS_PIN
  #define X_CS_PIN         P1_15   // ETH
#endif

#define Y_STEP_PIN         P2_01
#define Y_DIR_PIN          P0_11
#define Y_ENABLE_PIN       P0_10
#ifndef Y_CS_PIN
  #define Y_CS_PIN         P1_14   // ETH
#endif

#define Z_STEP_PIN         P2_02
#define Z_DIR_PIN          P0_20
#define Z_ENABLE_PIN       P0_19
#ifndef Z_CS_PIN
  #define Z_CS_PIN         P1_16   // ETH
#endif

#define E0_STEP_PIN        P2_03
#define E0_DIR_PIN         P0_22
#define E0_ENABLE_PIN      P0_21
#ifndef E0_CS_PIN
  #define E0_CS_PIN        P1_17   // ETH
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    P0_18   // ETH
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    P0_17   // ETH
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     P0_15   // ETH
  #endif
#endif

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN          1   // A0 (T0)
#define TEMP_BED_PIN        0   // A1 (T1)

//
// Heaters / Fans
//
#define HEATER_0_PIN       P2_07
#define HEATER_BED_PIN     P2_05
#ifndef FAN_PIN
  #define FAN_PIN          P2_04
#endif

//
// Unused
//
//#define PIN_P2_10        P2_10   // IBOOT-1
//#define PIN_P0_27        P0_27   // Onboard SD Detect

/**
 * LCD / Controller
 *
 * REPRAP_DISCOUNT_SMART_CONTROLLER is not supported due to the lack of LCD_PINS_D5,
 * LCD_PINS_D6 or LCD_PINS_D7 in the EXP1 connector.
 *
 * A remote SD card is not supported as the pins routed to EXP2 are the same as used
 * for the onboard SD card, and a chip select signal is not provided for the remote
 * SD card.
 */
#if HAS_SPI_LCD

  #define BEEPER_PIN       P1_31   // EXP1-1

  #define BTN_EN1          P3_26   // EXP2-3
  #define BTN_EN2          P3_25   // EXP2-5
  #define BTN_ENC          P1_30   // EXP1-2

  #define SD_DETECT_PIN    P0_27   // EXP2-7
  #define LCD_PINS_RS      P0_16   // EXP1-4
  #define LCD_PINS_ENABLE  P0_18   // (MOSI) EXP1-3
  #define LCD_PINS_D4      P0_15   // (SCK)  EXP1-5

  #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) && HAS_CHARACTER_LCD
    #error "REPRAP_DISCOUNT_SMART_CONTROLLER is not supported by the BIQU B300 v1.0"
  #endif

  #if ENABLED(SDSUPPORT)
    #error "SDSUPPORT is not supported by the BIQU B300 v1.0 when an LCD controller is used"
  #endif

#endif // HAS_SPI_LCD

/**
 * SD Card Reader
 *
 * Software SPI is used to interface with a stand-alone SD card reader connected to EXP1.
 * Hardware SPI can't be used because P0_17 (MISO) is not brought out on this board.
 */
#if ENABLED(SDSUPPORT)
  #define SCK_PIN          P0_15   // EXP1-5
  #define MISO_PIN         P0_16   // EXP1-4
  #define MOSI_PIN         P0_18   // EXP1-3
  #define SS_PIN           P1_30   // EXP1-2
  #define SDSS             SS_PIN
#endif

/**
 *  PWMS
 *
 *  There are 6 PWMS.  Each PWM can be assigned to one of two pins.
 *
 *  PWM1.1   P0_18   LCD_PINS_ENABLE
 *  PWM1.1   P2_0    X_STEP_PIN
 *  PWM1.2   P1_20   <none>
 *  PWM1.2   P2_1    Y_STEP_PIN
 *  PWM1.3   P1_21   <none>
 *  PWM1.3   P2_2    Z_STEP_PIN
 *  PWM1.4   P1_23   <none>
 *  PWM1.4   P2_3    E0_STEP_PIN
 *  PWM1.5   P1_24   X_MIN_PIN
 *  PWM1.5   P2_4    FAN_PIN
 *  PWM1.6   P1_26   Y_MIN_PIN
 *  PWM1.6   P2_5    HEATER_BED_PIN
 */
