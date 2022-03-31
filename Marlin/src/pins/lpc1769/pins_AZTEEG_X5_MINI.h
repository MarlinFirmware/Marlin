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
 * Azteeg X5 MINI pin assignments
 */
#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Azteeg X5 MINI"
#endif
#define BOARD_WEBSITE_URL "tiny.cc/x5_mini"

//
// LED
//
#define LED_PIN                            P1_18
#define LED2_PIN                           P1_20
#define LED3_PIN                           P1_19
#define LED4_PIN                           P1_21

//
// Servos
//
#define SERVO0_PIN                         P1_29

//
// Limit Switches
//
#define X_STOP_PIN                         P1_24
#define Y_STOP_PIN                         P1_26
#define Z_STOP_PIN                         P1_28

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                   P2_04
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                  P0_25_A2  // Analog Input (P0_25)
#endif

//
// Steppers
//
#define X_STEP_PIN                         P2_01
#define X_DIR_PIN                          P0_11
#define X_ENABLE_PIN                       P0_10

#define Y_STEP_PIN                         P2_02
#define Y_DIR_PIN                          P0_20
#define Y_ENABLE_PIN                       P0_19

#define Z_STEP_PIN                         P2_03
#define Z_DIR_PIN                          P0_22
#define Z_ENABLE_PIN                       P0_21

#define E0_STEP_PIN                        P2_00
#define E0_DIR_PIN                         P0_05
#define E0_ENABLE_PIN                      P0_04

//
// DIGIPOT slave addresses (7-bit unshifted)
//
#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A             0x2C
#endif
#ifndef DIGIPOT_I2C_ADDRESS_B
  #define DIGIPOT_I2C_ADDRESS_B             0x2E
#endif

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//
#define TEMP_BED_PIN                    P0_23_A0  // A0 (TH1)
#define TEMP_0_PIN                      P0_24_A1  // A1 (TH2)

//
// Heaters / Fans
//
#define HEATER_BED_PIN                     P2_07
#define HEATER_0_PIN                       P2_05
#ifndef FAN_PIN
  #define FAN_PIN                          P0_26
#endif
#define FAN1_PIN                           P1_25

//
// Display
//
#if HAS_WIRED_LCD

  #if ENABLED(CR10_STOCKDISPLAY)

    // Re-Arm can support Creality stock display without SD card reader and single cable on EXP3.
    // Re-Arm J3 pins 1 (p1.31) & 2 (P3.26) are not used. Stock cable will need to have one
    // 10-pin IDC connector trimmed or replaced with a 12-pin IDC connector to fit J3.
    // Requires REVERSE_ENCODER_DIRECTION in Configuration.h

    #define BEEPER_PIN                     P2_11  // J3-3 & AUX-4

    #define BTN_EN1                        P0_16  // J3-7 & AUX-4
    #define BTN_EN2                        P1_23  // J3-5 & AUX-4
    #define BTN_ENC                        P3_25  // J3-4 & AUX-4

    #define LCD_PINS_RS                    P0_15  // J3-9 & AUX-4 (CS)
    #define LCD_PINS_ENABLE                P0_18  // J3-10 & AUX-3 (SID, MOSI)
    #define LCD_PINS_D4                    P2_06  // J3-8 & AUX-3 (SCK, CLK)

  #else

    #define BTN_EN1                        P3_26  // (31) J3-2 & AUX-4
    #define BTN_EN2                        P3_25  // (33) J3-4 & AUX-4
    #define BTN_ENC                        P2_11  // (35) J3-3 & AUX-4

    #define SD_DETECT_PIN                  P1_31  // (49) not 5V tolerant   J3-1 & AUX-3
    #define KILL_PIN                       P1_22  // (41) J5-4 & AUX-4
    #define LCD_PINS_RS                    P0_16  // (16) J3-7 & AUX-4
    #define LCD_SDSS                       P0_16  // (16) J3-7 & AUX-4
    #define LCD_BACKLIGHT_PIN              P0_16  // (16) J3-7 & AUX-4 - only used on DOGLCD controllers
    #define LCD_PINS_ENABLE                P0_18  // (51) (MOSI) J3-10 & AUX-3
    #define LCD_PINS_D4                    P0_15  // (52) (SCK)  J3-9 & AUX-3

    #define DOGLCD_A0                      P2_06  // (59) J3-8 & AUX-2

    #if IS_RRW_KEYPAD
      #define SHIFT_OUT_PIN                P0_18  // (51)  (MOSI) J3-10 & AUX-3
      #define SHIFT_CLK_PIN                P0_15  // (52)  (SCK)  J3-9 & AUX-3
      #define SHIFT_LD_PIN                 P1_31  // (49)  not 5V tolerant   J3-1 & AUX-3
    #elif !IS_NEWPANEL
      //#define SHIFT_OUT_PIN              P2_11  // (35)  J3-3 & AUX-4
      //#define SHIFT_CLK_PIN              P3_26  // (31)  J3-2 & AUX-4
      //#define SHIFT_LD_PIN               P3_25  // (33)  J3-4 & AUX-4
      //#define SHIFT_EN_PIN               P1_22  // (41)  J5-4 & AUX-4
    #endif

    #if ANY(VIKI2, miniVIKI)
      //#define LCD_SCREEN_ROT_180

      #define BEEPER_PIN                   P1_30  // (37) may change if cable changes
      #define DOGLCD_CS                    P0_26  // (63) J5-3 & AUX-2
      #define DOGLCD_SCK              SD_SCK_PIN
      #define DOGLCD_MOSI            SD_MOSI_PIN

      #define STAT_LED_BLUE_PIN            P0_26  // (63)  may change if cable changes
      #define STAT_LED_RED_PIN             P1_21  // ( 6)  may change if cable changes
    #else
      #if IS_ULTIPANEL
        #define LCD_PINS_D5                P1_17  // (71) ENET_MDIO
        #define LCD_PINS_D6                P1_14  // (73) ENET_RX_ER
        #define LCD_PINS_D7                P1_10  // (75) ENET_RXD1
      #endif
      #define BEEPER_PIN                   P1_30  // (37) not 5V tolerant
      #define DOGLCD_CS                    P0_16  // (16)

      #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
        #define BTN_ENC_EN           LCD_PINS_D7  // Detect the presence of the encoder
      #endif

    #endif

    #if ENABLED(MINIPANEL)
      // GLCD features
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
    #endif

  #endif

#endif // HAS_WIRED_LCD

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(LCD)
  #define SD_SCK_PIN                       P0_15
  #define SD_MISO_PIN                      P0_17
  #define SD_MOSI_PIN                      P0_18
  #define SD_SS_PIN                        P1_23
#elif SD_CONNECTION_IS(ONBOARD)
  #undef SD_DETECT_PIN
  #define SD_SCK_PIN                       P0_07
  #define SD_MISO_PIN                      P0_08
  #define SD_MOSI_PIN                      P0_09
  #define ONBOARD_SD_CS_PIN                P0_06  // Chip select for "System" SD card
  #define SD_SS_PIN            ONBOARD_SD_CS_PIN
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif
