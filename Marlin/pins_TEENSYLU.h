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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.   If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 *  Rev C    29MAY 2017
 *
 *  Converted all pins to Teensyduino system
 */

 /**
  *  SILKSCREEN ERROR
  *
  *  The silkscreen for the endstops do NOT match the schematic.  The silkscreen SHOULD
  *  read (from left to right) X-STOP, Y-STOP, Z-STOP & E-STOP.  The silkscreen actually
  *  reads                     E-STOP, X-STOP, Y-STOP & Z-STOP.
  *
  *  The pin assignments in this file match the silkscreen.
  */


#if !defined(__AVR_AT90USB1286__) && !defined(__AVR_AT90USB1286P__)
  #error "Oops!  Make sure you have 'AT90USB1286_STANDARD' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME         "Teensylu"

#define USBCON 1286  // Disable MarlinSerial etc.
#define LARGE_FLASH        true


//
// Limit Switche definitions that match the SCHEMATIC
//
//#define X_STOP_PIN              25
//#define Y_STOP_PIN              26
//#define Z_STOP_PIN              27
//#define E_STOP_PIN              36


//
// Limit Switch definitions that match the SILKSCREEN
//
#define X_STOP_PIN              26
#define Y_STOP_PIN              27
#define Z_STOP_PIN              36
//#define E_STOP_PIN              25

//
// Steppers
//
#define X_STEP_PIN              28
#define X_DIR_PIN               29
#define X_ENABLE_PIN            19

#define Y_STEP_PIN              30
#define Y_DIR_PIN               31
#define Y_ENABLE_PIN            18

#define Z_STEP_PIN              32
#define Z_DIR_PIN               33
#define Z_ENABLE_PIN            17

#define E0_STEP_PIN             34
#define E0_DIR_PIN              35
#define E0_ENABLE_PIN           13


// Temperature Sensors

#define TEMP_0_PIN               7  // Analog Input (Extruder)
#define TEMP_BED_PIN             6  // Analog Input (Bed)

//
// Heaters / Fans
//
#define HEATER_0_PIN            15  // Extruder
#define HEATER_BED_PIN          14

#define FAN_PIN                 22 // always was Teensy pin number

//
// Misc. Functions
//
#define SDSS                    20
#define CASE_LIGHT_PIN           0

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

  #define BEEPER_PIN            -1

  #if ENABLED(LCD_I2C_PANELOLU2)
    #define BTN_EN1              3
    #define BTN_EN2              2
    #define BTN_ENC             41
    #define SDSS                38  // use SD card on Panelolu2
  #endif // LCD_I2C_PANELOLU2

  #define SD_DETECT_PIN         -1

#endif // ULTRA_LCD && NEWPANEL

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN    24  // MUST BE HARDWARE PWM
#define SPINDLE_LASER_ENABLE_PIN 39  // Pin should have a pullup!
#define SPINDLE_DIR_PIN          40
