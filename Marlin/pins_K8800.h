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
 * Ultimaker pin assignments
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME    "Vertex Delta"
#define DEFAULT_SOURCE_CODE_URL "https://github.com/Psychokiller1888/Vertex-Delta-Firmware"
#define BOARD_NAME              "K8800 MOTHERBOARD"

//#define LCD_SCREEN_ROT_180

#define LARGE_FLASH true

#define X_STEP_PIN          54
#define X_DIR_PIN           55
#define X_MIN_PIN           -1
#define X_MAX_PIN            3
#define X_ENABLE_PIN        38

#define Y_STEP_PIN          60
#define Y_DIR_PIN           61
#define Y_MIN_PIN           -1
#define Y_MAX_PIN           14
#define Y_ENABLE_PIN        56

#define Z_STEP_PIN          46
#define Z_DIR_PIN           48
#define Z_MIN_PIN           68 //USED FOR BED LEVELING
#define Z_MAX_PIN           66
#define Z_ENABLE_PIN        63

#define HEATER_0_PIN        10 //EXTRUDER 1
#define TEMP_0_PIN          13

#define E0_STEP_PIN         26
#define E0_DIR_PIN          28
#define E0_ENABLE_PIN       24

#define E1_STEP_PIN         32
#define E1_DIR_PIN          34
#define E1_ENABLE_PIN       30

#define FAN_PIN              8 //FILAMENT FAN
#define FAN1_PIN             9 //CONTROLLER FAN

#define SDSS                25

#define FIL_RUNOUT_PIN      69 //PK7
#define KILL_PIN            20 //PD1

#define LED_PIN             -1
#define PS_ON_PIN           -1
#define SUICIDE_PIN         -1

#define LCD_SDSS            53
#define DOGLCD_CS           29
#define DOGLCD_A0           27

#define BEEPER_PIN        6

#define LCD_PINS_RS       27
#define LCD_PINS_ENABLE   29
#define LCD_PINS_D4       37
#define LCD_PINS_D5       35
#define LCD_PINS_D6       33
#define LCD_PINS_D7       31

#define BTN_EN1           17
#define BTN_EN2           16
#define BTN_ENC           23 //BUTTON

#define SD_DETECT_PIN     21 //PD0
