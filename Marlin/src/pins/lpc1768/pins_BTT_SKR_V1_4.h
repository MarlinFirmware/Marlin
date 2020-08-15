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

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BTT SKR V1.4"
#endif

//
// SD Connection
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION                  LCD
#endif

//
// Servos
//
#define SERVO0_PIN                         P2_00

//
// TMC StallGuard DIAG pins
//
#define X_DIAG_PIN                         P1_29  // X-STOP
#define Y_DIAG_PIN                         P1_28  // Y-STOP
#define Z_DIAG_PIN                         P1_27  // Z-STOP
#define E0_DIAG_PIN                        P1_26  // E0DET
#define E1_DIAG_PIN                        P1_25  // E1DET

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_DIR < 0
    #define X_MAX_PIN                      P1_26  // E0DET
  #else
    #define X_MIN_PIN                      P1_26  // E0DET
  #endif
#else
  #define X_STOP_PIN                       P1_29  // X-STOP
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_DIR < 0
    #define Y_MAX_PIN                      P1_25  // E1DET
  #else
    #define Y_MIN_PIN                      P1_25  // E1DET
  #endif
#else
  #define Y_STOP_PIN                       P1_28  // Y-STOP
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_DIR < 0
    #define Z_MAX_PIN                      P1_00  // PWRDET
  #else
    #define Z_MIN_PIN                      P1_00  // PWRDET
  #endif
#else
  #ifndef Z_STOP_PIN
    #define Z_STOP_PIN                     P1_27  // Z-STOP
  #endif
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  P0_10
#endif

//
// Filament Runout Sensor
//
#define FIL_RUNOUT_PIN                     P1_26  // E0DET
#define FIL_RUNOUT2_PIN                    P1_25  // E1DET

//
// Power Supply Control
//
#ifndef PS_ON_PIN
  #define PS_ON_PIN                        P1_00  // PWRDET
#endif

//
// Power Loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                   P1_00  // PWRDET
#endif

//
// Steppers
//
#define X_STEP_PIN                         P2_02
#define X_DIR_PIN                          P2_06
#define X_ENABLE_PIN                       P2_01
#ifndef X_CS_PIN
  #define X_CS_PIN                         P1_10
#endif

#define Y_STEP_PIN                         P0_19
#define Y_DIR_PIN                          P0_20
#define Y_ENABLE_PIN                       P2_08
#ifndef Y_CS_PIN
  #define Y_CS_PIN                         P1_09
#endif

#define Z_STEP_PIN                         P0_22
#define Z_DIR_PIN                          P2_11
#define Z_ENABLE_PIN                       P0_21
#ifndef Z_CS_PIN
  #define Z_CS_PIN                         P1_08
#endif

#define E0_STEP_PIN                        P2_13
#define E0_DIR_PIN                         P0_11
#define E0_ENABLE_PIN                      P2_12
#ifndef E0_CS_PIN
  #define E0_CS_PIN                        P1_04
#endif

#define E1_STEP_PIN                        P1_15
#define E1_DIR_PIN                         P1_14
#define E1_ENABLE_PIN                      P1_16
#ifndef E1_CS_PIN
  #define E1_CS_PIN                        P1_01
#endif

#define TEMP_1_PIN                      P0_23_A0  // A2 (T2) - (69) - TEMP_1_PIN
#define TEMP_BED_PIN                    P0_25_A2  // A0 (T0) - (67) - TEMP_BED_PIN

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                    P1_17
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                    P0_05
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                     P0_04
  #endif
#endif

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */
  //#define X_HARDWARE_SERIAL  Serial
  //#define X2_HARDWARE_SERIAL Serial1
  //#define Y_HARDWARE_SERIAL  Serial1
  //#define Y2_HARDWARE_SERIAL Serial1
  //#define Z_HARDWARE_SERIAL  Serial1
  //#define Z2_HARDWARE_SERIAL Serial1
  //#define E0_HARDWARE_SERIAL Serial1
  //#define E1_HARDWARE_SERIAL Serial1
  //#define E2_HARDWARE_SERIAL Serial1
  //#define E3_HARDWARE_SERIAL Serial1
  //#define E4_HARDWARE_SERIAL Serial1

  //
  // Software serial
  //
  #define X_SERIAL_TX_PIN                  P1_10
  #define X_SERIAL_RX_PIN                  P1_10

  #define Y_SERIAL_TX_PIN                  P1_09
  #define Y_SERIAL_RX_PIN                  P1_09

  #define Z_SERIAL_TX_PIN                  P1_08
  #define Z_SERIAL_RX_PIN                  P1_08

  #define E0_SERIAL_TX_PIN                 P1_04
  #define E0_SERIAL_RX_PIN                 P1_04

  #define E1_SERIAL_TX_PIN                 P1_01
  #define E1_SERIAL_RX_PIN                 P1_01

  #define Z2_SERIAL_TX_PIN                 P1_01
  #define Z2_SERIAL_RX_PIN                 P1_01

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

//
// SD Connection
//
#if SD_CONNECTION_IS(LCD)
  #define SS_PIN                           P0_16
#endif

/**
 *              _____                                             _____
 *          NC | · · | GND                                    5V | · · | GND
 *       RESET | · · | 1.31(SD_DETECT)             (LCD_D7) 1.23 | · · | 1.22 (LCD_D6)
 *  (MOSI)0.18 | · · | 3.25(BTN_EN2)               (LCD_D5) 1.21 | · · | 1.20 (LCD_D4)
 * (SD_SS)0.16 | · · | 3.26(BTN_EN1)               (LCD_RS) 1.19 | · · | 1.18 (LCD_EN)
 *   (SCK)0.15 | · · | 0.17(MISO)                 (BTN_ENC) 0.28 | · · | 1.30 (BEEPER)
 *              -----                                             -----
 *              EXP2                                              EXP1
 */
#if HAS_SPI_LCD
  #if ENABLED(ANET_FULL_GRAPHICS_LCD)

    #define LCD_PINS_RS                    P1_23

    #define BTN_EN1                        P1_20
    #define BTN_EN2                        P1_22
    #define BTN_ENC                        P1_18

    #define LCD_PINS_ENABLE                P1_21
    #define LCD_PINS_D4                    P1_19

  #elif ENABLED(CR10_STOCKDISPLAY)
    #define BTN_ENC                        P0_28  // (58) open-drain
    #define LCD_PINS_RS                    P1_22

    #define BTN_EN1                        P1_18
    #define BTN_EN2                        P1_20

    #define LCD_PINS_ENABLE                P1_23
    #define LCD_PINS_D4                    P1_21

  #elif ENABLED(ENDER2_STOCKDISPLAY)

    /** Creality Ender-2 display pinout
     *                   _____
     *               5V | 1 2 | GND
     *      (MOSI) 1.23 | 3 4 | 1.22 (LCD_RS)
     *    (LCD_A0) 1.21 | 5 6 | 1.20 (BTN_EN2)
     *       RESET 1.19 | 7 8 | 1.18 (BTN_EN1)
     *   (BTN_ENC) 0.28 | 9 10| 1.30  (SCK)
     *                   -----
     *                    EXP1
     */

    #define BTN_EN1                        P1_18
    #define BTN_EN2                        P1_20
    #define BTN_ENC                        P0_28

    #define DOGLCD_CS                      P1_22
    #define DOGLCD_A0                      P1_21
    #define DOGLCD_SCK                     P1_30
    #define DOGLCD_MOSI                    P1_23
    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN              -1

  #else

    #define BTN_ENC                        P0_28  // (58) open-drain
    #define LCD_PINS_RS                    P1_19

    #define BTN_EN1                        P3_26  // (31) J3-2 & AUX-4
    #define BTN_EN2                        P3_25  // (33) J3-4 & AUX-4

    #define LCD_PINS_ENABLE                P1_18
    #define LCD_PINS_D4                    P1_20

    #define LCD_SDSS                       P0_16  // (16) J3-7 & AUX-4

    #if SD_CONNECTION_IS(LCD)
      #define SD_DETECT_PIN                P1_31  // (49) (NOT 5V tolerant)
    #endif

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS                    P1_18
      #define DOGLCD_A0                    P1_19
      #define DOGLCD_SCK                   P0_15
      #define DOGLCD_MOSI                  P0_18

      #define LCD_BACKLIGHT_PIN            -1

      #define FORCE_SOFT_SPI                      // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

      #define LCD_RESET_PIN                P1_20  // Must be high or open for LCD to operate normally.

      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN            P1_21
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN            P1_22
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN            P1_23
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN               P1_21
      #endif

    #else                                         // !FYSETC_MINI_12864

      #if ENABLED(MKS_MINI_12864)
        #define DOGLCD_CS                  P1_21
        #define DOGLCD_A0                  P1_22
        #define DOGLCD_SCK                 P0_15
        #define DOGLCD_MOSI                P0_18
        #define FORCE_SOFT_SPI
      #endif

      #if ENABLED(ULTIPANEL)
        #define LCD_PINS_D5                P1_21
        #define LCD_PINS_D6                P1_22
        #define LCD_PINS_D7                P1_23
      #endif

    #endif // !FYSETC_MINI_12864

  #endif // HAS_GRAPHICAL_LCD

#endif // HAS_SPI_LCD

#if HAS_ADC_BUTTONS
  #error "ADC BUTTONS do not work unmodifed on SKR 1.4, The ADC ports cannot take more than 3.3v."
#endif

//
// Neopixel LED
//
#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                     P1_24
#endif

/**
 * Special pins
 *   P1_30  (37) (NOT 5V tolerant)
 *   P1_31  (49) (NOT 5V tolerant)
 *   P0_27  (57) (Open collector)
 *   P0_28  (58) (Open collector)
 */

//
// Include common SKR pins
//
#include "pins_BTT_SKR_common.h"
