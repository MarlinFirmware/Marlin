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

/**
 * MKS SBASE pin assignments
 */

#ifndef TARGET_LPC1768
  #error "Oops!  Make sure you have the LPC1768 environment selected in your IDE."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "MKS SBASE"
  #define DEFAULT_WEBSITE_URL "https://github.com/makerbase-mks/MKS-SBASE"
#endif

// unused
/*
#define PIN_P0_27         P0_27
#define PIN_P0_28         P0_28
*/


//
// Servo pin
//
#define SERVO0_PIN         P1_23   // J8-3 (low jitter)
#define SERVO1_PIN         P2_12   // J8-4
#define SERVO2_PIN         P2_11   // J8-5
#define SERVO3_PIN         P4_28   // J8-6

//
// Limit Switches
//
#define X_MIN_PIN          P1_24   // 10k pullup to 3.3V, 1K series
#define X_MAX_PIN          P1_25   // 10k pullup to 3.3V, 1K series
#define Y_MIN_PIN          P1_26   // 10k pullup to 3.3V, 1K series
#define Y_MAX_PIN          P1_27   // 10k pullup to 3.3V, 1K series
#define Z_MIN_PIN          P1_28   // The original Mks Sbase DIO19 has a 10k pullup to 3.3V or 5V, 1K series, so when using a Zprobe we must use DIO41 (J8 P1.22)
#define Z_MAX_PIN          P1_29   // 10k pullup to 3.3V, 1K series

//
// Steppers
//
#define X_STEP_PIN         P2_00
#define X_DIR_PIN          P0_05
#define X_ENABLE_PIN       P0_04

#define Y_STEP_PIN         P2_01
#define Y_DIR_PIN          P0_11
#define Y_ENABLE_PIN       P0_10

#define Z_STEP_PIN         P2_02
#define Z_DIR_PIN          P0_20
#define Z_ENABLE_PIN       P0_19

#define E0_STEP_PIN        P2_03
#define E0_DIR_PIN         P0_22
#define E0_ENABLE_PIN      P0_21

#define E1_STEP_PIN        P2_08
#define E1_DIR_PIN         P2_13
#define E1_ENABLE_PIN      P4_29

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//

#define TEMP_BED_PIN        0   // A0 (TH1)
#define TEMP_0_PIN          1   // A1 (TH2)
#define TEMP_1_PIN          2   // A2 (TH3)
#define TEMP_2_PIN          3   // A3 (TH4)


//
// Heaters / Fans
//

#define HEATER_BED_PIN     P2_05
#define HEATER_0_PIN       P2_07
#define HEATER_1_PIN       P2_06
#define FAN_PIN            P2_04


#define PS_ON_PIN          P0_25


//
// Connector J7
//

// 5V
// NC
// GND
#define PIN_P0_17          P0_17
#define PIN_P0_16          P0_16
#define PIN_P0_14          P0_14


//
// Connector J8
//

// GND
#define PIN_P1_22          P1_22
#define PIN_P1_23          P1_23
#define PIN_P2_12          P2_12
#define PIN_P2_11          P2_11
#define PIN_P4_28          P4_28

//
// Prusa i3 MK2 Multi Material Multiplexer Support
//

#if ENABLED(MK2_MULTIPLEXER)
  #define E_MUX0_PIN         P1_23   // J8-3
  #define E_MUX1_PIN         P2_12   // J8-4
  #define E_MUX2_PIN         P2_11   // J8-5
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

#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN       P1_31   // EXP1.1
  #define BTN_ENC          P1_30   // EXP1.2
  #define BTN_EN1          P3_26   // EXP2.5
  #define BTN_EN2          P3_25   // EXP2.3
  #define LCD_PINS_RS      P0_16   // EXP1.4
  #define LCD_SDSS         P0_28   // EXP2.4
  #define LCD_PINS_ENABLE  P0_18   // EXP1.3
  #define LCD_PINS_D4      P0_15   // EXP1.5
#endif // ULTRA_LCD

//
// Ethernet pins
//
#ifndef ULTIPANEL
  #define ENET_MDIO        P1_17   // J12-4
  #define ENET_RX_ER       P1_14   // J12-6
  #define ENET_RXD1        P1_10   // J12-8
#endif
#define ENET_MOC           P1_16   // J12-3
#define REF_CLK            P1_15   // J12-5
#define ENET_RXD0          P1_09   // J12-7
#define ENET_CRS           P1_08   // J12-9
#define ENET_TX_EN         P1_04   // J12-10
#define ENET_TXD0          P1_00   // J12-11
#define ENET_TXD1          P1_01   // J12-12

//
// Misc. Functions
//
#define LPC_SOFTWARE_SPI  // MKS_SBASE needs a software SPI because the
                          // selected pins are not on a hardware SPI controller

// A custom cable is needed. See the README file in the
// Marlin\src\config\examples\Mks\Sbase directory

#define SCK_PIN            P1_22   // J8-2 (moved from EXP2 P0.7)
#define MISO_PIN           P1_23   // J8-3 (moved from EXP2 P0.8)
#define MOSI_PIN           P2_12   // J8-4 (moved from EXP2 P0.5)
#define SS_PIN             P0_28
#define SDSS               P0_06

/**
 * P0.27 is on EXP2 and the on-board SD card's socket. That means it can't be
 * used as the SD_DETECT for the LCD's SD card.
 *
 * The best solution is to use the custom cable to connect the LCD's SD_DETECT
 * to a pin NOT on EXP2.
 *
 * If you can't find a pin to use for the LCD's SD_DETECT then comment out
 * SD_DETECT_PIN entirely and remove that wire from the the custom cable.
 */
#define SD_DETECT_PIN      P2_11   // J8-5 (moved from EXP2 P0.27)


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
  *   P1_30 - not 5V tolerant
  *   P1_31 - not 5V tolerant
  *   P0_27 - open collector
  *   P0_28 - open collector
  *
  */
