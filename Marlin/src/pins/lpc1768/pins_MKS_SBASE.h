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
 * MKS SBASE pin assignments
 */

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME   "MKS SBASE"
#endif
#ifndef BOARD_WEBSITE_URL
  #define BOARD_WEBSITE_URL "github.com/makerbase-mks/MKS-SBASE"
#endif

#define LED_PIN                            P1_18  // Used as a status indicator
#define LED2_PIN                           P1_19
#define LED3_PIN                           P1_20
#define LED4_PIN                           P1_21

//
// Servos
//
#define SERVO0_PIN                         P1_23  // J8-3 (low jitter)
#define SERVO1_PIN                         P2_12  // J8-4
#define SERVO2_PIN                         P2_11  // J8-5
#define SERVO3_PIN                         P4_28  // J8-6

//
// Limit Switches - Not Interrupt Capable
//
#define X_MIN_PIN                          P1_24  // 10k pullup to 3.3V, 1K series
#define X_MAX_PIN                          P1_25  // 10k pullup to 3.3V, 1K series
#define Y_MIN_PIN                          P1_26  // 10k pullup to 3.3V, 1K series
#define Y_MAX_PIN                          P1_27  // 10k pullup to 3.3V, 1K series
#define Z_MIN_PIN                          P1_28  // The original Mks Sbase DIO19 has a 10k pullup to 3.3V or 5V, 1K series, so when using a Zprobe we must use DIO41 (J8 P1.22)
#define Z_MAX_PIN                          P1_29  // 10k pullup to 3.3V, 1K series

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                  P4_28  // Connector J8
#endif

//
// Steppers
//
#define X_STEP_PIN                         P2_00
#define X_DIR_PIN                          P0_05
#define X_ENABLE_PIN                       P0_04

#define Y_STEP_PIN                         P2_01
#define Y_DIR_PIN                          P0_11
#define Y_ENABLE_PIN                       P0_10

#define Z_STEP_PIN                         P2_02
#define Z_DIR_PIN                          P0_20
#define Z_ENABLE_PIN                       P0_19

#define E0_STEP_PIN                        P2_03
#define E0_DIR_PIN                         P0_22
#define E0_ENABLE_PIN                      P0_21

#define E1_STEP_PIN                        P2_08
#define E1_DIR_PIN                         P2_13
#define E1_ENABLE_PIN                      P4_29

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//
#define TEMP_BED_PIN                    P0_23_A0  // A0 (TH1)
#define TEMP_0_PIN                      P0_24_A1  // A1 (TH2)
#define TEMP_1_PIN                      P0_25_A2  // A2 (TH3)
#define TEMP_2_PIN                      P0_26_A3  // A3 (TH4)

//
// Heaters / Fans
//
#define HEATER_BED_PIN                     P2_05
#define HEATER_0_PIN                       P2_07
#define HEATER_1_PIN                       P2_06
#ifndef FAN_PIN
  #define FAN_PIN                          P2_04
#endif

//
// Connector J7
// Note: These pins are all digitally shared with the EXP1/EXP2 Connector.
// Using them with an LCD connected or configured will lead to hangs & crashes.
//

// 5V
// NC
// GND
#define PIN_P0_17                          P0_17
#define PIN_P0_16                          P0_16
#define PIN_P0_15                          P0_15

//
// Connector J8
//

// GND
#define PIN_P1_22                          P1_22
#define PIN_P1_23                          P1_23  // PWM Capable
#define PIN_P2_12                          P2_12  // Interrupt Capable
#define PIN_P2_11                          P2_11  // Interrupt Capable

//
// Průša i3 MMU1 (Multi Material Multiplexer) Support
//
#if HAS_PRUSA_MMU1
  #define E_MUX0_PIN                       P1_23  // J8-3
  #define E_MUX1_PIN                       P2_12  // J8-4
  #define E_MUX2_PIN                       P2_11  // J8-5
#endif

//
// Power Supply Control
//
#if ENABLED(MKS_PWC)
  #define PS_ON_PIN                        P0_25  // SERVO
  #define KILL_PIN                         P1_29  // Z+
  #define KILL_PIN_STATE                    HIGH
#endif

//
// Ethernet pins
//
#if !IS_ULTIPANEL
  #define ENET_MDIO                        P1_17  // J12-4
  #define ENET_RX_ER                       P1_14  // J12-6
  #define ENET_RXD1                        P1_10  // J12-8
#endif

#define ENET_MOC                           P1_16  // J12-3
#define REF_CLK                            P1_15  // J12-5
#define ENET_RXD0                          P1_09  // J12-7
#define ENET_CRS                           P1_08  // J12-9
#define ENET_TX_EN                         P1_04  // J12-10
#define ENET_TXD0                          P1_00  // J12-11
#define ENET_TXD1                          P1_01  // J12-12

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#define ONBOARD_SD_CS_PIN                  P0_06  // Chip select for "System" SD card

#if SD_CONNECTION_IS(CUSTOM_CABLE)

  /**
   * A custom cable is needed. See the README file in the
   * Marlin\src\config\examples\Mks\Sbase directory
   * P0.27 is on EXP2 and the on-board SD card's socket. That means it can't be
   * used as the SD_DETECT for the LCD's SD card.
   *
   * The best solution is to use the custom cable to connect the LCD's SD_DETECT
   * to a pin NOT on EXP2.
   *
   * If you can't find a pin to use for the LCD's SD_DETECT then comment out
   * SD_DETECT_PIN entirely and remove that wire from the the custom cable.
   */
  #define SD_DETECT_PIN                    P2_11  // J8-5 (moved from EXP2 P0.27)
  #define SD_SCK_PIN                       P1_22  // J8-2 (moved from EXP2 P0.7)
  #define SD_MISO_PIN                      P1_23  // J8-3 (moved from EXP2 P0.8)
  #define SD_MOSI_PIN                      P2_12  // J8-4 (moved from EXP2 P0.9)
  #define SD_SS_PIN                        P0_28
  #define LPC_SOFTWARE_SPI                        // With a custom cable we need software SPI because the
                                                  // selected pins are not on a hardware SPI controller
#elif SD_CONNECTION_IS(LCD) || SD_CONNECTION_IS(ONBOARD)
  #define SD_SCK_PIN                       P0_07
  #define SD_MISO_PIN                      P0_08
  #define SD_MOSI_PIN                      P0_09
  #if SD_CONNECTION_IS(LCD)
    // Use standard cable and header, SPI and SD detect are shared with onboard SD card.
    // Hardware SPI is used for both SD cards. The detect pin is shared between the
    // LCD and onboard SD readers so we disable it.
    #define SD_SS_PIN                      P0_28
  #else
    #define SD_DETECT_PIN                  P0_27
    #define SD_SS_PIN          ONBOARD_SD_CS_PIN
  #endif
#endif

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

#if IS_TFTGLCD_PANEL

  #if ENABLED(TFTGLCD_PANEL_SPI)
    #define TFTGLCD_CS                     P3_25  // EXP2.3
  #endif

  #if SD_CONNECTION_IS(LCD)
    #define SD_DETECT_PIN                  P0_28  // EXP2.4
  #endif

#elif HAS_WIRED_LCD

  #define BEEPER_PIN                       P1_31  // EXP1.1
  #define BTN_ENC                          P1_30  // EXP1.2
  #define BTN_EN1                          P3_26  // EXP2.5
  #define BTN_EN2                          P3_25  // EXP2.3
  #define LCD_PINS_RS                      P0_16  // EXP1.4
  #define LCD_SDSS                         P0_28  // EXP2.4
  #define LCD_PINS_ENABLE                  P0_18  // EXP1.3
  #define LCD_PINS_D4                      P0_15  // EXP1.5
  #if ANY(VIKI2, miniVIKI)
    #define DOGLCD_SCK                SD_SCK_PIN
    #define DOGLCD_MOSI              SD_MOSI_PIN
  #endif

  #if ENABLED(FYSETC_MINI_12864)
    /**
     * The FYSETC display can NOT use the SCK and MOSI pins on EXP2, so a
     * special cable is needed to go between EXP2 on the FYSETC and the
     * controller board's EXP2 and J8. It also means that a software SPI
     * is needed to drive those pins.
     *
     * The FYSETC requires mode 3 SPI interface.
     *
     * Pins 6, 7 & 8 on EXP2 are no connects. That means a second special
     * cable will be needed if the RGB LEDs are to be active.
     */
    #define DOGLCD_CS            LCD_PINS_ENABLE  // EXP1.3  (LCD_EN on FYSETC schematic)
    #define DOGLCD_A0                LCD_PINS_RS  // EXP1.4  (LCD_A0 on FYSETC schematic)
    #define DOGLCD_SCK                     P2_11  // J8-5  (SCK on FYSETC schematic)
    #define DOGLCD_MOSI                    P4_28  // J8-6  (MOSI on FYSETC schematic)

    //#define FORCE_SOFT_SPI                      // Use this if default of hardware SPI causes display problems
                                                  //   results in LCD soft SPI mode 3, SD soft SPI mode 0

    #if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0)
      #ifndef RGB_LED_R_PIN
        #define RGB_LED_R_PIN              P2_12  // J8-4  (LCD_D6 on FYSETC schematic)
      #endif
      #ifndef RGB_LED_G_PIN
        #define RGB_LED_G_PIN              P1_23  // J8-3  (LCD_D5 on FYSETC schematic)
      #endif
      #ifndef RGB_LED_B_PIN
        #define RGB_LED_B_PIN              P1_22  // J8-2  (LCD_D7 on FYSETC schematic)
      #endif
    #elif ENABLED(FYSETC_MINI_12864_2_1)
      #define NEOPIXEL_PIN                 P2_12
    #endif

  #elif ENABLED(MINIPANEL)
    // GLCD features
    // Uncomment screen orientation
    //#define LCD_SCREEN_ROT_90
    //#define LCD_SCREEN_ROT_180
    //#define LCD_SCREEN_ROT_270
  #endif

#endif // HAS_WIRED_LCD

/**
 * Example for trinamic drivers using the J8 connector on MKs Sbase.
 * 2130s need 1 pin for each driver. 2208/2209s need 2 pins for serial control.
 * This board does not have enough pins to use hardware serial.
 */

#if HAS_DRIVER(TMC2130)
  // J8
  #define X_CS_PIN                         P1_22
  #define Y_CS_PIN                         P1_23
  #define Z_CS_PIN                         P2_12
  #define E0_CS_PIN                        P2_11
  #define E1_CS_PIN                        P4_28

  // Hardware SPI is on EXP2. See if you can make it work:
  // https://github.com/makerbase-mks/MKS-SBASE/issues/25
  #define TMC_USE_SW_SPI
  #if ENABLED(TMC_USE_SW_SPI)
    #ifndef TMC_SW_MOSI
      #define TMC_SW_MOSI                  P0_03  // AUX1
    #endif
    #ifndef TMC_SW_MISO
      #define TMC_SW_MISO                  P0_02  // AUX1
    #endif
    #ifndef TMC_SW_SCK
      #define TMC_SW_SCK                   P0_26  // TH4
    #endif
  #endif

#endif

#if MB(MKS_SBASE) && HAS_TMC_UART

  /**
   * TMC2208/TMC2209 stepper drivers
   *
   * The shortage of pins becomes apparent.
   * Worst case you may have to give up the LCD
   * RX pins need to be interrupt capable
   */
  #define X_SERIAL_TX_PIN                  P1_22  // J8-2
  #define X_SERIAL_RX_PIN                  P2_12  // J8-4 Interrupt Capable
  #define Y_SERIAL_TX_PIN                  P1_23  // J8-3
  #define Y_SERIAL_RX_PIN                  P2_11  // J8-5 Interrupt Capable
  #define Z_SERIAL_TX_PIN                  P2_12  // J8-4
  #define Z_SERIAL_RX_PIN                  P0_25  // TH3
  #define E0_SERIAL_TX_PIN                 P4_28  // J8-6
  #define E0_SERIAL_RX_PIN                 P0_26  // TH4

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

// UNUSED
#define PIN_P0_27                          P0_27  // EXP2/Onboard SD
#define PIN_P0_28                          P0_28  // EXP2
#define PIN_P0_02                          P0_02  // AUX1 (Interrupt Capable/ADC/Serial Port 0)
#define PIN_P0_03                          P0_03  // AUX1 (Interrupt Capable/ADC/Serial Port 0)

/**
 *  PWMs
 *
 *  There are 6 PWMs.  Each PWM can be assigned to one of two pins.
 *
 *  SERVO2 does NOT have a PWM assigned to it.
 *
 *  PWM1.1   P1_18   SERVO3_PIN       FIL_RUNOUT_PIN   5V output, PWM
 *  PWM1.1   P2_00   E0_STEP_PIN
 *  PWM1.2   P1_20   SERVO0_PIN
 *  PWM1.2   P2_01   X_STEP_PIN
 *  PWM1.3   P1_21   SERVO1_PIN       J5-1
 *  PWM1.3   P2_02   Y_STEP_PIN
 *  PWM1.4   P1_23   SDSS(SSEL0)      J3-5  AUX-3
 *  PWM1.4   P2_03   Z_STEP_PIN
 *  PWM1.5   P1_24   X_MIN_PIN        10K PULLUP TO 3.3v, 1K SERIES
 *  PWM1.5   P2_04   RAMPS_D9_PIN
 *  PWM1.6   P1_26   Y_MIN_PIN        10K PULLUP TO 3.3v, 1K SERIES
 *  PWM1.6   P2_05   RAMPS_D10_PIN
 */

/**
 * Special pins
 *   P1_30 - not 5V tolerant - EXP1
 *   P1_31 - not 5V tolerant - EXP1
 *   P0_27 - open collector  - EXP2
 *   P0_28 - open collector  - EXP2
 */

/**
 * Serial Ports
 *   P0_00 - Port  3
 *   P0_01 - SD Card (Onboard)
 *   P0_10 - Port  2
 *   P0_11 - Y_EN/Y_DIR
 *   P0_15 - Port  1
 *   P0_16 - EXP1
 *   P0_02 - Port  0
 *   P0_03 - AUX1
 *   P0_29 - Port -1
 *   P0_30 - USB
 */
