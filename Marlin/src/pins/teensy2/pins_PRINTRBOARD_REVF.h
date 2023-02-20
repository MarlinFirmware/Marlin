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
 *  Schematic (RevF): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.F/schematic.png
 *  Origin (RevF): https://github.com/lwalkera/printrboard/raw/revF/Printrboard.sch
 *  Schematic (RevF2): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.F2/schematic.png
 *  Origin (RevF2): https://raw.githubusercontent.com/lwalkera/printrboard/revF2/Printrboard.sch
 *  Schematic (RevF3): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.F3/schematic.png
 *  Origin (RevF3): https://raw.githubusercontent.com/lwalkera/printrboard/revF3/Printrboard.sch
 *  Schematic (RevF4): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.F4/schematic.png
 *  Origin (RevF4): https://raw.githubusercontent.com/lwalkera/printrboard/revF4/Printrboard.sch
 *  Schematic (RevF5): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.F5/schematic.png
 *  Origin (RevF5): https://raw.githubusercontent.com/lwalkera/printrboard/revF5/Printrboard.sch
 *  Schematic (RevF6): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Printrboard%20Rev.F6/schematic.png
 *  Origin (RevF6): https://raw.githubusercontent.com/lwalkera/printrboard/revF6/Printrboard.sch
 */

/**
 *  There are two Arduino IDE extensions that are compatible with this board
 *  and with the mainstream Marlin software.
 *
 *  Teensyduino -  https://www.pjrc.com/teensy/teensyduino.html
 *  Installation - https://www.pjrc.com/teensy/td_download.html
 *
 *    Select Teensy++ 2.0 in Arduino IDE from the 'Tools > Board' menu
 *
 *    Note: With Teensyduino extension, the Arduino IDE will report 130048 bytes of program storage space available,
 *    but there is actually only 122880 bytes due to the larger DFU bootloader shipped by default on all Printrboard RevF.
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
 *  Teensyduino is the most popular and easiest option.
 */

/**
 *  To burn the bootloader that comes with Printrboard HID extension:
 *
 *   1. Connect your programmer to the board.
 *   2. In the Arduino IDE select "Printrboard" and then select the programmer.
 *   3. In the Arduino IDE click on "burn bootloader". Don't worry about the "verify failed at 1F000" error message.
 *   4. The programmer is no longer needed. Remove it.
 */

#include "env_validate.h"

#if !defined(__MARLIN_DEPS__) && !defined(USBCON)
  #error "USBCON should be defined by the platform for this board."
#endif

#define BOARD_INFO_NAME "Printrboard Rev.F"

#define AVR_CHIPOSCILLATOR_FREQ 16000000

// Disable JTAG pins so EXP1 pins work correctly
// (Its pins are used for the Extrudrboard and filament sensor, for example).
#define DISABLE_JTAG

/**
 * Note that REV F6 of the Printrboard stole the A HOTEND pin and
 * reassigned it to a second fan for the extruder heater.  It's
 * recommended that you swap the A and B outputs on the Extrudrboard
 * so EXTRUDERS=2 will still work on F6, using B for E1/HEATER_1/TEMP_1.
 * See https://printrbot.zendesk.com/hc/en-us/articles/115003072346
 *
 * If you have REV F6 you probably also want to set E0_AUTO_FAN_PIN
 * to PRINTRBOARD_F6_HEATER_FAN_PIN (44).
 *
 * Define NO_EXTRUDRBOARD if you don't have an EXTRUDRBOARD and wish to
 * reassign different functions to EXP1.
 *
 * Define NO_EXTRUDRBOARD_OUTPUT_SWAP if you have a REV F5 or lower and
 * want to use EXTRUDRBOARD A for E1 and EXTRUDRBOARD B for E2.
 */
//#define NO_EXTRUDRBOARD
//#define NO_EXTRUDRBOARD_OUTPUT_SWAP

//
// Limit Switches
//
#define X_STOP_PIN                         PinE3  // E3
#define Y_STOP_PIN                         PinB4  // B4 PWM2A
#define Z_STOP_PIN                         PinE4  // E4

//
// Steppers
//
#define X_STEP_PIN                         PinA0  // A0
#define X_DIR_PIN                          PinA1  // A1
#define X_ENABLE_PIN                       PinE7  // E7

#define Y_STEP_PIN                         PinA2  // A2
#define Y_DIR_PIN                          PinA3  // A3
#define Y_ENABLE_PIN                       PinE6  // E6

#define Z_STEP_PIN                         PinA4  // A4
#define Z_DIR_PIN                          PinA5  // A5
#define Z_ENABLE_PIN                       PinC7  // C7

#define E0_STEP_PIN                        PinA6  // A6
#define E0_DIR_PIN                         PinA7  // A7
#define E0_ENABLE_PIN                      PinC3  // C3

#if DISABLED(NO_EXTRUDRBOARD)
#if DISABLED(NO_EXTRUDRBOARD_OUTPUT_SWAP)
  #define E1_STEP_PIN                      PinB5  // B5
  #define E1_DIR_PIN                       PinE5  // E5
  #define E1_ENABLE_PIN                    PinF4  // F4

  #define E2_STEP_PIN                      PinD2  // D2
  #define E2_DIR_PIN                       PinD3  // D3
  #define E2_ENABLE_PIN                    PinF5  // F5
#else
  #define E1_STEP_PIN                      PinD2  // D2
  #define E1_DIR_PIN                       PinD3  // D3
  #define E1_ENABLE_PIN                    PinF5  // F5

  #define E2_STEP_PIN                      PinB5  // B5
  #define E2_DIR_PIN                       PinE5  // E5
  #define E2_ENABLE_PIN                    PinF4  // F4
#endif
#endif // NO_EXTRUDRBOARD

// Enable control of stepper motor currents with the I2C based MCP4728 DAC used on Printrboard REVF
#define HAS_MOTOR_CURRENT_DAC                  1

// Set default drive strength percents if not already defined - X, Y, Z, E axis
#ifndef DAC_MOTOR_CURRENT_DEFAULT
  #define DAC_MOTOR_CURRENT_DEFAULT { 70, 70, 50, 70 }
#endif

// Number of channels available for DAC
#define DAC_STEPPER_ORDER { 3, 2, 1, 0 }

#define DAC_STEPPER_SENSE    0.11
#define DAC_STEPPER_ADDRESS                    0
#define DAC_STEPPER_MAX                      3520
#define DAC_STEPPER_VREF                       1  // internal Vref, gain 1x = 2.048V
#define DAC_STEPPER_GAIN                       0
#define DAC_OR_ADDRESS                      0x00

//
// Temperature Sensors
//
#define TEMP_0_PIN                         PinD1  // Analog Input (Extruder)
#define TEMP_BED_PIN                       PinD0  // Analog Input (Bed)

#if DISABLED(NO_EXTRUDRBOARD)
#if DISABLED(NO_EXTRUDRBOARD_OUTPUT_SWAP)
  #define TEMP_1_PIN                       PinD2  // Analog Input (Extrudrboard A THERM)
  #define TEMP_2_PIN                       PinD3  // Analog Input (Extrudrboard B THERM)
#else
  #define TEMP_1_PIN                       PinD3  // Analog Input (Extrudrboard B THERM)
  #define TEMP_2_PIN                       PinD2  // Analog Input (Extrudrboard A THERM)
#endif
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN                       PinC5  // C5 PWM3B - Extruder
#define HEATER_BED_PIN                     PinC4  // C4 PWM3C

#if DISABLED(NO_EXTRUDRBOARD)
#if DISABLED(NO_EXTRUDRBOARD_OUTPUT_SWAP)
  #define HEATER_1_PIN                     PinF6  // F6 - Extrudrboard A HOTEND
  #define HEATER_2_PIN                     PinF7  // F7 - Extrudrboard B HOTEND
#else
  #define HEATER_1_PIN                     PinF7  // F7 - Extrudrboard B HOTEND
  #define HEATER_2_PIN                     PinF6  // F6 - Extrudrboard A HOTEND
#endif
#endif

#ifndef FAN_PIN
  #define FAN_PIN                          PinC6  // C6 PWM3A
#endif

//
// LCD / Controller
//
//#define USE_INTERNAL_SD

#if HAS_WIRED_LCD
  #define LCD_PINS_RS                      PinE1  // E1       JP11-11
  #define LCD_PINS_ENABLE                  PinE0  // E0       JP11-10
  #define LCD_PINS_D4                      PinD7  // D7       JP11-8
  #define LCD_PINS_D5                      PinD6  // D6       JP11-7
  #define LCD_PINS_D6                      PinD5  // D5       JP11-6
  #define LCD_PINS_D7                      PinD4  // D4       JP11-5

  #if EITHER(VIKI2, miniVIKI)

    #define BEEPER_PIN                     PinE0  // E0       JP11-10
    #define DOGLCD_A0                      PinF2  // F2       JP2-2
    #define DOGLCD_CS                      PinF3  // F3       JP2-4

    #define BTN_EN1                        PinD2  // D2 TX1   JP2-5
    #define BTN_EN2                        PinD3  // D3 RX1   JP2-7
    #define BTN_ENC                        PinF7  // F7 TDI   JP2-12

    #define SDSS                           PinD3  // F5 TMS   JP2-8

    #define STAT_LED_RED_PIN               PinC2  // C2       JP11-14
    #define STAT_LED_BLUE_PIN              PinC0  // C0       JP11-12

    #define LCD_SCREEN_ROTATE                180  // 0, 90, 180, 270

  #elif ENABLED(MINIPANEL)

    #if DISABLED(USE_INTERNAL_SD)
      //      PIN       FASTIO PIN#  ATUSB90 PIN# Teensy2.0++ PIN#  Printrboard RevF Conn.   MKSLCD12864 PIN#
      #define SDSS                         PinC1  //      36               C1                EXP2-13             EXP2-07
      #define SD_DETECT_PIN                PinE1  //      34               E1                EXP2-11             EXP2-04
    #endif

    //      PIN       FASTIO PIN#  ATUSB90 PIN# Teensy2.0++ PIN#  Printrboard RevF Conn.   MKSLCD12864 PIN#
    #define DOGLCD_A0                      PinD4  //       29               D4                EXP2-05             EXP1-04
    #define DOGLCD_CS                      PinD5  //       30               D5                EXP2-06             EXP1-05
    #define BTN_ENC                        PinD6  //       31               D6                EXP2-07             EXP1-09
    #define BEEPER_PIN                     PinD7  //       32               D7                EXP2-08             EXP1-10
    #define KILL_PIN                       PinE0  //       33               E0                EXP2-10             EXP2-03
    #define BTN_EN1                        PinC0  //       35               C0                EXP2-12             EXP2-06
    #define BTN_EN2                        PinC2  //       37               C2                EXP2-14             EXP2-08
    //#define LCD_BACKLIGHT_PIN            PinF5  //    56               F5                EXP1-12     Not Implemented
    //#define SCK                          PinB1  //         11               B1                ICSP-04             EXP2-09
    //#define MOSI                         PinB2  //         12               B2                ICSP-03             EXP2-05
    //#define MISO                         PinB3  //         13               B3                ICSP-06             EXP2-05

    // Alter timing for graphical display
    #define BOARD_ST7920_DELAY_1             313
    #define BOARD_ST7920_DELAY_2             313
    #define BOARD_ST7920_DELAY_3             313

  #else

    #define BTN_EN1                        PinC0  // C0       JP11-12
    #define BTN_EN2                        PinC1  // C1       JP11-13
    #define BTN_ENC                        PinC2  // C2       JP11-14

  #endif

#endif

//
// Misc. Functions
//
//      PIN       FASTIO PIN#  ATUSB90 PIN# Teensy2.0++ PIN#  Printrboard RevF Conn.
#ifndef SDSS
  #define SDSS                             PinB0  //       10               B0
#endif

/**
 * This is EXP1-2, which is also the TEMP_A_PIN for the Extrudrboard.
 * If using w/ Extrudrboard, cut off pin 2 on the Extrudrboard male
 * connector to ensure this is disconnected from the A THERM pullups.
 * You probably want to set EXTRUDERS=2 and swap the Extrudrboard outputs,
 * which will let you use Channel B on the Extrudrboard as E1.
 */
#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                     PinD2  // Analog Input
#endif
