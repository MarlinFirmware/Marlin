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
 *  rev B    30 DEC 2016
 *
 *  The original version of this file did NOT result in a useful program because:
 *   1. The pin numbers assumed that the "#define AT90USBxx_TEENSYPP_ASSIGNMENTS" line
 *      in FASTIO.h was commented out. There wasn't an Arduino IDE 1.6.x extension/package
 *      that supported this pin map so the latest Marlin wouldn't compile.
 *   2. The silkscreen for the four end stops don't agree with the schematic. Activating
 *      the X endstop would tell the software that the Y endstop just went active.
 *   3. The thermistor inputs also had heater names assigned to them. The result was
 *      thermistor inputs that were set to digital outputs.
 *
 *  Rev B corrects the above problems by:
 *   1. The "Marlin_AT90USB" extension/package was developed.  This extension enables the
 *      latest Marlin software to compile using Arduino IDE 1.6.x and 1.80.
 *   2. The endstop pin numbers in this file were changed to match the silkscreen.  This
 *      makes it a little confusing when trying to correlate the schematic with the pin
 *      numbers used in this file.
 *   3. The offending heater names were deleted.
 *
 *  To create a useable image for Teensylu do the following:
 *   a) Install the Marlin_AT90USB extension with either of the following methods:
 *        Automatic - paste this URL into preferences and then use Boards manager
 *            https://rawgit.com/Bob-the-Kuhn/Marlin_AT90USB/master/package_MARLIN_AT90USB_index.json
 *        Manual:
 *           1. Copy the following URL into Go to "https://github.com/Bob-the-Kuhn/Marlin_AT90USB",
 *              click on the "Clone or Download" button and then click on "Download ZIP" button.
 *           2. Unzip the file, find the "Marlin_AT90USB" directory and then copy it to the
 *              hardware directory in Arduino.  The Arduino hardware directory will probably be
 *              located in a path similar to this: C:\Program Files (x86)\Arduino\hardware
 *   b) Connect the USBtinyISP to the board.
 *   c) In the Arduino IDE select the "AT90USB1286_STANDARD" board in the of the "Marlin_AT90USB"
 *      section and select the "USBtinyISP" programmer.
 *   d) In the Arduino IDE click on "burn bootloader".  Don't worry about the "verify
 *      failed at 1F000" error message.
 *   e) The USBtinyISP programmer is no longer needed.  Remove it.
 *   f) In FASTIO.h comment out the "#define AT90USBxx_TEENSYPP_ASSIGNMENTS" line.
 *   g) To upload a sketch do the following:
 *       1. remove the jumper
 *       2. press reset
 *       3. click on the "upload" button in the Arduino IDE
 *       4. wait until the upload finishes (less than a minute)
 *       5. put the jumper back on
 *       6. press the reset button
 *
 *
 *  NOTE - the "Marlin_AT90USB" pin maps make PWM0A available rather than the usual PWM1C.
 *         These PWMs share the same physical pin. Marlin uses TIMER1 to generate
 *         interrupts and sets it up such that PWM1A, PWM1B & PWM1C can not be used.
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

#if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
  #error "please disable (comment out) the AT90USBxx_TEENSYPP_ASSIGNMENTS flag in FASTIO.h "
#endif

#define BOARD_NAME         "Teensylu"

#define USBCON 1286  // Disable MarlinSerial etc.
#define LARGE_FLASH        true


//
// Limit Switche definitions that match the SCHEMATIC
//
//#define X_STOP_PIN              13
//#define Y_STOP_PIN              14
//#define Z_STOP_PIN              15
//#define E_STOP_PIN              36


//
// Limit Switch definitions that match the SILKSCREEN
//
#define X_STOP_PIN              14
#define Y_STOP_PIN              15
#define Z_STOP_PIN              36
//#define E_STOP_PIN              13

//
// Steppers
//
#define X_STEP_PIN               0
#define X_DIR_PIN                1
#define X_ENABLE_PIN            39

#define Y_STEP_PIN               2
#define Y_DIR_PIN                3
#define Y_ENABLE_PIN            38

#define Z_STEP_PIN               4
#define Z_DIR_PIN                5
#define Z_ENABLE_PIN            23

#define E0_STEP_PIN              6
#define E0_DIR_PIN               7
#define E0_ENABLE_PIN           19


// Temperature Sensors

#define TEMP_0_PIN               7  // Analog Input (Extruder)
#define TEMP_BED_PIN             6  // Analog Input (Bed)

//
// Heaters / Fans
//
#define HEATER_0_PIN            21  // Extruder
#define HEATER_BED_PIN          20

#define FAN_PIN                 22

//
// Misc. Functions
//
#define SDSS                     8
#define CASE_LIGHT_PIN          24

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)

  #define BEEPER_PIN            -1

  #if ENABLED(LCD_I2C_PANELOLU2)
    #define BTN_EN1             27
    #define BTN_EN2             26
    #define BTN_ENC             43
    #define SDSS                40  // use SD card on Panelolu2
  #endif // LCD_I2C_PANELOLU2

  #define SD_DETECT_PIN         -1

#endif // ULTRA_LCD && NEWPANEL

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN    12  // MUST BE HARDWARE PWM
#define SPINDLE_LASER_ENABLE_PIN 41  // Pin should have a pullup!
#define SPINDLE_DIR_PIN          42
