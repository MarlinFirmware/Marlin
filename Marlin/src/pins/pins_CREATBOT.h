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
 * Creatbot (Based on Rumba) pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "Creatbot supports up to 3 hotends / E-steppers. Comment out this line to continue."
#endif

#define DEFAULT_MACHINE_NAME "Creatbot DX"
#define BOARD_NAME           "Creatbot"

//
// Servos (Use for Servo or BLTouch)
//
//#define SERVO0_PIN         5

//
// Limit Switches
//
#define X_MIN_PIN          37
#define Y_MIN_PIN          35
#define Z_MIN_PIN          33

//
// Filament Runout Sensors
//
#define FIL_RUNOUT_PIN     36
#define FIL_RUNOUT2_PIN    34
//#define FIL_RUNOUT3_PIN    32

//
// Z Probe (when not Z_MIN_PIN, used for Auto level NOT BLTouch)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  33
#endif

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

//#define E2_STEP_PIN        29
//#define E2_DIR_PIN         28
//#define E2_ENABLE_PIN      39


//
// Temperature Sensors
//
#define TEMP_0_PIN       15   
#define TEMP_1_PIN       14   
#define TEMP_2_PIN       13
#define TEMP_3_PIN       12       
#define TEMP_BED_PIN     11   
//#define TEMP_CHAMBER_PIN   12   

//
// Heaters / Fans
//
#define HEATER_0_PIN        2
#define HEATER_1_PIN        3
#define HEATER_2_PIN        6
#define HEATER_3_PIN        8   
#define HEATER_BED_PIN      9
//#define HEATER_CHAMBER_PIN  8


#define FAN_PIN             7
#define FAN1_PIN            8


//
// Misc. Functions
//
#define LED_PIN            13
#define PS_ON_PIN          40
#define KILL_PIN           41
     



