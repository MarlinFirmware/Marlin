/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef TARGET_LPC1768
  #error "Oops!  Make sure you have the LPC1768 environment selected in your IDE."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "BIQU SKR V1.1"
#endif

//
// Limit Switches
//
#define X_MIN_PIN          P1_29
#define X_MAX_PIN          P1_28
#define Y_MIN_PIN          P1_27
#define Y_MAX_PIN          P1_26
#define Z_MIN_PIN          P1_25
#define Z_MAX_PIN          P1_24

//
// Steppers
//
#define X_STEP_PIN         P0_04
#define X_DIR_PIN          P0_05
#define X_ENABLE_PIN       P4_28

#define Y_STEP_PIN         P2_01
#define Y_DIR_PIN          P2_02
#define Y_ENABLE_PIN       P2_00

#define Z_STEP_PIN         P0_20
#define Z_DIR_PIN          P0_21
#define Z_ENABLE_PIN       P0_19

#define E0_STEP_PIN        P0_11
#define E0_DIR_PIN         P2_13
#define E0_ENABLE_PIN      P2_12

#define E1_STEP_PIN        P0_01
#define E1_DIR_PIN         P0_00
#define E1_ENABLE_PIN      P0_10

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_BED_PIN       0   // A0 (T0) - (67) - TEMP_BED_PIN
#define TEMP_0_PIN         1   // A1 (T1) - (68) - TEMP_0_PIN
#define TEMP_1_PIN         2   // A2 (T2) - (69) - TEMP_1_PIN

//
// Heaters / Fans
//
#define HEATER_0_PIN       P2_07
#define HEATER_1_PIN       P2_04
#define FAN_PIN            P2_03
#define HEATER_BED_PIN     P2_05

//
// Misc. Functions
//
#define SDSS               P1_23   // (53)

/**
 * LCD / Controller
 *
 * All controllers can use J3 and J5 on the Re-ARM board.  Custom cabling will be required.
 */

/**
 * Smart LCD adapter
 *
 * The Smart LCD adapter can be used for the two 10 pin LCD controllers such as
 * REPRAP_DISCOUNT_SMART_CONTROLLER.  It can't be used for controllers that use
 * DOGLCD_A0, DOGLCD_CS, LCD_PINS_D5, LCD_PINS_D6 or LCD_PINS_D7. A custom cable
 * is needed to pick up 5V for the EXP1 connection.
 *
 * SD card on the LCD uses the same SPI signals as the LCD. This results in garbage/lines
 * on the LCD display during accesses of the SD card. The menus/code has been arranged so
 * that the garbage/lines are erased immediately after the SD card accesses are completed.
 */

#if ENABLED(ULTRA_LCD)

  #if ENABLED(CR10_STOCKDISPLAY)

    // Re-Arm can support Creality stock display without SD card reader and single cable on EXP3.
    // Re-Arm J3 pins 1 (p1.31) & 2 (P3.26) are not used. Stock cable will need to have one
    // 10-pin IDC connector trimmed or replaced with a 12-pin IDC connector to fit J3.
    // Requires REVERSE_ENCODER_DIRECTION in Configuration.h

    #define BEEPER_PIN     P2_11   // J3-3 & AUX-4

    #define BTN_EN1        P0_16   // J3-7 & AUX-4
    #define BTN_EN2        P1_23   // J3-5 & AUX-4
    #define BTN_ENC        P3_25   // J3-4 & AUX-4

    #define LCD_PINS_RS    P0_15   // J3-9 & AUX-4 (CS)
    #define LCD_PINS_ENABLE P0_18  // J3-10 & AUX-3 (SID, MOSI)
    #define LCD_PINS_D4    P2_06   // J3-8 & AUX-3 (SCK, CLK)

  #else

    #define BEEPER_PIN     P1_30   // (37) not 5V tolerant

    #define BTN_EN1        P3_26   // (31) J3-2 & AUX-4
    #define BTN_EN2        P3_25   // (33) J3-4 & AUX-4
    #define BTN_ENC        P2_11   // (35) J3-3 & AUX-4

    #define SD_DETECT_PIN  P1_31   // (49) not 5V tolerant   J3-1 & AUX-3
    #define KILL_PIN       P1_22   // (41) J5-4 & AUX-4
    #define LCD_PINS_RS    P0_16   // (16) J3-7 & AUX-4
    #define LCD_SDSS       P0_16   // (16) J3-7 & AUX-4

    #if ENABLED(REPRAPWORLD_KEYPAD)
      #define SHIFT_OUT    P0_18   // (51)  (MOSI) J3-10 & AUX-3
      #define SHIFT_CLK    P0_15   // (52)  (SCK)  J3-9 & AUX-3
      #define SHIFT_LD     P1_31   // (49)  not 5V tolerant   J3-1 & AUX-3
    #elif DISABLED(NEWPANEL)
      //#define SHIFT_CLK  P3_26   // (31)  J3-2 & AUX-4
      //#define SHIFT_LD   P3_25   // (33)  J3-4 & AUX-4
      //#define SHIFT_OUT  P2_11   // (35)  J3-3 & AUX-4
      //#define SHIFT_EN   P1_22   // (41)  J5-4 & AUX-4
    #endif

    #if ENABLED(VIKI2) || ENABLED(miniVIKI)
      // #define LCD_SCREEN_ROT_180

      #define BTN_EN1      P3_26   // (31) J3-2 & AUX-4
      #define BTN_EN2      P3_25   // (33) J3-4 & AUX-4
      #define BTN_ENC      P2_11   // (35) J3-3 & AUX-4

      #define SD_DETECT_PIN P1_31  // (49) not 5V tolerant   J3-1 & AUX-3
      #define KILL_PIN     P1_22   // (41) J5-4 & AUX-4

      #define DOGLCD_CS    P0_16   // (16)
      #define DOGLCD_A0    P2_06   // (59) J3-8 & AUX-2
      #define DOGLCD_SCK   SCK_PIN
      #define DOGLCD_MOSI  MOSI_PIN

      #define STAT_LED_BLUE_PIN   P0_26   // (63)  may change if cable changes
      #define STAT_LED_RED_PIN    P1_21   // ( 6)  may change if cable changes
    #else
      #define DOGLCD_CS    P0_26   // (63) J5-3 & AUX-2
      #define DOGLCD_A0    P2_06   // (59) J3-8 & AUX-2
      #define LCD_BACKLIGHT_PIN P0_16 // (16) J3-7 & AUX-4 - only used on DOGLCD controllers
      #define LCD_PINS_ENABLE P0_18   // (51) (MOSI) J3-10 & AUX-3
      #define LCD_PINS_D4  P0_15   // (52) (SCK)  J3-9 & AUX-3
      #if ENABLED(ULTIPANEL)
        #define LCD_PINS_D5 P1_17  // (71) ENET_MDIO
        #define LCD_PINS_D6 P1_14  // (73) ENET_RX_ER
        #define LCD_PINS_D7 P1_10  // (75) ENET_RXD1
      #endif
    #endif

    //#define MISO_PIN      P0_17   // (50)  system defined J3-10 & AUX-3
    //#define MOSI_PIN      P0_18   // (51)  system defined J3-10 & AUX-3
    //#define SCK_PIN       P0_15   // (52)  system defined J3-9 & AUX-3
    //#define SS_PIN        P1_23   // (53)  system defined J3-5 & AUX-3 - sometimes called SDSS

    #if ENABLED(MINIPANEL)
      // GLCD features
      //#define LCD_CONTRAST   190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
    #endif

  #endif

#endif // ULTRA_LCD
