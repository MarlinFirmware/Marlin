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

#define BOARD_INFO_NAME "BIGTREE SKR 1.3"

//
// EEPROM
//
#define FLASH_EEPROM_EMULATION
//#define SDCARD_EEPROM_EMULATION

/**
 * Trinamic Stallguard pins
 */
#define X_DIAG_PIN           P1_29   // X-
#define Y_DIAG_PIN           P1_27   // Y-
#define Z_DIAG_PIN           P1_25   // Z-
#define E0_DIAG_PIN          P1_28   // X+
#define E1_DIAG_PIN          P1_26   // Y+

/**
 * Limit Switches
 */
#if X_STALL_SENSITIVITY
  #define X_STOP_PIN         X_DIAG_PIN
  #if X_HOME_DIR < 0
    #define X_MAX_PIN        P1_28   // X+
  #else
    #define X_MIN_PIN        P1_28   // X+
  #endif
#else
  #define X_MIN_PIN          P1_29   // X-
  #define X_MAX_PIN          P1_28   // X+
#endif

#if Y_STALL_SENSITIVITY
  #define Y_STOP_PIN         Y_DIAG_PIN
  #if Y_HOME_DIR < 0
    #define Y_MAX_PIN        P1_26   // Y+
  #else
    #define Y_MIN_PIN        P1_26   // Y+
  #endif
#else
  #define Y_MIN_PIN          P1_27   // Y-
  #define Y_MAX_PIN          P1_26   // Y+
#endif

#if Z_STALL_SENSITIVITY
  #define Z_STOP_PIN         Z_DIAG_PIN
  #if Z_HOME_DIR < 0
    #define Z_MAX_PIN        P1_24   // Z+
  #else
    #define Z_MIN_PIN        P1_24   // Z+
  #endif
#else
  #define Z_MIN_PIN          P1_25   // Z-
  #define Z_MAX_PIN          P1_24   // Z+
#endif

#define ONBOARD_ENDSTOPPULLUPS     // Board has built-in pullups

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN       P2_00
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  P1_24
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   P1_28
#endif

//
// Steppers
//
#define X_STEP_PIN         P2_02
#define X_DIR_PIN          P2_06
#define X_ENABLE_PIN       P2_01
#ifndef X_CS_PIN
  #define X_CS_PIN         P1_17
#endif

#define Y_STEP_PIN         P0_19
#define Y_DIR_PIN          P0_20
#define Y_ENABLE_PIN       P2_08
#ifndef Y_CS_PIN
  #define Y_CS_PIN         P1_15
#endif

#define Z_STEP_PIN         P0_22
#define Z_DIR_PIN          P2_11
#define Z_ENABLE_PIN       P0_21
#ifndef Z_CS_PIN
  #define Z_CS_PIN         P1_10
#endif

#define E0_STEP_PIN        P2_13
#define E0_DIR_PIN         P0_11
#define E0_ENABLE_PIN      P2_12
#ifndef E0_CS_PIN
  #define E0_CS_PIN        P1_08
#endif

#ifndef E1_CS_PIN
  #define E1_CS_PIN        P1_01
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    P4_28
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    P0_05
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     P0_04
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
  #define X_SERIAL_TX_PIN  P4_29
  #define X_SERIAL_RX_PIN  P1_17

  #define Y_SERIAL_TX_PIN  P1_16
  #define Y_SERIAL_RX_PIN  P1_15

  #define Z_SERIAL_TX_PIN  P1_14
  #define Z_SERIAL_RX_PIN  P1_10

  #define E0_SERIAL_TX_PIN P1_09
  #define E0_SERIAL_RX_PIN P1_08

  #define E1_SERIAL_TX_PIN P1_04
  #define E1_SERIAL_RX_PIN P1_01

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE 19200
#endif

/**
 *               _____                                              _____
 *           NC | 1 2 | GND                                     5V | 1 2 | GND
 *        RESET | 3 4 | 1.31 (SD_DETECT)             (LCD_D7) 1.23 | 3 4 | 1.22 (LCD_D6)
 *  (MOSI) 0.18 | 5 6   3.25 (BTN_EN2)               (LCD_D5) 1.21 | 5 6   1.20 (LCD_D4)
 * (SD_SS) 0.16 | 7 8 | 3.26 (BTN_EN1)               (LCD_RS) 1.19 | 7 8 | 1.18 (LCD_EN)
 *   (SCK) 0.15 | 9 10| 0.17 (MISO)                 (BTN_ENC) 0.28 | 9 10| 1.30 (BEEPER)
 *               -----                                              -----
 *               EXP2                                               EXP1
 */

#define EXPA1_03_PIN        P1_23
#define EXPA1_04_PIN        P1_22
#define EXPA1_05_PIN        P1_21
#define EXPA1_06_PIN        P1_20
#define EXPA1_07_PIN        P1_19
#define EXPA1_08_PIN        P1_18
#define EXPA1_09_PIN        P0_28
#define EXPA1_10_PIN        P1_30

#define EXPA2_03_PIN        -1
#define EXPA2_04_PIN        P1_31
#define EXPA2_05_PIN        P0_18
#define EXPA2_06_PIN        P3_25
#define EXPA2_07_PIN        P0_16
#define EXPA2_08_PIN        P3_26
#define EXPA2_09_PIN        P0_15
#define EXPA2_10_PIN        P0_17

#if HAS_SPI_LCD

  #if ENABLED(ANET_FULL_GRAPHICS_LCD)

    #error "CAUTION! ANET_FULL_GRAPHICS_LCD requires wiring modifications. See 'pins_BTT_SKR_V1_3.h' for details. Comment out this line to continue."

   /**
    * 1. Cut the tab off the LCD connector so it can be plugged into the "EXP1" connector the other way.
    * 2. Swap the LCD's +5V (Pin2) and GND (Pin1) wires. (This is the critical part!)
    * 3. Rewire the CLK Signal (LCD Pin9) to LCD Pin7. (LCD Pin9 remains open because this pin is open drain.)
    * 4. A wire is needed to connect the Reset switch at J3 (LCD Pin7) to EXP2 (Pin3) on the board.
    *
    * !!! If you are unsure, ask for help! Your motherboard may be damaged in some circumstances !!!
    *
    * The ANET_FULL_GRAPHICS_LCD connector plug:
    *
    *                  BEFORE                          AFTER
    *                  _____                           _____
    *           GND 1 | 1 2 |  2 5V              5V 1 | 1 2 |  2 GND
    *            CS 3 | 3 4 |  4 BTN_EN2         CS 3 | 3 4 |  4 BTN_EN2
    *           SID 5 | 5 6    6 BTN_EN1        SID 5 | 5 6    6 BTN_EN1
    *          open 7 | 7 8 |  8 BTN_ENC        CLK 7 | 7 8 |  8 BTN_ENC
    *           CLK 9 | 9 10| 10 Beeper        open 9 | 9 10| 10 Beeper
    *                  -----                           -----
    *                   LCD                             LCD
    */

    #define LCD_PINS_RS    EXPA1_03_PIN

    #define BTN_EN1        EXPA1_06_PIN
    #define BTN_EN2        EXPA1_04_PIN
    #define BTN_ENC        EXPA1_08_PIN

    #define LCD_PINS_ENABLE EXPA1_05_PIN
    #define LCD_PINS_D4    EXPA1_07_PIN

  #elif ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS    EXPA1_04_PIN

    #define BTN_EN1        EXPA1_08_PIN
    #define BTN_EN2        EXPA1_06_PIN
    #define BTN_ENC        EXPA1_09_PIN   // (58) open-drain

    #define LCD_PINS_ENABLE EXPA1_03_PIN
    #define LCD_PINS_D4    EXPA1_05_PIN

  #else // !CR10_STOCKDISPLAY

    #define LCD_PINS_RS    EXPA1_07_PIN

    #define BTN_EN1        EXPA2_08_PIN   // (31) J3-2 & AUX-4
    #define BTN_EN2        EXPA2_06_PIN   // (33) J3-4 & AUX-4
    #define BTN_ENC        EXPA1_09_PIN   // (58) open-drain

    #define LCD_PINS_ENABLE EXPA1_08_PIN
    #define LCD_PINS_D4    EXPA1_06_PIN

    #define LCD_SDSS       EXPA2_07_PIN   // (16) J3-7 & AUX-4
    #define SD_DETECT_PIN  EXPA2_04_PIN   // (49) (NOT 5V tolerant)

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS    EXPA1_08_PIN
      #define DOGLCD_A0    EXPA1_07_PIN
      #define DOGLCD_SCK   EXPA2_09_PIN
      #define DOGLCD_MOSI  EXPA2_05_PIN

      #define LCD_BACKLIGHT_PIN -1

      #define FORCE_SOFT_SPI      // Use this if default of hardware SPI causes display problems
                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

      #define LCD_RESET_PIN EXPA1_06_PIN   // Must be high or open for LCD to operate normally.

      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN EXPA1_05_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN EXPA1_04_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN EXPA1_03_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN    EXPA1_05_PIN
      #endif

    #else // !FYSETC_MINI_12864

      #if ENABLED(MKS_MINI_12864)

        #define DOGLCD_CS    EXPA1_05_PIN
        #define DOGLCD_A0    EXPA1_04_PIN
        #define DOGLCD_SCK   EXPA2_09_PIN
        #define DOGLCD_MOSI  EXPA2_05_PIN

      #elif ENABLED(ENDER2_STOCKDISPLAY)

        /**
         * Creality Ender-2 display pinout
         *                   _____
         *               5V | 1 2 | GND
         *     (MOSI) P1_23 | 3 4 | P1_22 (LCD_CS)
         *   (LCD_A0) P1_21 | 5 6   P1_20 (BTN_EN2)
         *      RESET P1_19 | 7 8 | P1_18 (BTN_EN1)
         *  (BTN_ENC) P0_28 | 9 10| P1_30 (SCK)
         *                   -----
         *                    EXP1
         */

        #define BTN_EN1      EXPA1_08_PIN
        #define BTN_EN2      EXPA1_06_PIN
        #define BTN_ENC      EXPA1_09_PIN
        #define DOGLCD_CS    EXPA1_04_PIN
        #define DOGLCD_A0    EXPA1_05_PIN
        #define DOGLCD_SCK   EXPA1_10_PIN
        #define DOGLCD_MOSI  EXPA1_03_PIN
        #define FORCE_SOFT_SPI
        #define LCD_BACKLIGHT_PIN -1
      #endif

      #if ENABLED(ULTIPANEL)
        #define LCD_PINS_D5 EXPA1_05_PIN
        #define LCD_PINS_D6 EXPA1_04_PIN
        #define LCD_PINS_D7 EXPA1_03_PIN
      #endif

    #endif // !FYSETC_MINI_12864

  #endif // !CR10_STOCKDISPLAY

#endif // HAS_SPI_LCD

//
// SD Support
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION LCD
#endif

#if SD_CONNECTION_IS(LCD)
  #define SS_PIN           EXPA2_07_PIN
#endif

/**
 * Special pins
 *   P1_30  (37) (NOT 5V tolerant)
 *   P1_31  (49) (NOT 5V tolerant)
 *   P0_27  (57) (Open collector)
 *   P0_28  (58) (Open collector)
 */

// Include common SKR pins
#include "pins_BTT_SKR_common.h"
