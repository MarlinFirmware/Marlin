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
 *  Rev B  2 JUN 2017
 *
 *  Converted to Arduino pin numbering
 */

/**
 *  There are two Arduino IDE extensions that are compatible with this board
 *  and with the mainstream Marlin software.
 *
 *  Teensyduino - http://www.pjrc.com/teensy/teensyduino.html
 *    Installation instructions are at the above URL.
 *    
 *    Select Teensy++ 2.0 in Arduino IDE from the 'Tools -> Boards' menu
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
 *       4. Select "Printrboard" from the 'Tools -> Boards' menu.
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

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0' or 'Printrboard' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "Printrboard Rev F"
#define LARGE_FLASH        true

//
// Limit Switches
//
#define X_STOP_PIN         47   // E3
#define Y_STOP_PIN         24   // B4 PWM2A
#define Z_STOP_PIN         36   // E4

//
// Steppers
//
#define X_STEP_PIN         28   // A0
#define X_DIR_PIN          29   // A1
#define X_ENABLE_PIN       19   // E7

#define Y_STEP_PIN         30   // A2
#define Y_DIR_PIN          31   // A3
#define Y_ENABLE_PIN       18   // E6

#define Z_STEP_PIN         32   // A4
#define Z_DIR_PIN          33   // A5
#define Z_ENABLE_PIN       17   // C7

#define E0_STEP_PIN        34   // A6
#define E0_DIR_PIN         35   // A7
#define E0_ENABLE_PIN      13   // C3

// Enable control of stepper motor currents with the I2C based MCP4728 DAC used on Printrboard REVF
#define DAC_STEPPER_CURRENT

// Set default drive strength percents if not already defined - X, Y, Z, E axis
#ifndef DAC_MOTOR_CURRENT_DEFAULT 
  #define DAC_MOTOR_CURRENT_DEFAULT { 70, 70, 50, 70 }    
#endif

// Number of channels available for DAC
#define DAC_STEPPER_ORDER { 3, 2, 1, 0 }

#define DAC_STEPPER_SENSE    0.11
#define DAC_STEPPER_ADDRESS  0
#define DAC_STEPPER_MAX   3520
#define DAC_STEPPER_VREF     1   // internal Vref, gain 1x = 2.048V
#define DAC_STEPPER_GAIN     0
#define DAC_OR_ADDRESS    0x00

//
// Temperature Sensors
//
#define TEMP_0_PIN          1   // Analog Input (Extruder)
#define TEMP_BED_PIN        0   // Analog Input (Bed)

//
// Heaters / Fans
//
#define HEATER_0_PIN       15   // C5 PWM3B - Extruder
#define HEATER_1_PIN       44   // F6
#define HEATER_2_PIN       45   // F7
#define HEATER_BED_PIN     14   // C4 PWM3C

#define FAN_PIN            16   // C6 PWM3A

//
// LCD / Controller
//
//#define USE_INTERNAL_SD

#if ENABLED(ULTRA_LCD)
  #define BEEPER_PIN       -1

  #define LCD_PINS_RS       9   // E1       JP11-11
  #define LCD_PINS_ENABLE   8   // E0       JP11-10
  #define LCD_PINS_D4       7   // D7       JP11-8
  #define LCD_PINS_D5       6   // D6       JP11-7
  #define LCD_PINS_D6       5   // D5       JP11-6
  #define LCD_PINS_D7       4   // D4       JP11-5

  #define BTN_EN1          10   // C0       JP11-12
  #define BTN_EN2          11   // C1       JP11-13
  #define BTN_ENC          12   // C2       JP11-14

  #define SD_DETECT_PIN    -1
#endif

#if ENABLED(VIKI2) || ENABLED(miniVIKI)
  #define BEEPER_PIN        8   // E0       JP11-10
  #define DOGLCD_A0        40   // F2       JP2-2
  #define DOGLCD_CS        41   // F3       JP2-4
  #define LCD_SCREEN_ROT_180

  #define BTN_EN1           2   // D2 TX1   JP2-5
  #define BTN_EN2           3   // D3 RX1   JP2-7
  #define BTN_ENC          45   // F7 TDI   JP2-12

  #define SDSS             43   // F5 TMS   JP2-8
  #define SD_DETECT_PIN    -1

  #define STAT_LED_RED_PIN  12  // C2       JP11-14
  #define STAT_LED_BLUE_PIN 10  // C0       JP11-12
#endif

#if ENABLED(MINIPANEL)
  #if ENABLED(USE_INTERNAL_SD)
    //      PIN       FASTIO PIN#  ATUSB90 PIN# Teensy2.0++ PIN# 
    #define SDSS               20  //        10               B0  
    #define SD_DETECT_PIN      -1  // no auto-detect SD insertion on built-in Printrboard SD reader
  #else
    //      PIN       FASTIO PIN#  ATUSB90 PIN# Teensy2.0++ PIN#  Printrboard RevF Conn.   MKSLCD12864 PIN#  
    #define SDSS               11  //        36               C1                EXP2-13             EXP2-07
    #define SD_DETECT_PIN       9  //        34               E1                EXP2-11             EXP2-04
  #endif
  
    //      PIN       FASTIO PIN#  ATUSB90 PIN# Teensy2.0++ PIN#  Printrboard RevF Conn.   MKSLCD12864 PIN#  
    #define DOGLCD_A0           4  //        29               D4                EXP2-05             EXP1-04      
    #define DOGLCD_CS           5  //        30               D5                EXP2-06             EXP1-05
    #define BTN_ENC             6  //        31               D6                EXP2-07             EXP1-09
    #define BEEPER_PIN          7  //        32               D7                EXP2-08             EXP1-10
    #define KILL_PIN            8  //        33               E0                EXP2-10             EXP2-03
    #define BTN_EN1            10  //        35               C0                EXP2-12             EXP2-06
    #define BTN_EN2            12  //        37               C2                EXP2-14             EXP2-08
  //#define LCD_BACKLIGHT_PIN  43  //        56               F5                EXP1-12     Not Implemented
  //#define SCK                21  //        11               B1                ICSP-04             EXP2-09
  //#define MOSI               22  //        12               B2                ICSP-03             EXP2-05
  //#define MISO               23  //        13               B3                ICSP-06             EXP2-05            
  
  // encoder connections present
  #define BLEN_A 0
  #define BLEN_B 1
  #define BLEN_C 2

  // encoder rotation values
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1

  // increase delays to max
  #define ST7920_DELAY_1 DELAY_5_NOP
  #define ST7920_DELAY_2 DELAY_5_NOP
  #define ST7920_DELAY_3 DELAY_5_NOP
#endif

//
// Misc. Functions
//
#ifndef SDSS
  #define SDSS               20   // B0 SS
#endif

#define FILWIDTH_PIN        2   // Analog Input
