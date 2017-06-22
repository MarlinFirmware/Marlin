/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef __PINMAP_RE_ARM_H__
#define __PINMAP_RE_ARM_H__

// ******************
// Runtime pinmapping
// ******************

const adc_pin_data adc_pin_map[] = {
  {0xFF, 0xFF, 0xFF}, //A0
  {0xFF, 0xFF, 0xFF}, //A1
  {0xFF, 0xFF, 0xFF}, //A2
  {0xFF, 0xFF, 0xFF}, //A3
  {0xFF, 0xFF, 0xFF}, //A4
  {0xFF, 0xFF, 0xFF}, //A5
  {0xFF, 0xFF, 0xFF}, //A6
  {0xFF, 0xFF, 0xFF}, //A7
  {0xFF, 0xFF, 0xFF}, //A8
  {0xFF, 0xFF, 0xFF}, //A9
  {0xFF, 0xFF, 0xFF}, //A10
  {0xFF, 0xFF, 0xFF}, //A11
  {0xFF, 0xFF, 0xFF}, //A12
  {0, 23, 0}, //A13 - TEMP_0_PIN
  {0, 24, 1}, //A14 - TEMP_BED_PIN
  {0, 25, 2}  //A15 - TEMP_1_PIN
};

const pin_data pin_map[] = { // pin map for variable pin function
  {0,  2}, //DIO0  - TXD0
  {0,  3}, //DIO1  - RXD0
  {1, 25}, //DIO2  - X_MAX_PIN
  {1, 24}, //DIO3  - X_MIN_PIN
  {0xFF,0xFF}, //DIO4  - SERVO3_PIN - FIL_RUNOUT_PIN
  {0xFF,0xFF}, //DIO5  - SERVO2_PIN
  {0xFF,0xFF}, //DIO6  - SERVO1_PIN
  {0xFF,0xFF}, //DIO7  -
  {2,  7}, //DIO8  - RAMPS_D8_PIN
  {2,  4}, //DIO9  - RAMPS_D9_PIN
  {2,  5}, //DIO10 - RAMPS_D10_PIN
  {0xFF,0xFF}, //DIO11 -
  {2, 12}, //DIO12 - PS_ON_PIN
  {4, 28}, //DIO13 - LED_PIN
  {1, 26}, //DIO14 - Y_MIN_PIN
  {1, 27}, //DIO15 - Y_MAX_PIN
  {0, 16}, //DIO16 - LCD_PINS_RS
  {0, 18}, //DIO17 - LCD_PINS_ENABLE
  {1, 29}, //DIO18 - Z_MIN_PIN
  {1, 28}, //DIO19 - Z_MAX_PIN
  {0xFF,0xFF}, //DIO20
  {0xFF,0xFF}, //DIO21
  {0xFF,0xFF}, //DIO22
  {0, 15}, //DIO23 - LCD_PINS_D4
  {0,  4}, //DIO24 - E0_ENABLE_PIN
  {0xFF,0xFF}, //DIO25
  {2,  0}, //DIO26 - E0_STEP_PIN
  {0xFF,0xFF}, //DIO27
  {0,  5}, //DIO28 - E0_DIR_PIN
  {0xFF,0xFF}, //DIO29
  {4, 29}, //DIO30 - E1_ENABLE_PIN
  {3, 26}, //DIO31 - BTN_EN1
  {0xFF,0xFF}, //DIO32
  {3, 25}, //DIO33 - BTN_EN2
  {2, 13}, //DIO34 - E1_DIR_PIN
  {2, 11}, //DIO35 - BTN_ENC
  {2,  8}, //DIO36 - E1_STEP_PIN
  {1, 30}, //DIO37 - BEEPER_PIN
  {0, 10}, //DIO38 - X_ENABLE_PIN
  {0xFF,0xFF}, //DIO39
  {0xFF,0xFF}, //DIO40
  {1, 22}, //DIO41 - KILL_PIN
  {0xFF,0xFF}, //DIO42
  {0xFF,0xFF}, //DIO43
  {0xFF,0xFF}, //DIO44
  {0xFF,0xFF}, //DIO45
  {2,  3}, //DIO46 - Z_STEP_PIN
  {0xFF,0xFF}, //DIO47
  {0, 22}, //DIO48 - Z_DIR_PIN
  {1, 31}, //DIO49 - SD_DETECT_PIN
  {0, 17}, //DIO50 - MISO_PIN
  {0, 18}, //DIO51 - MOSI_PIN
  {0, 15}, //DIO52 - SCK_PIN
  {1, 23}, //DIO53 - SDSS
  {2,  1}, //DIO54 - X_STEP_PIN
  {0, 11}, //DIO55 - X_DIR_PIN
  {0, 19}, //DIO56 - Y_ENABLE_PIN
  {0xFF,0xFF}, //DIO57
  {0xFF,0xFF}, //DIO58
  {0xFF,0xFF}, //DIO59
  {2,  2}, //DIO60 - Y_STEP_PIN
  {0, 20}, //DIO61 - Y_DIR_PIN
  {0, 21}, //DIO62 - Z_ENABLE_PIN
  {0xFF,0xFF}, //DIO63
  {0xFF,0xFF}, //DIO64
  {0xFF,0xFF}, //DIO65
  {0xFF,0xFF}, //DIO66
  {0xFF,0xFF}, //DIO67
  {0xFF,0xFF}, //DIO68
  {0xFF,0xFF}, //DIO69
};

// ***********************
// Preprocessor pinmapping
// ***********************

// #define X_MAX_PIN          2
#define DIO2_PORT 1
#define DIO2_PIN 25
// #define X_MIN_PIN          3
#define DIO3_PORT 1
#define DIO3_PIN 24
// #define Y_MIN_PIN          14
#define DIO14_PORT 1
#define DIO14_PIN 26
// #define Y_MAX_PIN          15
#define DIO15_PORT 1
#define DIO15_PIN 27
// #define Z_MIN_PIN          18
#define DIO18_PORT 1
#define DIO18_PIN 29
// #define Z_MAX_PIN          19
#define DIO19_PORT 1
#define DIO19_PIN 28
// #define X_STEP_PIN         54
#define DIO54_PORT 2
#define DIO54_PIN 1
// #define X_DIR_PIN          55
#define DIO55_PORT 0
#define DIO55_PIN 11
// #define X_ENABLE_PIN       38
#define DIO38_PORT 0
#define DIO38_PIN 10
// #define Y_STEP_PIN         60
#define DIO60_PORT 2
#define DIO60_PIN 2
// #define Y_DIR_PIN          61
#define DIO61_PORT 0
#define DIO61_PIN 20
// #define Y_ENABLE_PIN       56
#define DIO56_PORT 0
#define DIO56_PIN 19
// #define Z_STEP_PIN         46
#define DIO46_PORT 2
#define DIO46_PIN 3
// #define Z_DIR_PIN          48
#define DIO48_PORT 0
#define DIO48_PIN 22
// #define Z_ENABLE_PIN       62
#define DIO62_PORT 0
#define DIO62_PIN 21
// #define E0_STEP_PIN        26
#define DIO26_PORT 2
#define DIO26_PIN 0
// #define E0_DIR_PIN         28
#define DIO28_PORT 0
#define DIO28_PIN 5
// #define E0_ENABLE_PIN      24
#define DIO24_PORT 0
#define DIO24_PIN 4
// #define E1_STEP_PIN        36
#define DIO36_PORT 2
#define DIO36_PIN 8
// #define E1_DIR_PIN         34
#define DIO34_PORT 2
#define DIO34_PIN 13
// #define E1_ENABLE_PIN      30
#define DIO30_PORT 4
#define DIO30_PIN 29
// #define RAMPS_D8_PIN   8
#define DIO8_PORT 2
#define DIO8_PIN 7
// #define RAMPS_D9_PIN   9
#define DIO9_PORT 2
#define DIO9_PIN 4
// #define RAMPS_D10_PIN 10
#define DIO10_PORT 2
#define DIO10_PIN 5
// #define MISO          50
#define DIO50_PORT 0
#define DIO50_PIN 17
// #define MOSI          51
#define DIO51_PORT 0
#define DIO51_PIN 18
// #define SCK           52
#define DIO52_PORT 0
#define DIO52_PIN 15
// #define SDSS          53
#define DIO53_PORT 1
#define DIO53_PIN 23
// #define LED_PIN       13
#define DIO13_PORT 4
#define DIO13_PIN 28
// #define FIL_RUNOUT_PIN 4
#define DIO4_PORT -1
#define DIO4_PIN -1
// #define PS_ON_PIN   12
#define DIO12_PORT 2
#define DIO12_PIN 12
// #define LCD_PINS_RS 16
#define DIO16_PORT 0
#define DIO16_PIN 16
// #define LCD_PINS_ENABLE 17
#define DIO17_PORT 0
#define DIO17_PIN 18
// #define LCD_PINS_D4 23
#define DIO23_PORT 0
#define DIO23_PIN 15
// #define BEEPER_PIN 37
#define DIO37_PORT 1
#define DIO37_PIN 30
// #define BTN_EN1 31
#define DIO31_PORT 3
#define DIO31_PIN 26
// #define BTN_EN2 33
#define DIO33_PORT 3
#define DIO33_PIN 25
// #define BTN_ENC 35
#define DIO35_PORT 2
#define DIO35_PIN 11
// #define SD_DETECT_PIN 49
#define DIO49_PORT 1
#define DIO49_PIN 31
// #define KILL_PIN 41
#define DIO41_PORT 1
#define DIO41_PIN 22

#endif //__PINMAP_RE_ARM_H__
