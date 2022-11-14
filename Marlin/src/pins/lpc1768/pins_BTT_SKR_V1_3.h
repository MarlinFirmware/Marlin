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
 * BigTreeTech SKR 1.3 pin assignments
 */

#define BOARD_INFO_NAME "BTT SKR V1.3"

#define LPC1768_IS_SKRV1_3
#define USES_DIAG_JUMPERS

//
// Trinamic Stallguard pins
//
#define X_DIAG_PIN                         P1_29  // X-
#define Y_DIAG_PIN                         P1_27  // Y-
#define Z_DIAG_PIN                         P1_25  // Z-
#define E0_DIAG_PIN                        P1_28  // X+
#define E1_DIAG_PIN                        P1_26  // Y+

//
// Limit Switches
//
#ifdef X_STALL_SENSITIVITY
  #define X_STOP_PIN                  X_DIAG_PIN
  #if X_HOME_TO_MIN
    #define X_MAX_PIN                      P1_28  // X+
  #else
    #define X_MIN_PIN                      P1_28  // X+
  #endif
#else
  #define X_MIN_PIN                        P1_29  // X-
  #define X_MAX_PIN                        P1_28  // X+
#endif

#ifdef Y_STALL_SENSITIVITY
  #define Y_STOP_PIN                  Y_DIAG_PIN
  #if Y_HOME_TO_MIN
    #define Y_MAX_PIN                      P1_26  // Y+
  #else
    #define Y_MIN_PIN                      P1_26  // Y+
  #endif
#else
  #define Y_MIN_PIN                        P1_27  // Y-
  #define Y_MAX_PIN                        P1_26  // Y+
#endif

#ifdef Z_STALL_SENSITIVITY
  #define Z_STOP_PIN                  Z_DIAG_PIN
  #if Z_HOME_TO_MIN
    #define Z_MAX_PIN                      P1_24  // Z+
  #else
    #define Z_MIN_PIN                      P1_24  // Z+
  #endif
#else
  #define Z_MIN_PIN                        P1_25  // Z-
  #define Z_MAX_PIN                        P1_24  // Z+
#endif

#define ONBOARD_ENDSTOPPULLUPS                    // Board has built-in pullups

//
// Servos
//
#ifndef SERVO0_PIN
  #define SERVO0_PIN                       P2_00
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  P1_24
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                   P1_28
#endif

//
// Steppers
//
#define X_STEP_PIN                         P2_02
#define X_DIR_PIN                          P2_06
#define X_ENABLE_PIN                       P2_01
#ifndef X_CS_PIN
  #define X_CS_PIN                         P1_17
#endif

#define Y_STEP_PIN                         P0_19
#define Y_DIR_PIN                          P0_20
#define Y_ENABLE_PIN                       P2_08
#ifndef Y_CS_PIN
  #define Y_CS_PIN                         P1_15
#endif

#define Z_STEP_PIN                         P0_22
#define Z_DIR_PIN                          P2_11
#define Z_ENABLE_PIN                       P0_21
#ifndef Z_CS_PIN
  #define Z_CS_PIN                         P1_10
#endif

#define E0_STEP_PIN                        P2_13
#define E0_DIR_PIN                         P0_11
#define E0_ENABLE_PIN                      P2_12
#ifndef E0_CS_PIN
  #define E0_CS_PIN                        P1_08
#endif

#ifndef E1_CS_PIN
  #define E1_CS_PIN                        P1_01
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                    P4_28
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
  //#define X_HARDWARE_SERIAL  Serial1
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

  #define X_SERIAL_TX_PIN                  P4_29
  #define X_SERIAL_RX_PIN                  P1_17

  #define Y_SERIAL_TX_PIN                  P1_16
  #define Y_SERIAL_RX_PIN                  P1_15

  #define Z_SERIAL_TX_PIN                  P1_14
  #define Z_SERIAL_RX_PIN                  P1_10

  #define E0_SERIAL_TX_PIN                 P1_09
  #define E0_SERIAL_RX_PIN                 P1_08

  #define E1_SERIAL_TX_PIN                 P1_04
  #define E1_SERIAL_RX_PIN                 P1_01

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

/**
 *                 ------                                     ------
 *  (BEEPER) 1.30 | 1  2 | 0.28 (BTN_ENC)        (MISO) 0.17 | 1  2 | 0.15 (SCK)
 *  (LCD_EN) 1.18 | 3  4 | 1.19  (LCD_RS)     (BTN_EN1) 3.26 | 3  4 | 0.16 (SD_SS)
 *  (LCD_D4) 1.20   5  6 | 1.21  (LCD_D5)     (BTN_EN2) 3.25   5  6 | 0.18 (MOSI)
 *  (LCD_D6) 1.22 | 7  8 | 1.23  (LCD_D7)   (SD_DETECT) 1.31 | 7  8 | RESET
 *            GND | 9 10 | 5V                            GND | 9 10 | --
 *                 ------                                     ------
 *                  EXP1                                       EXP2
 */
#define EXP1_01_PIN                        P1_30
#define EXP1_02_PIN                        P0_28
#define EXP1_03_PIN                        P1_18
#define EXP1_04_PIN                        P1_19
#define EXP1_05_PIN                        P1_20
#define EXP1_06_PIN                        P1_21
#define EXP1_07_PIN                        P1_22
#define EXP1_08_PIN                        P1_23

#define EXP2_01_PIN                        P0_17
#define EXP2_02_PIN                        P0_15
#define EXP2_03_PIN                        P3_26
#define EXP2_04_PIN                        P0_16
#define EXP2_05_PIN                        P3_25
#define EXP2_06_PIN                        P0_18
#define EXP2_07_PIN                        P1_31
#define EXP2_08_PIN                        -1

#if HAS_WIRED_LCD
  #if ENABLED(ANET_FULL_GRAPHICS_LCD_ALT_WIRING)
    #error "ANET_FULL_GRAPHICS_LCD_ALT_WIRING only applies to the ANET 1.0 board."

  #elif ENABLED(ANET_FULL_GRAPHICS_LCD)

    #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
      #error "CAUTION! ANET_FULL_GRAPHICS_LCD requires wiring modifications. See 'pins_BTT_SKR_V1_3.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
    #endif

   /**
    * 1. Cut the tab off the LCD connector so it can be plugged into the "EXP1" connector the other way.
    * 2. Swap the LCD's +5V (Pin2) and GND (Pin1) wires. (This is the critical part!)
    * 3. Rewire the CLK Signal (LCD Pin9) to LCD Pin7. (LCD Pin9 remains open because it is open drain.)
    * 4. A wire is needed to connect the Reset switch at J3 (LCD Pin7) to EXP2 (Pin3) on the board.
    *
    * !!! If you are unsure, ask for help! Your motherboard may be damaged in some circumstances !!!
    *
    * The ANET_FULL_GRAPHICS_LCD connector plug:
    *
    *                  BEFORE                          AFTER
    *                  ------                          ------
    *           (CLK) | 1  2 | (BEEPER)      (BEEPER) |10  9 | --
    *              -- | 3  4 | (BTN_ENC)    (BTN_ENC) | 8  7 | (CLK)
    *           (SID)   5  6 | (BTN_EN1)    (BTN_EN1)   6  5 | (SID)
    *            (CS) | 7  8 | (BTN_EN2)    (BTN_EN2) | 4  3 | (CS)
    *             GND | 9 10 | 5V                 GND | 2  1 | 5V
    *                  ------                          ------
    *                   LCD                             LCD
    */

    #define LCD_PINS_RS              EXP1_08_PIN

    #define BTN_EN1                  EXP1_05_PIN
    #define BTN_EN2                  EXP1_07_PIN
    #define BTN_ENC                  EXP1_03_PIN

    #define LCD_PINS_ENABLE          EXP1_06_PIN
    #define LCD_PINS_D4              EXP1_04_PIN

  #elif ENABLED(WYH_L12864)

    #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
      #error "CAUTION! WYH_L12864 requires wiring modifications. See 'pins_BTT_SKR_V1_3.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
    #endif

    /**
     * 1. Cut the tab off the LCD connector so it can be plugged into the "EXP1" connector the other way.
     * 2. Swap the LCD's +5V (Pin2) and GND (Pin1) wires. (This is the critical part!)
     * 3. Swap the LCD's MOSI (Pin9) and empty (Pin10) wires because Pin9 is open drain.
     *
     * !!! If you are unsure, ask for help! Your motherboard may be damaged in some circumstances !!!
     *
     * The WYH_L12864 connector plug:
     *
     *                  BEFORE                      AFTER
     *                  ______                     ______
     *                 | 1  2 | (MOSI)     (MOSI) |10  9 | --
     *       (BTN_ENC) | 3  4 | (SCK)   (BTN_ENC) | 8  7 | (SCK)
     *       (BTN_EN1)   5  6 | (SID)   (BTN_EN1)   6  5 | (SID)
     *       (BTN_EN2) | 7  8 | (CS)    (BTN_EN2) | 4  3 | (CS)
     *              5V | 9 10 | GND           GND | 2  1 | 5V
     *                  ------                     ------
     *                   LCD                        LCD
     */
    #define BTN_EN1                  EXP1_05_PIN
    #define BTN_EN2                  EXP1_07_PIN
    #define BTN_ENC                  EXP1_03_PIN
    #define DOGLCD_CS                EXP1_08_PIN
    #define DOGLCD_A0                EXP1_06_PIN
    #define DOGLCD_SCK               EXP1_04_PIN
    #define DOGLCD_MOSI              EXP1_01_PIN
    #define LCD_BACKLIGHT_PIN            -1

  #elif ENABLED(CR10_STOCKDISPLAY)

    #define LCD_PINS_RS              EXP1_07_PIN

    #define BTN_EN1                  EXP1_03_PIN
    #define BTN_EN2                  EXP1_05_PIN
    #define BTN_ENC                  EXP1_02_PIN  // (58) open-drain

    #define LCD_PINS_ENABLE          EXP1_08_PIN
    #define LCD_PINS_D4              EXP1_06_PIN

  #elif HAS_ADC_BUTTONS

    #error "ADC BUTTONS do not work unmodified on SKR 1.3, The ADC ports cannot take more than 3.3v."

  #elif HAS_SPI_TFT                               // Config for Classic UI (emulated DOGM) and Color UI

    #define TFT_A0_PIN               EXP1_08_PIN
    #define TFT_DC_PIN               EXP1_08_PIN
    #define TFT_CS_PIN               EXP1_07_PIN
    #define TFT_RESET_PIN            EXP1_04_PIN
    #define TFT_BACKLIGHT_PIN        EXP1_03_PIN

    //#define TFT_RST_PIN            EXP2_07_PIN
    #define TFT_MOSI_PIN             EXP2_06_PIN
    #define TFT_SCK_PIN              EXP2_02_PIN
    #define TFT_MISO_PIN             EXP2_01_PIN

    #define BTN_EN2                  EXP2_05_PIN
    #define BTN_EN1                  EXP2_03_PIN
    #define BTN_ENC                  EXP1_02_PIN

    #define TOUCH_BUTTONS_HW_SPI
    #define TOUCH_BUTTONS_HW_SPI_DEVICE        1

    #define TFT_BUFFER_SIZE                 2400

    #ifndef TFT_WIDTH
      #define TFT_WIDTH                      480
    #endif
    #ifndef TFT_HEIGHT
      #define TFT_HEIGHT                     320
    #endif

    #define LCD_READ_ID                     0xD3
    #define LCD_USE_DMA_SPI

    #if ENABLED(TFT_CLASSIC_UI)
      #ifndef TOUCH_CALIBRATION_X
        #define TOUCH_CALIBRATION_X       -11386
      #endif
      #ifndef TOUCH_CALIBRATION_Y
        #define TOUCH_CALIBRATION_Y         8684
      #endif
      #ifndef TOUCH_OFFSET_X
        #define TOUCH_OFFSET_X               689
      #endif
      #ifndef TOUCH_OFFSET_Y
        #define TOUCH_OFFSET_Y              -273
      #endif
    #elif ENABLED(TFT_COLOR_UI)
      #ifndef TOUCH_CALIBRATION_X
        #define TOUCH_CALIBRATION_X       -16741
      #endif
      #ifndef TOUCH_CALIBRATION_Y
        #define TOUCH_CALIBRATION_Y        11258
      #endif
      #ifndef TOUCH_OFFSET_X
        #define TOUCH_OFFSET_X              1024
      #endif
      #ifndef TOUCH_OFFSET_Y
        #define TOUCH_OFFSET_Y              -367
      #endif
      #define TFT_BUFFER_SIZE               2400
    #endif

  #elif IS_TFTGLCD_PANEL
    #if ENABLED(TFTGLCD_PANEL_SPI)
      #define TFTGLCD_CS             EXP2_03_PIN
    #endif

    #define SD_DETECT_PIN            EXP2_07_PIN

  #else                                           // !CR10_STOCKDISPLAY

    #define LCD_PINS_RS              EXP1_04_PIN

    #define BTN_EN1                  EXP2_03_PIN  // (31) J3-2 & AUX-4
    #define BTN_EN2                  EXP2_05_PIN  // (33) J3-4 & AUX-4
    #define BTN_ENC                  EXP1_02_PIN  // (58) open-drain

    #define LCD_PINS_ENABLE          EXP1_03_PIN
    #define LCD_PINS_D4              EXP1_05_PIN

    #define LCD_SDSS                 EXP2_04_PIN  // (16) J3-7 & AUX-4
    #define SD_DETECT_PIN            EXP2_07_PIN  // (49) (NOT 5V tolerant)

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_CS              EXP1_03_PIN
      #define DOGLCD_A0              EXP1_04_PIN
      #define DOGLCD_SCK             EXP2_02_PIN
      #define DOGLCD_MOSI            EXP2_06_PIN

      #define LCD_BACKLIGHT_PIN            -1

      #define FORCE_SOFT_SPI                      // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

      #define LCD_RESET_PIN          EXP1_05_PIN  // Must be high or open for LCD to operate normally.

      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN      EXP1_06_PIN
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN      EXP1_07_PIN
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN      EXP1_08_PIN
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN         EXP1_06_PIN
      #endif

    #else                                         // !FYSETC_MINI_12864

      #if ENABLED(MKS_MINI_12864)

        #define DOGLCD_CS            EXP1_06_PIN
        #define DOGLCD_A0            EXP1_07_PIN
        #define DOGLCD_SCK           EXP2_02_PIN
        #define DOGLCD_MOSI          EXP2_06_PIN
        #define FORCE_SOFT_SPI

      #elif ENABLED(ENDER2_STOCKDISPLAY)

        /**
         * Creality Ender-2 display pinout
         *                   ------
         *      (SCK) P1_30 | 1  2 | P0_28 (BTN_ENC)
         *  (BTN_EN1) P1_18 | 3  4 | P1_19 (RESET)
         *  (BTN_EN2) P1_20   5  6 | P1_21 (LCD_A0)
         *   (LCD_CS) P1_22 | 7  8 | P1_23 (MOSI)
         *              GND | 9 10 | 5V
         *                   ------
         *                    EXP1
         */

        #define BTN_EN1              EXP1_03_PIN
        #define BTN_EN2              EXP1_05_PIN
        #define BTN_ENC              EXP1_02_PIN
        #define DOGLCD_CS            EXP1_07_PIN
        #define DOGLCD_A0            EXP1_06_PIN
        #define DOGLCD_SCK           EXP1_01_PIN
        #define DOGLCD_MOSI          EXP1_08_PIN
        #define FORCE_SOFT_SPI
        #define LCD_BACKLIGHT_PIN          -1
      #endif

      #if IS_ULTIPANEL
        #define LCD_PINS_D5          EXP1_06_PIN
        #define LCD_PINS_D6          EXP1_07_PIN
        #define LCD_PINS_D7          EXP1_08_PIN

        #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
          #define BTN_ENC_EN         LCD_PINS_D7  // Detect the presence of the encoder
        #endif

      #endif

    #endif // !FYSETC_MINI_12864

  #endif // !CR10_STOCKDISPLAY

#endif // HAS_WIRED_LCD

#if NEED_TOUCH_PINS
  #define TOUCH_CS_PIN               EXP1_05_PIN
  #define TOUCH_SCK_PIN              EXP2_02_PIN
  #define TOUCH_MOSI_PIN             EXP2_06_PIN
  #define TOUCH_MISO_PIN             EXP2_01_PIN
  #define TOUCH_INT_PIN              EXP1_06_PIN
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
