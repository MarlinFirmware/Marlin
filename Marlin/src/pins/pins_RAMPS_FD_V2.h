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
 * RAMPS-FD v2
 *
 * EEPROM supported
 * Use 1k thermistor tables
 */

#define BOARD_NAME "RAMPS-FD v2"

#ifndef E0_CS_PIN
  #define E0_CS_PIN        32   // assign to unused pin
#endif


#define TMC_USE_SW_SPI
#define TMC_SW_MOSI       58  // SDI on TMC2130
#define TMC_SW_MISO       68  // SDO on TMC2130
#define TMC_SW_SCK        60


#define X_CS_PIN         32   // assign to unused pin
#define Y_CS_PIN         32   // assign to unused pin
#define Y2_CS_PIN        32   // assign to unused pin // E1 socket
#define Z_CS_PIN         32   // assign to unused pin
#define Z2_CS_PIN        32   // assign to unused pin
#define E1_CS_PIN        32   // assign to unused pin
#define E2_CS_PIN        32   // assign to unused pin

#include "pins_RAMPS_FD_V1.h"

#undef INVERTED_HEATER_PINS
#undef INVERTED_BED_PINS
#undef INVERTED_FAN_PINS

#define I2C_EEPROM
#define E2END 0xFFFF // 64K in a 24C512

#ifndef PS_ON_PIN
//  #define PS_ON_PIN        12
#endif

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN      5   // Analog Input on AUX2
#endif


/**
 *
 * custom pin definitions for my printer
 *
 */

#undef  FILWIDTH_PIN
#undef  TEMP_BED_PIN            // normally 0   -  Analog Input

#undef  TEMP_0_PIN              // normally 1   -  Analog Input
#undef  TEMP_1_PIN              // normally 2   -  Analog Input
#undef  TEMP_2_PIN              // normally 3   -  Analog Input
#define TEMP_0_PIN         1    // normally 1

#define TEMP_BED_PIN       0    // normally 0

#undef  SERVO2_PIN
#define Z_MIN_PROBE_PIN    5    // use servo 2 pin because it's physically easier to connect to

#undef  HEATER_2_PIN            // use HEATER_2_PIN for extruder 0 fan
#undef  SERVO1_PIN              // use servo 1 pin for extruder fan
#undef  E0_AUTO_FAN_PIN
#define E0_AUTO_FAN_PIN    6
#define E0_FAN_RPM        56     // extruder PWM analog input (using temp1 input analog channel 2
#undef  FAN_PIN
#define FAN_PIN           11


/**
 *
 * L6470 pin changes
 *
 */
//
// Steppers
//
#undef  X_STEP_PIN
#define X_STEP_PIN         60   // L6470_0 on bottom board
//#undef  X_DIR_PIN               // not used in a L6470 system but Marlin still wants to see them
//#define X_DIR_PIN          32   // assign to unused pin
#undef  X_ENABLE_PIN            // not used in a L6470 system but Marlin still wants to see them// ELIMINATE?
#define X_ENABLE_PIN       32   // assign to unused pin

#undef  Y_STEP_PIN
#define Y_STEP_PIN         68   // L6470_0 on middle board
//#undef  Y_DIR_PIN               // not used in a L6470 system but Marlin still wants to see them
//#define Y_DIR_PIN          32   // assign to unused pin
#undef  Y_ENABLE_PIN            // not used in a L6470 system but Marlin still wants to see them    // ELIMINATE?
#define Y_ENABLE_PIN       32   // assign to unused pin

#undef  Z_STEP_PIN
#define Z_STEP_PIN         14   // L6470_0 on top board
//#undef  Z_DIR_PIN               // not used in a L6470 system but Marlin still wants to see them
//#define Z_DIR_PIN          32   // assign to unused pin
#undef  Z_ENABLE_PIN            // not used in a L6470 system but Marlin still wants to see them    // ELIMINATE?
#define Z_ENABLE_PIN       32   // assign to unused pin

#undef  E0_STEP_PIN
#define E0_STEP_PIN        59   // L6470_1 on bottom board
//#undef  E0_DIR_PIN              // not used in a L6470 system but Marlin still wants to see them
//#define E0_DIR_PIN         32   // assign to unused pin
#undef  E0_ENABLE_PIN           // not used in a L6470 system but Marlin still wants to see them    // ELIMINATE?
#define E0_ENABLE_PIN      32   // assign to unused pin

#undef  E1_STEP_PIN
#define E1_STEP_PIN        69   // L6470_1 on middle board
//#undef  E1_DIR_PIN              // not used in a L6470 system but Marlin still wants to see them
//#define E1_DIR_PIN         32   // assign to unused pin
#undef  E1_ENABLE_PIN           // not used in a L6470 system but Marlin still wants to see them    // ELIMINATE?
#define E1_ENABLE_PIN      32   // assign to unused pin

#undef  E2_STEP_PIN
#define E2_STEP_PIN        15   // L6470_1 on top board
//#undef  E2_DIR_PIN              // not used in a L6470 system but Marlin still wants to see them
//#define E2_DIR_PIN         32   // assign to unused pin
#undef  E2_ENABLE_PIN           // not used in a L6470 system but Marlin still wants to see them    // ELIMINATE?
#define E2_ENABLE_PIN      32   // assign to unused pin

//
// L6470 soft SPI pins
//
#define L6470_CHAIN_SCK_PIN   61
#define L6470_CHAIN_MISO_PIN  17
#define L6470_CHAIN_MOSI_PIN  16
#define L6470_CHAIN_SS_PIN    23



//
// LCD
//
#if ENABLED(ULTRA_LCD)
  // ramps-fd lcd adaptor

  #if ENABLED(NEWPANEL)
    #undef  LCD_PINS_RS
    #undef  LCD_PINS_ENABLE
    #undef  LCD_PINS_D4
    #define LCD_PINS_RS    39
    #define LCD_PINS_ENABLE 43
    #define LCD_PINS_D4    45

  #endif
#endif

//
// MISC
//
#define L6470_RESET_CHAIN_PIN    58   //  reset for entire chain


//
// Spindle/laser
//
#undef  SPINDLE_LASER_ENA_PIN
#define SPINDLE_LASER_ENA_PIN  70  // must be pulled up or could turn on when input is tristate (during power up init)
#undef  SPINDLE_LASER_PWM_PIN
#undef  SERVO3_PIN              // use servo 3 pin for spindle speed control
#define SPINDLE_LASER_PWM_PIN     3
