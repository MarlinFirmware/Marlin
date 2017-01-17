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
 * 5DPrint D8 Driver board pin assignments
 *
 * https://bitbucket.org/makible/5dprint-d8-controller-board
 */

/**
 *  Rev B   30 DEC 2016
 *
 *  The original version of this file did NOT result in a useful program unless special
 *  precautions were taken.  This was because some pin definitions used the Marlin "Standard"
 *  pin map and others used the TEENSY++ 2.0 (TEENSYPP) pin map (as defined in the "Teensyduino"
 *  extension from PJRC http://www.pjrc.com).  The result was some pins could use only
 *  the Arduino "digitalRead", "digitalWrite", ... functions and others could only use the
 *  the FASTIO "READ", "WRITE", ...  macros.  In general Marlin accesses some pins using
 *  both systems.  That results in a program that can't control the hardware correctly
 *  (changing X direction might turn on a heater FET, etc.)
 *
 *  We think this was so that the SD card detect signal on port E3 could be used.  In
 *  "Teensyduino" that port is not mapped to a logical pin so it wasn't available to the
 *  program.  To get around that the Marlin "Standard" pin map was used to access that
 *  pin.  Unfortunately there are no known Arduino IDE extensions that use the Marlin
 *  "Standard" pin map that is compatible with Arduino IDE 1.6.6 and later.  That means
 *  that the latest Marlin code can't be compiled for this board.
 *
 *  The following was done to allow compiling the latest Marlin code without special
 *  precautions:
 *   1. The "Marlin_AT90USB" Arduino IDE extension was created.  This extension contains
 *      board definitions for both the STANDARD and the TEENSYPP pin maps.  The TEENSYPP
 *      pin map has been extended to include ports E2 & E3.  The extended TEENSYPP pin
 *      map matches exactly the TEENSYPP map in FASTIO.h.
 *   2. Two sets of pin definitions were added to this file - one with just the TEENSY pin
 *      numbering and one with just Marlin "Standard" pin numbering.
 *
 */

/**
 *  The "Marlin_AT90USB" Arduino IDE extension can be found at
 *  https://github.com/Bob-the-Kuhn/Marlin_AT90USB
 *
 *  This extension contains the following boards.  All are 16MHz.
 *     AT90USB1286_TEENSYPP
 *     AT90USB646_TEENSYPP
 *     AT90USB1286_STANDARD
 *     AT90USB646_STANDARD
 *
 *  The STANDARD boards use the FASTIO AT90USB646/1286 map when the
 *  "AT90USBxx_TEENSYPP_ASSIGNMENTS" flag is commented out.
 *
 *  The TEENSYPP boards use the FASTIO AT90USB646/1286 map when the
 *  "AT90USBxx_TEENSYPP_ASSIGNMENTS" flag is enabled/defined. It is the same as the
 *  Teensyduino's "TEENSY++ 2.0" pin map with the addition of:
 *     port E2 is mapped to logical pin 46
 *     port E3 is mapped to logical pin 47
 *
 *  Since the schematic doesn't show the logical pin mapping it doesn't matter which
 *  pin map is selected by the user. Selection of the pin map is done by
 *     1. enabling/disabling the "AT90USBxx_TEENSYPP_ASSIGNMENTS" flag in fastio.h
 *     2. selecting the appropriate board in the Arduino TOOLS menu
 *
 *  NOTE - the "Marlin_AT90USB" pin maps make PWM0A available rather than the usual PWM1C.
 *         These PWMs share the same physical pin. Marlin uses TIMER1 to generate
 *         interrupts and sets it up such that PWM1A, PWM1B & PWM1C can not be used.
 *
 *  To create a useable image do the following:
 *   a) Install the extension with either of the following methods:
 *        Automatic - paste this URL into preferences and then use Boards manager
 *            https://rawgit.com/Bob-the-Kuhn/Marlin_AT90USB/master/package_MARLIN_AT90USB_index.json
 *        Manual:
 *           1. Copy the following URL into Go to "https://github.com/Bob-the-Kuhn/Marlin_AT90USB",
 *              click on the "Clone or Download" button and then click on "Download ZIP" button.
 *           2. Unzip the file, find the "Marlin_AT90USB" directory and then copy it to the
 *              hardware directory in Arduino.  The Arduino hardware directory will probably be
 *              located in a path similar to this: C:\Program Files (x86)\Arduino\hardware
 *   b) Connect the USBtinyISP to the board.
 *   c) In the Arduino IDE select one of the "Marlin_AT90USB" boards and select the
 *      "USBtinyISP" programmer.
 *   d) In the Arduino IDE click on "burn bootloader".  Don't worry about the "verify
 *      failed at 1F000" error message.
 *   e) The USBtinyISP programmer is no longer needed.  Remove it.
 *   f) In FASTIO.h comment/uncomment the "#define AT90USBxx_TEENSYPP_ASSIGNMENTS" line
 *      as needed.
 *   g) To upload a sketch do the following:
 *       1. remove the jumper
 *       2. press reset
 *       3. click on the "upload" button in the Arduino IDE
 *       4. wait until the upload finishes (less than a minute)
 *       5. put the jumper back on
 *       6. press the reset button
 *
 */

/**
 *  Using "Marlin_AT90USB" is not required.
 *
 *  If "Marlin_AT90USB" is not used then this file assumes that the "Teensyduino"
 *  extension from PJRC http://www.pjrc.com) is being used.  In that case the SD card
 *  detect capability is lost.
 *
 *  If neither extension is used then you'll probably need to modify this file. To aid
 *  in that
 *    1. The rev A pin definitions are still in this file.  They've at the end in one
 *       long comment.
 *    2. At the very end of this file is a table showing the two pin mappings, the signal
 *       names in this file and the signal names in the schematic.
 *
 */


#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0', 'AT90USB1286_TEENSYPP' or 'AT90USB1286_STANDARD' selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME "Makibox"
#define BOARD_NAME           "5DPrint D8"

#define USBCON 1286  // Disable MarlinSerial etc.
#define LARGE_FLASH        true

// determine which pin map to use
#if defined(ARDUINO_AT90USB1286_TEENSYPP) || defined(ARDUINO_AT90USB646_TEENSYPP)
  #define USE_TEENSYPP_PIN_MAP true // using TEENSYPP pin map
#elif defined(ARDUINO_AT90USB1286_STANDARD) || defined(ARDUINO_AT90USB646_STANDARD)
  #define USE_TEENSYPP_PIN_MAP false // using STANDARD pin map
#else
  #define USE_TEENSYPP_PIN_MAP true     // default to TEENSYPP pin map
#endif

#if USE_TEENSYPP_PIN_MAP   // start of "AT90USB1286_TEENSYPP" pin definitions

  #if !ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
    #error "please enable (un-comment out) the AT90USBxx_TEENSYPP_ASSIGNMENTS flag in FASTIO.h "
  #endif


  //
  // Limit Switches
  //
  #define X_STOP_PIN         37
  #define Y_STOP_PIN         36
  #define Z_STOP_PIN         19

  //
  // Steppers
  //
  #define X_STEP_PIN         28
  #define X_DIR_PIN          29
  #define X_ENABLE_PIN       17

  #define Y_STEP_PIN         30
  #define Y_DIR_PIN          31
  #define Y_ENABLE_PIN       13

  #define Z_STEP_PIN         32
  #define Z_DIR_PIN          33
  #define Z_ENABLE_PIN       12

  #define E0_STEP_PIN        34
  #define E0_DIR_PIN         35
  #define E0_ENABLE_PIN      11

  // Microstepping pins
  #define X_MS1_PIN          25
  #define X_MS2_PIN          26
  #define Y_MS1_PIN          9
  #define Y_MS2_PIN          8
  #define Z_MS1_PIN          7
  #define Z_MS2_PIN          6
  #define E0_MS1_PIN         5
  #define E0_MS2_PIN         4

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN         39
  #define TEMP_BED_PIN       38

  //
  // Heaters & Fans
  //
  #define HEATER_0_PIN       1
  #define HEATER_BED_PIN     0
  #define FAN_PIN            16

  //
  // SD card
  //
  #define SDSS               20
  #if defined(VARIANT_FLAG)
    #define SD_DETECT_PIN      47   //only valid if using Marlin_AT90USB extension
  #endif

  //
  // M3, M4 & M5 (spindle control) support
  //
  #define SPINDLE_DIR_PIN    42
  #define SPINDLE_ENABLE_PIN 45
  #define SPINDLE_SPEED_PIN  27

#else  // start of "AT90USB1286_STANDARD" pin definitions

  #if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
    #error "please disable (comment out) the AT90USBxx_TEENSYPP_ASSIGNMENTS flag in FASTIO.h "
  #endif

  //
  // Limit Switches
  //
  #define X_STOP_PIN         37
  #define Y_STOP_PIN         36
  #define Z_STOP_PIN         39

  //
  // Steppers
  //
  #define X_STEP_PIN         0
  #define X_DIR_PIN          1
  #define X_ENABLE_PIN       23

  #define Y_STEP_PIN         2
  #define Y_DIR_PIN          3
  #define Y_ENABLE_PIN       19

  #define Z_STEP_PIN         4
  #define Z_DIR_PIN          5
  #define Z_ENABLE_PIN       18

  #define E0_STEP_PIN        6
  #define E0_DIR_PIN         7
  #define E0_ENABLE_PIN      17

  // Microstepping pins
  #define X_MS1_PIN          13
  #define X_MS2_PIN          14
  #define Y_MS1_PIN          33
  #define Y_MS2_PIN          32
  #define Z_MS1_PIN          31
  #define Z_MS2_PIN          30
  #define E0_MS1_PIN         29
  #define E0_MS2_PIN         28

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN         41
  #define TEMP_BED_PIN       40

  //
  // Heaters & Fans
  //
  #define HEATER_0_PIN       21
  #define HEATER_BED_PIN     20
  #define FAN_PIN            22

  //
  // SD card
  //
  #define SDSS               8
  #define SD_DETECT_PIN      35

  //
  // M3, M4 & M5 (spindle control) support
  //
  #define SPINDLE_DIR_PIN    15
  #define SPINDLE_ENABLE_PIN 44 // should have a pullup
  #define SPINDLE_SPEED_PIN  47

#endif  // end of "AT90USB1286_STANDARD" pin definitions

/**    // Rev A pin definitions

  #if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
    #error "please disable (comment out) the AT90USBxx_TEENSYPP_ASSIGNMENTS flag in FASTIO.h "
  #endif


  //
  // Limit Switches
  //
  #define X_STOP_PIN         37
  #define Y_STOP_PIN         36
  #define Z_STOP_PIN         39

  //
  // Steppers
  //
  #define X_STEP_PIN          0
  #define X_DIR_PIN           1
  #define X_ENABLE_PIN       23

  #define Y_STEP_PIN          2
  #define Y_DIR_PIN           3
  #define Y_ENABLE_PIN       19

  #define Z_STEP_PIN          4
  #define Z_DIR_PIN           5
  #define Z_ENABLE_PIN       18

  #define E0_STEP_PIN         6
  #define E0_DIR_PIN          7
  #define E0_ENABLE_PIN      17

  // Microstepping pins - Mapping not from fastio.h (?)
  #define X_MS1_PIN          25
  #define X_MS2_PIN          26
  #define Y_MS1_PIN           9
  #define Y_MS2_PIN           8
  #define Z_MS1_PIN           7
  #define Z_MS2_PIN           6
  #define E0_MS1_PIN          5
  #define E0_MS2_PIN          4

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN          1   // Analog Input
  #define TEMP_BED_PIN        0   // Analog Input

  //
  // Heaters / Fans
  //
  #define HEATER_0_PIN       21
  #define HEATER_BED_PIN     20

  // You may need to change FAN_PIN to 16 because Marlin isn't using fastio.h
  // for the fan and Teensyduino uses a different pin mapping.
  #define FAN_PIN            16

  //
  // Misc. Functions
  //
  #define SDSS               20

 */  // Rev A pin definitions


/**
 *                      alternate  TEENSYPP  "Standard"        schematic  Expansion
 *  names in this file  functions  pin #     pin #       port  names      pin #
 *  X_STEP_PIN                     28        0           A0    X_STEP
 *  X_DIR_PIN                      29        1           A1    X_DIR
 *  Y_STEP_PIN                     30        2           A2    Y_STEP
 *  Y_DIR_PIN                      31        3           A3    Y_DIR
 *  Z_STEP_PIN                     32        4           A4    Z_STEP
 *  Z_DIR_PIN                      33        5           A5    Z_DIR
 *  E0_STEP_PIN                    34        6           A6    E_STEP
 *  E0_DIR_PIN                     35        7           A7    E_DIR
 *  SDSS                SS         20        8           B0    SDCS
 *                      SCK        21        9           B1    SCLK
 *                      MOSI       22        10          B2    MOSI
 *                      MISO       23        11          B3    MISO
 *                      PWM2A      24        12          B4    USB_LED
 *  X_MS1_PIN                      25        13          B5    X_MSI
 *  X_MS2_PIN                      26        14          B6    X_MS2
 *  SPINDLE_SPEED_PIN   PWM0A      27        15          B7    SP4        7
 *                                 10        16          C0    BUZ_DRV
 *  E0_ENABLE_PIN                  11        17          C1    /E_EN
 *  Z_ENABLE_PIN                   12        18          C2    /Z_EN
 *  Y_ENABLE_PIN                   13        19          C3    /Y_EN
 *  HEATER_BED_PIN      PWM3C      14        20          C4    HB_DRV
 *  HEATER_0_PIN        PWM3B      15        21          C5    HE_DRV
 *  FAN_PIN             PWM3A      16        22          C6    FAN_DRV
 *  X_ENABLE_PIN                   17        23          C7    /X_EN
 *                      PWM0B      0         24          D0    SCL
 *                      PWM2B      1         25          D1    SDA
 *                                 2         26          D2    SP7        10
 *                                 3         27          D3    SP8        11
 *  E0_MS2_PIN                     4         28          D3    E_MS2
 *  E0_MS1_PIN                     5         29          D4    E_MS1
 *  Z_MS2_PIN                      6         30          D5    Z_MS2
 *  Z_MS1_PIN                      7         31          D7    Z_MS1
 *  Y_MS2_PIN                      8         32          E0    Y_MS2
 *  Y_MS1_PIN                      9         33          E1    Y_MS1
 *                                 46*       34          E2
 *  SD_DETECT_PIN                  47*       35          E3    SDCD
 *  Y_STOP_PIN                     36        36          E4    Y_LIMIT
 *  X_STOP_PIN                     37        37          E5    X_LIMIT
 *                                 18        38          E6    EM_STOP
 *  Z_STOP_PIN                     19        39          E7    Z_LIMIT
 *  TEMP_BED_PIN        A0         38        40          F0    HB_THERM
 *  TEMP_0_PIN          A1         39        41          F1    HE_THERM
 *                      A2         40        42          F2    SP0        3
 *                      A3         41        43          F3    SP1        4
 *  SPINDLE_DIR_PIN     A4         42        44          F4    SP2        5
 *                      A5         43        45          F5    SP3        6
 *                      A6         44        46          F6    SP5        8
 *  SPINDLE_ENABLE_PIN  A7         45        47          F7    SP6        9
 *
 *                                 * not in (added to) Teensyduino pin map
 */
