/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Azteeg X5 MINI pin assignments
 */

#ifndef LPC1769
  #error "Oops! Make sure you have the LPC1769 environment selected in your IDE."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME        "Azteeg X5 MINI"
#endif
#define BOARD_WEBSITE_URL "http://www.panucatt.com/azteeg_X5_mini_reprap_3d_printer_controller_p/ax5mini.htm"

//
// LED
//
#define LED_PIN            P1_18

//
// Servos
//
#define SERVO0_PIN         P1_29

//
// Limit Switches
//
#define X_STOP_PIN         P1_24
#define Y_STOP_PIN         P1_26
#define Z_STOP_PIN         P1_28

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   P2_04
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN         2   // Analog Input (P0_25)
#endif

//
// Steppers
//
#define X_STEP_PIN         P2_01
#define X_DIR_PIN          P0_11
#define X_ENABLE_PIN       P0_10

#define Y_STEP_PIN         P2_02
#define Y_DIR_PIN          P0_20
#define Y_ENABLE_PIN       P0_19

#define Z_STEP_PIN         P2_03
#define Z_DIR_PIN          P0_22
#define Z_ENABLE_PIN       P0_21

#define E0_STEP_PIN        P2_00
#define E0_DIR_PIN         P0_05
#define E0_ENABLE_PIN      P0_04

//
// DIGIPOT slave addresses
//
#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A 0x2C   // unshifted slave address for first DIGIPOT
#endif

#ifndef DIGIPOT_I2C_ADDRESS_B
  #define DIGIPOT_I2C_ADDRESS_B 0x2E   // unshifted slave address for second DIGIPOT
#endif

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//
#define TEMP_BED_PIN        0   // A0 (TH1)
#define TEMP_0_PIN          1   // A1 (TH2)

//
// Heaters / Fans
//
#define HEATER_BED_PIN     P2_07
#define HEATER_0_PIN       P2_05
#ifndef FAN_PIN
  #define FAN_PIN          P0_26
#endif
#define FAN1_PIN           P1_25

//
// Display
//
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

    #define BTN_EN1        P3_26   // (31) J3-2 & AUX-4
    #define BTN_EN2        P3_25   // (33) J3-4 & AUX-4
    #define BTN_ENC        P2_11   // (35) J3-3 & AUX-4

    #define SD_DETECT_PIN  P1_31   // (49) not 5V tolerant   J3-1 & AUX-3
    #define KILL_PIN       P1_22   // (41) J5-4 & AUX-4
    #define LCD_PINS_RS    P0_16   // (16) J3-7 & AUX-4
    #define LCD_SDSS       P0_16   // (16) J3-7 & AUX-4
    #define LCD_BACKLIGHT_PIN P0_16 // (16) J3-7 & AUX-4 - only used on DOGLCD controllers
    #define LCD_PINS_ENABLE P0_18  // (51) (MOSI) J3-10 & AUX-3
    #define LCD_PINS_D4    P0_15   // (52) (SCK)  J3-9 & AUX-3

    #define DOGLCD_A0      P2_06   // (59) J3-8 & AUX-2

    #if ENABLED(REPRAPWORLD_KEYPAD)
      #define SHIFT_OUT    P0_18   // (51)  (MOSI) J3-10 & AUX-3
      #define SHIFT_CLK    P0_15   // (52)  (SCK)  J3-9 & AUX-3
      #define SHIFT_LD     P1_31   // (49)  not 5V tolerant   J3-1 & AUX-3
    #elif DISABLED(NEWPANEL)
      //#define SHIFT_OUT  P2_11   // (35)  J3-3 & AUX-4
      //#define SHIFT_CLK  P3_26   // (31)  J3-2 & AUX-4
      //#define SHIFT_LD   P3_25   // (33)  J3-4 & AUX-4
      //#define SHIFT_EN   P1_22   // (41)  J5-4 & AUX-4
    #endif

    #if ANY(VIKI2, miniVIKI)
      //#define LCD_SCREEN_ROT_180

      #define BEEPER_PIN   P1_30   // (37) may change if cable changes
      #define DOGLCD_CS    P0_26   // (63) J5-3 & AUX-2
      #define DOGLCD_SCK   SCK_PIN
      #define DOGLCD_MOSI  MOSI_PIN

      #define STAT_LED_BLUE_PIN P0_26   // (63)  may change if cable changes
      #define STAT_LED_RED_PIN  P1_21   // ( 6)  may change if cable changes
    #else
      #if ENABLED(ULTIPANEL)
        #define LCD_PINS_D5 P1_17   // (71) ENET_MDIO
        #define LCD_PINS_D6 P1_14   // (73) ENET_RX_ER
        #define LCD_PINS_D7 P1_10   // (75) ENET_RXD1
      #endif
      #define BEEPER_PIN   P1_30   // (37) not 5V tolerant
      #define DOGLCD_CS    P0_16   // (16)
    #endif

    #if ENABLED(MINIPANEL)
      // GLCD features
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
    #endif

  #endif

#endif // ULTRA_LCD

//
// SD Support
//
#if !ANY(LPC_SD_LCD, LPC_SD_ONBOARD, LPC_SD_CUSTOM_CABLE)
  #define LPC_SD_ONBOARD
#endif

#if ENABLED(LPC_SD_LCD)

  #define SCK_PIN            P0_15
  #define MISO_PIN           P0_17
  #define MOSI_PIN           P0_18
  #define SS_PIN             P1_23   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS      P0_06   // Chip select for "System" SD card

#elif ENABLED(LPC_SD_ONBOARD)

  #if ENABLED(USB_SD_ONBOARD)
    // When sharing the SD card with a PC we want the menu options to
    // mount/unmount the card and refresh it. So we disable card detect.
    #define SHARED_SD_CARD
    #undef SD_DETECT_PIN // there is also no detect pin for the onboard card
  #endif
  #define SCK_PIN            P0_07
  #define MISO_PIN           P0_08
  #define MOSI_PIN           P0_09
  #define SS_PIN             P0_06   // Chip select for SD card used by Marlin
  #define ONBOARD_SD_CS      P0_06   // Chip select for "System" SD card

#endif
