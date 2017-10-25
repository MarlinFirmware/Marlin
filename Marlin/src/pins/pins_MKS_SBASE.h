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

//#if !defined(TARGET_LPC1768)
#if DISABLED(IS_REARM)
  #error "Oops!  Make sure you have Re-Arm selected."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "MKS SBASE"
  #define DEFAULT_WEBSITE_URL "https://github.com/makerbase-mks/MKS-SBASE"
#endif

#define LARGE_FLASH true

// unused
/*
#define D57               57
#define D58               58
*/

//
// Limit Switches
//
#define X_MIN_PIN           3  //10k pullup to 3.3V, 1K series
#define X_MAX_PIN           2  //10k pullup to 3.3V, 1K series
#define Y_MIN_PIN          14  //10k pullup to 3.3V, 1K series
#define Y_MAX_PIN          15  //10k pullup to 3.3V, 1K series
#define Z_MIN_PIN          19  //The original Mks Sbase DIO19 has a 10k pullup to 3.3V or 5V, 1K series, so when using a Zprobe we must use DIO41 (J8 P1.22)
#define Z_MAX_PIN          18  //10k pullup to 3.3V, 1K series

//
// Steppers
//
#define X_STEP_PIN         26
#define X_DIR_PIN          28
#define X_ENABLE_PIN       24

#define Y_STEP_PIN         54
#define Y_DIR_PIN          55
#define Y_ENABLE_PIN       38

#define Z_STEP_PIN         60
#define Z_DIR_PIN          61
#define Z_ENABLE_PIN       56

#define E0_STEP_PIN        46
#define E0_DIR_PIN         48
#define E0_ENABLE_PIN      62

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define X2_STEP_PIN        36
#define X2_DIR_PIN         34
#define X2_ENABLE_PIN      30

//
// Temperature Sensors
// 3.3V max when defined as an analog input
//

#define TEMP_0_PIN          0  // A0 (TH1)
#define TEMP_BED_PIN        1  // A1 (TH2)
#define TEMP_1_PIN          2  // A2 (TH3)
#define TEMP_2_PIN          3  // A3 (TH4)


//
// Heaters / Fans
//

#define HEATER_BED_PIN     10
#define HEATER_0_PIN        8
#define HEATER_1_PIN       59
#define FAN_PIN             9


#define PS_ON_PIN          69


//
// Connector J7
//

// 5V
// NC
// GND
#define PIN_P0_17          50
#define PIN_P0_16          16
#define PIN_P0_14          80


//
// Connector J8
//

// GND
#define PIN_P1_22          41
#define PIN_P1_23          53
#define PIN_P2_12          12
#define PIN_P2_11          35
#define PIN_P4_28          13

//
// Prusa i3 MK2 Multi Material Multiplexer Support
//

#define E_MUX0_PIN         50   // J7-4
#define E_MUX1_PIN         16   // J7-5
#define E_MUX2_PIN         80   // J7-6


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
  #define BEEPER_PIN       49  // EXP1.1
  #define BTN_ENC          37  // EXP1.2
  #define BTN_EN1          31  // EXP2.5
  #define BTN_EN2          33  // EXP2.3
  #define SD_DETECT_PIN    57  // EXP2.7
  #define LCD_PINS_RS      16  // EXP1.4
  #define LCD_SDSS         58  // EXP2.4
  #define LCD_PINS_ENABLE  51  // EXP1.3
  #define LCD_PINS_D4      80  // EXP1.5
#endif // ULTRA_LCD

//
// Ethernet pins
//
#ifndef ULTIPANEL
  #define ENET_MDIO        71  // J12-4
  #define ENET_RX_ER       73  // J12-6
  #define ENET_RXD1        75  // J12-8
#endif
#define ENET_MOC           70  // J12-3
#define REF_CLK            72  // J12-5
#define ENET_RXD0          74  // J12-7
#define ENET_CRS           76  // J12-9
#define ENET_TX_EN         77  // J12-10
#define ENET_TXD0          78  // J12-11
#define ENET_TXD1          79  // J12-12

/**
 *  PWMs
 *
 *  There are 6 PWMs.  Each PWM can be assigned to one of two pins.
 *
 *  SERVO2 does NOT have a PWM assigned to it.
 *
 *  PWM1.1   DIO4    SERVO3_PIN       FIL_RUNOUT_PIN   5V output, PWM
 *  PWM1.1   DIO26   E0_STEP_PIN
 *  PWM1.2   DIO11   SERVO0_PIN
 *  PWM1.2   DIO54   X_STEP_PIN
 *  PWM1.3   DIO6    SERVO1_PIN       J5-1
 *  PWM1.3   DIO60   Y_STEP_PIN
 *  PWM1.4   DIO53   SDSS(SSEL0)      J3-5  AUX-3
 *  PWM1.4   DIO46   Z_STEP_PIN
 *  PWM1.5   DIO3    X_MIN_PIN        10K PULLUP TO 3.3v, 1K SERIES
 *  PWM1.5   DIO9    RAMPS_D9_PIN
 *  PWM1.6   DIO14   Y_MIN_PIN        10K PULLUP TO 3.3v, 1K SERIES
 *  PWM1.6   DIO10   RAMPS_D10_PIN
 */

 /**
  * Special pins
  *   D37 - not 5V tolerant
  *   D49 - not 5V tolerant
  *   D57 - open collector
  *   D58 - open collector
  *
  */
