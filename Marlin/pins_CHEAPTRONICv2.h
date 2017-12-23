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

/**
 * Cheaptronic v2.0 pin assignments
 * Built and sold by Michal Dyntar - RRO
 *          www.reprapobchod.cz
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "Cheaptronic v2.0"
#define LARGE_FLASH        true

//
// Limit Switches
//
#define X_MIN_PIN          30
#define X_MAX_PIN          31
#define Y_MIN_PIN          32
#define Y_MAX_PIN          33
#define Z_MIN_PIN          34
#define Z_MAX_PIN          35

//
// Steppers
//
#define X_STEP_PIN         17
#define X_DIR_PIN          16
#define X_ENABLE_PIN       48

#define Y_STEP_PIN         54
#define Y_DIR_PIN          47
#define Y_ENABLE_PIN       55

#define Z_STEP_PIN         57
#define Z_DIR_PIN          56
#define Z_ENABLE_PIN       62

#define E0_STEP_PIN        23
#define E0_DIR_PIN         22
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        26
#define E1_DIR_PIN         25
#define E1_ENABLE_PIN      27

#define E2_STEP_PIN        29
#define E2_DIR_PIN         28
#define E2_ENABLE_PIN      39

//
// Temperature sensors
//
#define TEMP_0_PIN         15
#define TEMP_1_PIN         13
#define TEMP_2_PIN         14
#define TEMP_3_PIN         11
#define TEMP_BED_PIN       12

//
// Heaters / Fans
//
#define HEATER_0_PIN       6
#define HEATER_1_PIN       7
#define HEATER_2_PIN       8
#define HEATER_BED_PIN     9

//
// LCD
//
#define LCD_PINS_RS        19
#define LCD_PINS_ENABLE    42
#define LCD_PINS_D4        18
#define LCD_PINS_D5        38
#define LCD_PINS_D6        41
#define LCD_PINS_D7        40

//
// SD CARD, ROTARY ENCODER, BEEPER
//
#define SDPOWER            -1
#define SDSS               53
#define SD_DETECT_PIN      49
#define BEEPER_PIN         44
#define BTN_EN1            11
#define BTN_EN2            12
#define BTN_ENC            43

//
// Other board specific pins
//
#define LED_PIN            13
#define SPINDLE_ENABLE_PIN  4
#define FAN_PIN             3
#define PS_ON_PIN          45
#define KILL_PIN           46

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN     37   // should be Analog Input (0-15)
#endif
