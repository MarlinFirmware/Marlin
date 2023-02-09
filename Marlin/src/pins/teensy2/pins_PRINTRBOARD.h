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
 *  Rev B  2 JUN 2017
 *
 *  Converted to Arduino pin numbering
 * 
 *  Schematic (RevA): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.A/schematic.png
 *  Origin (RevA): https://raw.githubusercontent.com/lwalkera/printrboard/revA/Printrboard.sch
 *  Schematic (RevB): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.B/schematic.png
 *  Origin (RevB): https://raw.githubusercontent.com/lwalkera/printrboard/revB/Printrboard.sch
 *  Schematic (RevC): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.C/schematic.png
 *  Origin (RevC): https://raw.githubusercontent.com/lwalkera/printrboard/revC/Printrboard.sch
 *  Schematic (RevD): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.D/schematic.png
 *  Origin (RevD): https://raw.githubusercontent.com/lwalkera/printrboard/RevD/Printrboard.sch
 */

/**
 *  There are two Arduino IDE extensions that are compatible with this board
 *  and with the mainstream Marlin software.
 *
 *  Teensyduino - https://www.pjrc.com/teensy/teensyduino.html
 *    Select Teensy++ 2.0 in Arduino IDE from the 'Tools > Board' menu
 *
 *    Installation instructions are at the above URL.  Don't bother loading the
 *    libraries - they are not used with the Marlin software.
 *
 *  Printrboard - https://github.com/scwimbush/Printrboard-HID-Arduino-IDE-Support
 *
 *    Installation:
 *
 *       1. Go to the above URL, click on the "Clone or Download" button and then
 *          click on "Download ZIP" button.
 *       2. Unzip the file, find the "printrboard" directory and then copy it to the
 *          hardware directory in Arduino.  The Arduino hardware directory will probably
 *          be located in a path similar to this: C:\Program Files (x86)\Arduino\hardware.
 *       3. Restart Arduino.
 *       4. Select "Printrboard" from the 'Tools > Board' menu.
 *
 *  Teensyduino is the most popular option. Printrboard is used if your board doesn't have
 *  the Teensyduino bootloader on it.
 */

/**
 *  To burn the bootloader that comes with Printrboard:
 *
 *   1. Connect your programmer to the board.
 *   2. In the Arduino IDE select "Printrboard" and then select the programmer.
 *   3. In the Arduino IDE click on "burn bootloader". Don't worry about the "verify failed at 1F000" error message.
 *   4. The programmer is no longer needed. Remove it.
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Printrboard"

// Disable JTAG pins so they can be used for the Extrudrboard
#define DISABLE_JTAG

// Only for RevA.
//#define AVR_CHIPOSCILLATOR_FREQ 8000000

#define AVR_CHIPOSCILLATOR_FREQ 16000000

//
// Limit Switches
//
#define X_STOP_PIN                            PinE3  // E3
#define Y_STOP_PIN                            PinB0  // B0 SS
#define Z_STOP_PIN                            PinE4  // E4

//
// Steppers
//
#define X_STEP_PIN                            PinA0  // A0
#define X_DIR_PIN                             PinA1  // A1
#define X_ENABLE_PIN                          PinE7  // E7

#define Y_STEP_PIN                            PinA2  // A2
#define Y_DIR_PIN                             PinA3  // A3
#define Y_ENABLE_PIN                          PinE6  // E6

#define Z_STEP_PIN                            PinA4  // A4
#define Z_DIR_PIN                             PinA5  // A5
#define Z_ENABLE_PIN                          PinC7  // C7

#define E0_STEP_PIN                           PinA6  // A6
#define E0_DIR_PIN                            PinA7  // A7
#define E0_ENABLE_PIN                         PinC3  // C3

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinD1  // Analog Input
#define TEMP_BED_PIN                          PinD0  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinC5  // C5 PWM3B - Extruder
#define HEATER_1_PIN                          PinF6  // F6
#define HEATER_2_PIN                          PinF7  // F7
#define HEATER_BED_PIN                        PinC4  // C4 PWM3C

#ifndef FAN_PIN
  #define FAN_PIN                             PinC6  // C6 PWM3A
#endif

//
// Misc. Functions
//
#define FILWIDTH_PIN                          PinD2  // Analog Input

//
// LCD / Controller
//
#if HAS_WIRED_LCD && IS_NEWPANEL

  #define LCD_PINS_RS                         PinE1  // E1       JP11-11
  #define LCD_PINS_ENABLE                     PinE0  // E0       JP11-10
  #define LCD_PINS_D4                         PinD7  // D7       JP11-8
  #define LCD_PINS_D5                         PinD6  // D6       JP11-7
  #define LCD_PINS_D6                         PinD5  // D5       JP11-6
  #define LCD_PINS_D7                         PinD4  // D4       JP11-5

  #if EITHER(VIKI2, miniVIKI)
    #define BEEPER_PIN                        PinE0  // E0       JP11-10

    #define DOGLCD_A0                         PinF2  // F2       JP2-2
    #define DOGLCD_CS                         PinF3  // F3       JP2-4

    #define BTN_EN1                           PinD2  // D2 TX1   JP2-5
    #define BTN_EN2                           PinD3  // D3 RX1   JP2-7
    #define BTN_ENC                           PinF7  // F7 TDI   JP2-12

    #define SDSS                              PinF5  // F5 TMS   JP2-8

    #define STAT_LED_RED_PIN                  PinC2  // C2    JP11-14
    #define STAT_LED_BLUE_PIN                 PinC0  // C0    JP11-12

    #define LCD_SCREEN_ROTATE                180  // 0, 90, 180, 270

  #elif ENABLED(LCD_I2C_PANELOLU2)

    #define BTN_EN1                           PinD3  // D3 RX1   JP2-7
    #define BTN_EN2                           PinD2  // D2 TX1   JP2-5
    #define BTN_ENC                           PinF3  // F3       JP2-4

    #define SDSS                              PinF0  // F0       B-THERM connector - use SD card on Panelolu2

  #else

    #define BTN_EN1                           PinC0  // C0       JP11-12
    #define BTN_EN2                           PinC1  // C1       JP11-13
    #define BTN_ENC                           PinC2  // C2       JP11-14

  #endif

#endif // HAS_WIRED_LCD && IS_NEWPANEL

#ifndef SDSS
  #define SDSS                                PinB6  // B6 SDCS
#endif
