/**
 * Marlin 3D Printer Firmware
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
 * AZSMZ MINI pin assignments
 */

//#if !defined(TARGET_LPC1768)
#if DISABLED(IS_REARM)
  #error "Oops!  Make sure you have Re-Arm selected."
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "AZSMZ MINI"
#endif

#define LARGE_FLASH true

//
// Servos
//
#define SERVO0_PIN         53

//
// Limit Switches
//
#define X_MIN_PIN           3
#define Y_MIN_PIN          14
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

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

//
// Temperature Sensors
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN          0  // A0 (TH1)
#define TEMP_BED_PIN        1  // A1 (TH2)
#define TEMP_1_PIN          2  // A2 (TH3)

//
// Heaters / Fans
//
// EFB
#define HEATER_0_PIN        8
#define HEATER_BED_PIN     10
#define FAN_PIN             9
#define FAN1_PIN           63

#if ENABLED(AZSMZ_12864)
  #define BEEPER_PIN       37
  #define DOGLCD_A0        59
  #define DOGLCD_CS        41
  #define BTN_EN1          13
  #define BTN_EN2          15
  #define BTN_ENC          31
  #define LCD_SDSS         16
  #define SD_DETECT_PIN    33
#endif

//
// Ethernet pins
//
#define ENET_MDIO          71
#define ENET_RX_ER         73
#define ENET_RXD1          75
#define ENET_MOC           70
#define REF_CLK            72
#define ENET_RXD0          74
#define ENET_CRS           76
#define ENET_TX_EN         77
#define ENET_TXD0          78
#define ENET_TXD1          79

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
  */
