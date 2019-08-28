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
 * Re-ARM with RAMPS v1.4 pin assignments
 *
 * Applies to the following boards:
 *
 *  RAMPS_14_EFB (Hotend, Fan, Bed)
 *  RAMPS_14_EEB (Hotend0, Hotend1, Bed)
 *  RAMPS_14_EFF (Hotend, Fan0, Fan1)
 *  RAMPS_14_EEF (Hotend0, Hotend1, Fan)
 *  RAMPS_14_SF  (Spindle, Controller Fan)
 *
 */

// Numbers in parentheses () are the corresponding mega2560 pin numbers

#ifndef TARGET_LPC1768
  #error "Oops! Make sure you have the LPC1768 environment selected in your IDE."
#endif

#define BOARD_INFO_NAME "Re-ARM RAMPS 1.4"

//
// Servos
//
#define SERVO0_PIN         P1_20   // (11)
#define SERVO1_PIN         P1_21   // ( 6) also on J5-1
#define SERVO2_PIN         P1_19   // ( 5)
#define SERVO3_PIN         P1_18   // ( 4) 5V output

//
// Limit Switches
//
#define X_MIN_PIN          P1_24   // ( 3) 10k pullup to 3.3V, 1K series
#define X_MAX_PIN          P1_25   // ( 2) 10k pullup to 3.3V, 1K series
#define Y_MIN_PIN          P1_26   // (14) 10k pullup to 3.3V, 1K series
#define Y_MAX_PIN          P1_27   // (15) 10k pullup to 3.3V, 1K series
#define Z_MIN_PIN          P1_29   // (18) 10k pullup to 3.3V, 1K series
#define Z_MAX_PIN          P1_28   // (19) 10k pullup to 3.3V, 1K series

//
// Steppers
//
#define X_STEP_PIN         P2_01   // (54)
#define X_DIR_PIN          P0_11   // (55)
#define X_ENABLE_PIN       P0_10   // (38)
#ifndef X_CS_PIN
  #define X_CS_PIN         P1_01   // ETH
#endif

#define Y_STEP_PIN         P2_02   // (60)
#define Y_DIR_PIN          P0_20   // (61)
#define Y_ENABLE_PIN       P0_19   // (56)
#ifndef Y_CS_PIN
  #define Y_CS_PIN         P1_04   // ETH
#endif

#define Z_STEP_PIN         P2_03   // (46)
#define Z_DIR_PIN          P0_22   // (48)
#define Z_ENABLE_PIN       P0_21   // (62)
#ifndef Z_CS_PIN
  #define Z_CS_PIN         P1_10   // ETH
#endif

#define E0_STEP_PIN        P2_00   // (26)
#define E0_DIR_PIN         P0_05   // (28)
#define E0_ENABLE_PIN      P0_04   // (24)
#ifndef E0_CS_PIN
  #define E0_CS_PIN        P1_14   // ETH
#endif

#define E1_STEP_PIN        P2_08   // (36)
#define E1_DIR_PIN         P2_13   // (34)
#define E1_ENABLE_PIN      P4_29   // (30)
#ifndef E1_CS_PIN
  #define E1_CS_PIN        -1
#endif

//
// Software SPI pins for TMC2130 stepper drivers
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    P1_00   // ETH
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    P1_08   // ETH
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     P1_09   // ETH
  #endif
#endif

#if HAS_TMC220x
  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * Hardware serial communication ports.
   * If undefined software serial is used according to the pins below
   */

  //
  // Software serial
  //

  // P2_08 E1-Step
  // P2_13 E1-Dir

  #ifndef  X_SERIAL_TX_PIN
    #define X_SERIAL_TX_PIN  P0_01
  #endif
  #ifndef X_SERIAL_RX_PIN
    #define X_SERIAL_RX_PIN  P0_01
  #endif

  #ifndef Y_SERIAL_TX_PIN
    #define Y_SERIAL_TX_PIN  P0_00
  #endif
  #ifndef Y_SERIAL_RX_PIN
    #define Y_SERIAL_RX_PIN  P0_00
  #endif

  #ifndef Z_SERIAL_TX_PIN
    #define Z_SERIAL_TX_PIN  P2_13
  #endif
  #ifndef Z_SERIAL_RX_PIN
    #define Z_SERIAL_RX_PIN  P2_13
  #endif

  #ifndef E0_SERIAL_TX_PIN
    #define E0_SERIAL_TX_PIN P2_08
  #endif
  #ifndef E0_SESIAL_RX_PIN
    #define E0_SERIAL_RX_PIN P2_08
  #endif

#endif

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN          0   // A0 (T0) - (67) - TEMP_0_PIN
#define TEMP_BED_PIN        1   // A1 (T1) - (68) - TEMP_BED_PIN
#define TEMP_1_PIN          2   // A2 (T2) - (69) - TEMP_1_PIN
#define TEMP_2_PIN          3   // A3 - (63) - J5-3 & AUX-2
#define TEMP_3_PIN          4   // A4 - (37) - BUZZER_PIN
//#define TEMP_4_PIN          5   // A5 - (49) - SD_DETECT_PIN
//#define ??                  6   // A6 - ( 0)  - RXD0 - J4-4 & AUX-1
#define FILWIDTH_PIN        7   // A7 - ( 1)  - TXD0 - J4-5 & AUX-1

//
// Augmentation for auto-assigning RAMPS plugs
//
#if NONE(IS_RAMPS_EEB, IS_RAMPS_EEF, IS_RAMPS_EFB, IS_RAMPS_EFF, IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  #if HOTENDS > 1
    #if TEMP_SENSOR_BED
      #define IS_RAMPS_EEB
    #else
      #define IS_RAMPS_EEF
    #endif
  #elif TEMP_SENSOR_BED
    #define IS_RAMPS_EFB
  #else
    #define IS_RAMPS_EFF
  #endif
#endif

//
// Heaters / Fans
//
#ifndef MOSFET_D_PIN
  #define MOSFET_D_PIN     -1
#endif
#ifndef RAMPS_D8_PIN
  #define RAMPS_D8_PIN     P2_07   // (8)
#endif
#ifndef RAMPS_D9_PIN
  #define RAMPS_D9_PIN     P2_04   // (9)
#endif
#ifndef RAMPS_D10_PIN
  #define RAMPS_D10_PIN    P2_05   // (10)
#endif

#define HEATER_0_PIN       RAMPS_D10_PIN

#if ENABLED(IS_RAMPS_EFB)                      // Hotend, Fan, Bed
  #define HEATER_BED_PIN   RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EEF)                    // Hotend, Hotend, Fan
  #define HEATER_1_PIN     RAMPS_D9_PIN
#elif ENABLED(IS_RAMPS_EEB)                    // Hotend, Hotend, Bed
  #define HEATER_1_PIN     RAMPS_D9_PIN
  #define HEATER_BED_PIN   RAMPS_D8_PIN
#elif ENABLED(IS_RAMPS_EFF)                    // Hotend, Fan, Fan
  #define FAN1_PIN         RAMPS_D8_PIN
#elif DISABLED(IS_RAMPS_SF)                    // Not Spindle, Fan (i.e., "EFBF" or "EFBE")
  #define HEATER_BED_PIN   RAMPS_D8_PIN
  #if HOTENDS == 1
    #define FAN1_PIN       MOSFET_D_PIN
  #else
    #define HEATER_1_PIN   MOSFET_D_PIN
  #endif
#endif

#ifndef FAN_PIN
  #if EITHER(IS_RAMPS_EFB, IS_RAMPS_EFF)          // Hotend, Fan, Bed or Hotend, Fan, Fan
    #define FAN_PIN        RAMPS_D9_PIN
  #elif EITHER(IS_RAMPS_EEF, IS_RAMPS_SF)         // Hotend, Hotend, Fan or Spindle, Fan
    #define FAN_PIN        RAMPS_D8_PIN
  #elif ENABLED(IS_RAMPS_EEB)                  // Hotend, Hotend, Bed
    #define FAN_PIN        P1_18               // (4) IO pin. Buffer needed
  #else                                        // Non-specific are "EFB" (i.e., "EFBF" or "EFBE")
    #define FAN_PIN        RAMPS_D9_PIN
  #endif
#endif

//
// Misc. Functions
//
#define LED_PIN            P4_28   // (13)

// define digital pin 4 for the filament runout sensor. Use the RAMPS 1.4 digital input 4 on the servos connector
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN   P1_18   // (4)
#endif

#define PS_ON_PIN          P2_12   // (12)

#if !defined(MAX6675_SS_PIN) && DISABLED(USE_ZMAX_PLUG)
  #define MAX6675_SS_PIN   P1_28
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT) && !defined(SPINDLE_LASER_ENA_PIN)
  #if !defined(NUM_SERVOS) || NUM_SERVOS < 4   // Try to use servo connector
    #define CASE_LIGHT_PIN P1_18   // (4) MUST BE HARDWARE PWM
  #endif
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//            Use servo pins, if available
//
#if HAS_CUTTER && !PIN_EXISTS(SPINDLE_LASER_ENA)
  #if NUM_SERVOS > 1
    #if ENABLED(SPINDLE_FEATURE)
      #error "SPINDLE_FEATURE requires 3 free servo pins."
    #else
      #error "LASER_FEATURE requires 3 free servo pins."
    #endif
  #endif
  #define SPINDLE_LASER_ENA_PIN SERVO1_PIN   // (6) Pin should have a pullup/pulldown!
  #define SPINDLE_LASER_PWM_PIN SERVO3_PIN   // (4) MUST BE HARDWARE PWM
  #define SPINDLE_DIR_PIN       SERVO2_PIN   // (5)
#endif

//
// Průša i3 MK2 Multiplexer Support
//
#if SERIAL_PORT != 0 && SERIAL_PORT_2 != 0
  #define E_MUX0_PIN       P0_03   // ( 0) Z_CS_PIN
  #define E_MUX1_PIN       P0_02   // ( 1) E0_CS_PIN
#endif
#define E_MUX2_PIN         P0_26   // (63) E1_CS_PIN

/**
 * LCD / Controller
 *
 * All controllers can use J3 and J5 on the Re-ARM board. Custom cabling will be required.
 *
 * - https://github.com/wolfmanjm/universal-panel-adapter
 * - http://panucattdevices.freshdesk.com/support/solutions/articles/1000243195-lcd-display-installation
 */

/**
 * Smart LCD adapter
 *
 * The Smart LCD adapter can be used for the two 10 pin LCD controllers such as
 * REPRAP_DISCOUNT_SMART_CONTROLLER. It can't be used for controllers that use
 * DOGLCD_A0, DOGLCD_CS, LCD_PINS_D5, LCD_PINS_D6 or LCD_PINS_D7. A custom cable
 * is needed to pick up 5V for the EXP1 connection.
 *
 * SD card on the LCD uses the same SPI signals as the LCD. This results in garbage/lines
 * on the LCD display during accesses of the SD card. The menus/code has been arranged so
 * that the garbage/lines are erased immediately after the SD card accesses are completed.
 */

#if ENABLED(CR10_STOCKDISPLAY)

  // Re-Arm can support Creality stock display without SD card reader and single cable on EXP3.
  // Re-Arm J3 pins 1 (p1.31) & 2 (P3.26) are not used. Stock cable will need to have one
  // 10-pin IDC connector trimmed or replaced with a 12-pin IDC connector to fit J3.
  // Requires REVERSE_ENCODER_DIRECTION in Configuration.h

  #define BEEPER_PIN       P2_11   // J3-3 & AUX-4

  #define BTN_EN1          P0_16   // J3-7 & AUX-4
  #define BTN_EN2          P1_23   // J3-5 & AUX-4
  #define BTN_ENC          P3_25   // J3-4 & AUX-4

  #define LCD_PINS_RS      P0_15   // J3-9 & AUX-4 (CS)
  #define LCD_PINS_ENABLE  P0_18   // J3-10 & AUX-3 (SID, MOSI)
  #define LCD_PINS_D4      P2_06   // J3-8 & AUX-3 (SCK, CLK)

#elif HAS_SPI_LCD

  //#define SCK_PIN          P0_15   // (52)  system defined J3-9 & AUX-3
  //#define MISO_PIN         P0_17   // (50)  system defined J3-10 & AUX-3
  //#define MOSI_PIN         P0_18   // (51)  system defined J3-10 & AUX-3
  //#define SS_PIN           P1_23   // (53)  system defined J3-5 & AUX-3 (Sometimes called SDSS)

  #if ENABLED(FYSETC_MINI_12864)
    #define BEEPER_PIN     P1_01
    #define BTN_ENC        P1_04
  #else
    #define BEEPER_PIN     P1_30   // (37) not 5V tolerant
    #define BTN_ENC        P2_11   // (35) J3-3 & AUX-4
  #endif

  #define BTN_EN1          P3_26   // (31) J3-2 & AUX-4
  #define BTN_EN2          P3_25   // (33) J3-4 & AUX-4

  #define SD_DETECT_PIN    P1_31   // (49) J3-1 & AUX-3 (NOT 5V tolerant)
  #define KILL_PIN         P1_22   // (41) J5-4 & AUX-4
  #define LCD_PINS_RS      P0_16   // (16) J3-7 & AUX-4
  #define LCD_SDSS         P0_16   // (16) J3-7 & AUX-4

  #if ENABLED(NEWPANEL)
    #if ENABLED(REPRAPWORLD_KEYPAD)
      #define SHIFT_OUT    P0_18   // (51) (MOSI) J3-10 & AUX-3
      #define SHIFT_CLK    P0_15   // (52) (SCK)  J3-9 & AUX-3
      #define SHIFT_LD     P1_31   // (49)        J3-1 & AUX-3 (NOT 5V tolerant)
    #endif
  #else
    //#define SHIFT_CLK      P3_26   // (31)  J3-2 & AUX-4
    //#define SHIFT_LD       P3_25   // (33)  J3-4 & AUX-4
    //#define SHIFT_OUT      P2_11   // (35)  J3-3 & AUX-4
    //#define SHIFT_EN       P1_22   // (41)  J5-4 & AUX-4
  #endif

  #if ANY(VIKI2, miniVIKI)
    // #define LCD_SCREEN_ROT_180

    #define DOGLCD_CS      P0_16   // (16)
    #define DOGLCD_A0      P2_06   // (59) J3-8 & AUX-2
    #define DOGLCD_SCK     SCK_PIN
    #define DOGLCD_MOSI    MOSI_PIN

    #define STAT_LED_BLUE_PIN P0_26 //(63)  may change if cable changes
    #define STAT_LED_RED_PIN P1_21 // ( 6)  may change if cable changes
  #else

    #if ENABLED(FYSETC_MINI_12864)
      #define DOGLCD_SCK   P0_15
      #define DOGLCD_MOSI  P0_18

      // EXP1 on LCD adapter is not usable - using Ethernet connector instead
      #define DOGLCD_CS    P1_09
      #define DOGLCD_A0    P1_14
      //#define FORCE_SOFT_SPI    // Use this if default of hardware SPI causes display problems
                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

      #define LCD_RESET_PIN P0_16   // Must be high or open for LCD to operate normally.

      #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
        #ifndef RGB_LED_R_PIN
          #define RGB_LED_R_PIN P1_00
        #endif
        #ifndef RGB_LED_G_PIN
          #define RGB_LED_G_PIN P1_01
        #endif
        #ifndef RGB_LED_B_PIN
          #define RGB_LED_B_PIN P1_08
        #endif
      #elif ENABLED(FYSETC_MINI_12864_2_1)
        #define NEOPIXEL_PIN P1_00
      #endif
    #else
      #define DOGLCD_CS    P0_26   // (63) J5-3 & AUX-2
      #define DOGLCD_A0    P2_06   // (59) J3-8 & AUX-2
    #endif

    #define LCD_BACKLIGHT_PIN P0_16 //(16) J3-7 & AUX-4 - only used on DOGLCD controllers
    #define LCD_PINS_ENABLE P0_18  // (51) (MOSI) J3-10 & AUX-3
    #define LCD_PINS_D4    P0_15   // (52) (SCK)  J3-9 & AUX-3
    #if ENABLED(ULTIPANEL)
      #define LCD_PINS_D5  P1_17   // (71) ENET_MDIO
      #define LCD_PINS_D6  P1_14   // (73) ENET_RX_ER
      #define LCD_PINS_D7  P1_10   // (75) ENET_RXD1
    #endif
  #endif

  #if ENABLED(MINIPANEL)
    // GLCD features
    // Uncomment screen orientation
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270
  #endif

#endif // HAS_SPI_LCD

//
// Ethernet pins
//
#if DISABLED(ULTIPANEL)
  #define ENET_MDIO        P1_17   // (71)  J12-4
  #define ENET_RX_ER       P1_14   // (73)  J12-6
  #define ENET_RXD1        P1_10   // (75)  J12-8
#endif
#define ENET_MOC           P1_16   // (70)  J12-3
#define REF_CLK            P1_15   // (72)  J12-5
#define ENET_RXD0          P1_09   // (74)  J12-7
#define ENET_CRS           P1_08   // (76)  J12-9
#define ENET_TX_EN         P1_04   // (77)  J12-10
#define ENET_TXD0          P1_00   // (78)  J12-11
#define ENET_TXD1          P1_01   // (79)  J12-12

//
// SD Support
//
#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION ONBOARD
#endif

#define ONBOARD_SD_CS_PIN  P0_06   // Chip select for "System" SD card

#if SD_CONNECTION_IS(LCD)
  #define SCK_PIN          P0_15   // (52)  system defined J3-9 & AUX-3
  #define MISO_PIN         P0_17   // (50)  system defined J3-10 & AUX-3
  #define MOSI_PIN         P0_18   // (51)  system defined J3-10 & AUX-3
  #define SS_PIN           P1_23   // (53)  system defined J3-5 & AUX-3 (Sometimes called SDSS) - CS used by Marlin
#elif SD_CONNECTION_IS(ONBOARD)
  #undef SD_DETECT_PIN
  #define SCK_PIN          P0_07
  #define MISO_PIN         P0_08
  #define MOSI_PIN         P0_09
  #define SS_PIN           ONBOARD_SD_CS_PIN
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

/**
 *  Fast PWMs
 *
 *  The LPC1768's hardware PWM controller has 6 channels. Each channel
 *  can be setup to either control a dedicated pin directly or to generate
 *  an interrupt. The direct method's duty cycle is accurate to within a
 *  a microsecond. The interrupt method's average duty cycle has the
 *  the same accuracy but the individual cycles can vary because of higher
 *  priority interrupts.
 *
 *  All Fast PWMs have a 50Hz rate.
 *
 *  The following pins/signals use the direct method. All other pins use the
 *  the interrupt method. Note that SERVO2_PIN and RAMPS_D8_PIN use the
 *  interrupt method.
 *
 *     P1_20 (11)   SERVO0_PIN
 *     P1_21 ( 6)   SERVO1_PIN       J5-1
 *     P0_18 ( 4)   SERVO3_PIN       5V output
 *    *P2_04 ( 9)   RAMPS_D9_PIN
 *    *P2_05 (10)   RAMPS_D10_PIN
 *
 *    * - If used as a heater driver then a Fast PWM is NOT assigned. If used as
 *        a fan driver then enabling FAST_PWM_FAN assigns a Fast PWM to it.
 */

 /**
  * Special pins
  *   P1_30  (37) (NOT 5V tolerant)
  *   P1_31  (49) (NOT 5V tolerant)
  *   P0_27  (57) (Open collector)
  *   P0_28  (58) (Open collector)
  */

/**
 *  The following mega2560 pins are NOT available in a Re-ARM system:
 *
 *  7, 17, 22, 23, 25, 27, 29, 32, 39, 40, 42, 43, 44, 45, 47, 64, 65, 66
 */
