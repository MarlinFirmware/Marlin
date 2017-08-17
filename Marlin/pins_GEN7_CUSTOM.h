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
 * Gen7 Alfons3 board pin assignments
 *
 * These Pins are assigned for the modified GEN7 Board from Alfons3.
 * Please review the pins and adjust them for your needs.
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
  #error Oops!  Make sure you have 'Gen7' selected from the 'Tools -> Boards' menu.
#endif

//x axis pins
#define X_STEP_PIN      21                  // different from standard GEN7
#define X_DIR_PIN       20                  // different from standard GEN7
#define X_ENABLE_PIN    24
#define X_STOP_PIN       0

//y axis pins
#define Y_STEP_PIN      23
#define Y_DIR_PIN       22
#define Y_ENABLE_PIN    24
#define Y_STOP_PIN       1

//z axis pins
#define Z_STEP_PIN      26
#define Z_DIR_PIN       25
#define Z_ENABLE_PIN    24
#define Z_STOP_PIN       2

//extruder pins
#define E0_STEP_PIN     28
#define E0_DIR_PIN      27
#define E0_ENABLE_PIN   24

#define TEMP_0_PIN       2
#define TEMP_1_PIN      -1
#define TEMP_2_PIN      -1
#define TEMP_BED_PIN     1   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)

#define HEATER_0_PIN     4
#define HEATER_1_PIN    -1
#define HEATER_2_PIN    -1
#define HEATER_BED_PIN   3  // (bed)

#define SDPOWER         -1
#define SDSS            31                  // SCL pin of I2C header || CS Pin for SD Card support
#define LED_PIN         -1

#define FAN_PIN         -1
#define PS_ON_PIN       19
//our pin for debugging.

#define DEBUG_PIN       -1

//our RS485 pins
//#define TX_ENABLE_PIN       12
//#define RX_ENABLE_PIN       13

#define BEEPER_PIN       -1
#define SD_DETECT_PIN    -1
#define SUICIDE_PIN      -1    //has to be defined; otherwise Power_off doesn't work

#define KILL_PIN -1
//Pins for 4bit LCD Support
#define LCD_PINS_RS 18
#define LCD_PINS_ENABLE 17
#define LCD_PINS_D4 16
#define LCD_PINS_D5 15
#define LCD_PINS_D6 13
#define LCD_PINS_D7 14

//buttons are directly attached
#define BTN_EN1 11
#define BTN_EN2 10
#define BTN_ENC 12  //the click

