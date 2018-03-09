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
 * Gen7 v1.4 pin assignments
 */

#if !defined(__AVR_ATmega2560__) && !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
  #error "Oops!  Make sure you have 'Gen7' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME "Gen7 v1.4"

#define GEN7_VERSION 14 // v1.4

#define KNOWN_BOARD 1

#define LARGE_FLASH        true

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38 
#define X_MIN_PIN           3
#define X_MAX_PIN          -1 // 2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14 
#define Y_MAX_PIN          -1 // 15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          -1 // 19

#define Z2_STEP_PIN        36
#define Z2_DIR_PIN         34
#define Z2_ENABLE_PIN      30

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30

#define SDPOWER            -1
#define SDSS               53
//#define LED_PIN            -1

#define FAN_PIN            9// D8 for dual extruder 131227

#define PS_ON_PIN          12

#define FIL_RUNOUT_PIN 15
#define FIL_RUNOUT2_PIN 2

//#define LASER_PIN   -1

#define HEATER_0_PIN       7   // EXTRUDER 1
#define HEATER_1_PIN       10    // EXTRUDER 2 (FAN On Sprinter)

#define TEMP_0_PIN         3   // ANALOG NUMBERING
#define TEMP_1_PIN         4 //14   // ANALOG NUMBERING

#define HEATER_BED_PIN     8    // was 14 or 15 BED
#define TEMP_BED_PIN       14   // ANALOG NUMBERING

#define SERVO0_PIN         11
//#define SERVO1_PIN         -1
//#define SERVO2_PIN         -1
//#define SERVO3_PIN         -1

#ifdef ULTRA_LCD
  #ifdef NEWPANEL
       #define BEEPER_PIN 37
              #define DOGLCD_A0  27
       #define DOGLCD_CS  25
       #define LCD_BACKLIGHT_PIN 65  // backlight LED on A11/D65
       #define SDSS   53
       
       #define KILL_PIN 64
       // GLCD features
       //#define LCD_CONTRAST 190
       // Uncomment screen orientation
         //#define LCD_SCREEN_ROT_90
         //#define LCD_SCREEN_ROT_180
         //#define LCD_SCREEN_ROT_270
       //The encoder and click button
       
       #define BTN_EN1 31
       #define BTN_EN2 33
       #define BTN_ENC 35  //the click switch
       #define SD_DETECT_PIN 49
    #endif
#endif //ULTRA_LCD

//Test----------------------
#define FIL_RUNOUT3_PIN 15
#define FIL_RUNOUT4_PIN 2
#define FIL_RUNOUT5_PIN 2

#define E2_STEP_PIN        36
#define E2_DIR_PIN         34
#define E2_ENABLE_PIN      30

#define E3_STEP_PIN        36
#define E3_DIR_PIN         34
#define E3_ENABLE_PIN      30

#define E4_STEP_PIN        36
#define E4_DIR_PIN         34
#define E4_ENABLE_PIN      30

#define TEMP_2_PIN         3   // ANALOG NUMBERING
#define TEMP_3_PIN         3   // ANALOG NUMBERING
#define TEMP_4_PIN         3

#define HEATER_2_PIN       7
#define HEATER_3_PIN       7
#define HEATER_4_PIN       7

//Test----------------------
