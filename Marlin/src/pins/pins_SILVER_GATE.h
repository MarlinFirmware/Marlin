/*
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

#if !defined(__AVR_ATmega1281__) && !defined(__AVR_ATmega2561__) 
  #error Oops!  Make sure you have 'Silvergate' selected from the 'Tools -> Boards' menu.
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "Silver Gate"
#endif

#define LARGE_FLASH true

#define X_STEP_PIN         43
#define X_DIR_PIN          44
#define X_ENABLE_PIN       42
#define X_MIN_PIN          31
#define X_MAX_PIN          34

#define Y_STEP_PIN         40
#define Y_DIR_PIN          41
#define Y_ENABLE_PIN       39
#define Y_MIN_PIN          32
#define Y_MAX_PIN          35

#define Z_STEP_PIN         13
#define Z_DIR_PIN          38
#define Z_ENABLE_PIN       14
#define Z_MIN_PIN          33
#define Z_MAX_PIN          36

#define E0_STEP_PIN        27
#define E0_DIR_PIN         37
#define E0_ENABLE_PIN      45

#define E1_STEP_PIN        -1
#define E1_DIR_PIN         -1
#define E1_ENABLE_PIN      -1

#define SDPOWER            -1
#define SDSS               16
#define LED_PIN            -1


#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  // define digital pin 34 for the filament runout sensor - normal Z-MAX. 
  #define FIL_RUNOUT_PIN   34
#endif

#define FAN_PIN             5
#define PS_ON_PIN          -1

#if ENABLED(SILVER_GATE_GLCD_CONTROLLER) 
  #define KILL_PIN         21//BUT9
  #define HOME_PIN         28//BUT2
#endif

#define HEATER_0_PIN        7
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1

#define E0_AUTO_FAN_PIN     3
#define CONTROLLER_FAN_PIN   2

#define TEMP_0_PIN          7// ANALOG NUMBERING
#define TEMP_1_PIN         -1// ANALOG NUMBERING
#define TEMP_2_PIN         -1// ANALOG NUMBERING

#define HEATER_BED_PIN      8// BED
#define TEMP_BED_PIN        6// ANALOG NUMBERING

#if ENABLED(DOGLCD)
  #if ENABLED(U8GLIB_ST7920)       // SPI GLCD 12864 ST7920 
    #define LCD_PINS_RS     30
    #define LCD_PINS_ENABLE 20
    #define LCD_PINS_D4     25
    #define BEEPER_PIN      29
    #define BTN_EN1         19
    #define BTN_EN2         22
    #define BTN_ENC         24
    #define LCD_BACKLIGHT_PIN  6
  #endif
#endif

#define SD_DETECT_PIN       15
 
#define STAT_LED_RED_PIN      23//red
#define STAT_LED_BLUE_PIN     26//blue
//#define CASE_LIGHT_PIN      51// case light 
