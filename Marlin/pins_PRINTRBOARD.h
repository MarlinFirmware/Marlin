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
 *  Rev B   31 DEC 2016
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
 *  We think this was so that the X-STOP signal on port E3 could be used.  In
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
 *  At the very end of this file is a table showing the two pin mappings, the signal names
 *  in this file and the signal names in the schematic.
 *
 *  Rev B also adds support for the CASE_LIGHT and the M3, M4 & M5 spindle control commands.
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
 */

/**
 *  Using "Marlin_AT90USB" is not required.
 *
 *  If "Marlin_AT90USB" is not used then the Rev A pin definitions are used.  This assumes
 *  that either the "Teensyduino" extension from PJRC http://www.pjrc.com) or the "Printrboard"
 *  extension from https://github.com/scwimbush/Printrboard-HID-Arduino-IDE-Support is being
 *  used and requires the "#define AT90USBxx_TEENSYPP_ASSIGNMENTS" line in FASTIO.h to be
 *  commented out.  In that case the X-STOP capability is probably lost.
 *
 *  The "Printrboard" extension is preferred because it contains a better bootloader.
 */

/**
 * Rev B changes to pin functions
 *
 *  Since the SPI module is in MASTER mode, the SDSS signal is manually controlled by
 *  by the software which means the SDSS function can be assigned to any convenient pin.
 *  This allows the following:
 *    SDSS pin number changes depending on where the active SD card is (on board, on LCD)
 *    Y_STOP is always at the Y-stop connector
 *
 *  SD card detect is enabled for on board SD card
 *
 *  PANELOLU2 section
 *    In Rev A the SDSS pin used the same pin as FILAMENT_WIDTH_SENSOR.  In Rev B the
 *      FILWIDTH_PIN was moved to analog input A4 (when PANELOLU2 is enabled) .  That
 *      makes it available on pin 8 (TCK) on the SV1 connector on Printrboard Adapter.
 *      http://blog.think3dprint3d.com/2013/04/panelolu2-for-ramps-and-printrboard.html
 *
 *  VIKI section
 *    SDSS moved to resolve conflict with BTN_ENC
 *      This was caused by an error in the table in the VIKI 2 wiring diagram published
 *      by Panucatt maker of VIKI).  It was moved to the pin shown in the graphic.
 *    SD card detect added
 *      Caused by another error in the wiring diagram table.  It was set to the pin shown
 *      in the graphic.
 *    RESET - added a comment in case the user would rather the red button on the panel be
 *            be tied to the KILL_PIN instead of RESET.  RESET is shown in the graphic.
 *
 * None of these changes have been done to the Rev A section.
 */

/**
 * Rev A refers to "FASTIO" pins, "Non-FastIO" pins and "Teensyduino" pins.
 *   "FASTIO" pins are what the STANDARD boards use.  This is the FASTIO AT90USB646/1286 map
 *      when the "AT90USBxx_TEENSYPP_ASSIGNMENTS" flag is commented out.  Rev B sometimes
 *      calls this the STANDARD pin map.
 *   "Non-FastIO" pins and "Teensyduino" pins are a subset of the pin map used by the
 *      TEENSYPP boards.  Rev B sometimes calls this the TEENSYPP pin map.
 */

#ifndef __AVR_AT90USB1286__
  #error "Oops!  Make sure you have 'Teensy++ 2.0', 'AT90USB1286_TEENSYPP', 'AT90USB1286_STANDARD' or "Printrboard" selected from the 'Tools -> Boards' menu."
#endif

// determine which pin map to use
#if defined(ARDUINO_AT90USB1286_TEENSYPP)
  #define USE_TEENSYPP_PIN_MAP true // using TEENSYPP pin map
#elif defined(ARDUINO_AT90USB1286_STANDARD)
  #define USE_TEENSYPP_PIN_MAP false // using STANDARD pin map
#else
  #define USE_TEENSYPP_PIN_MAP true     // default to TEENSYPP pin map
#endif

#define BOARD_NAME         "Printrboard"

#define USBCON 1286  // Disable MarlinSerial etc.
#define LARGE_FLASH        true

// Disable JTAG pins so they can be used for the Extrudrboard
#define DISABLE_JTAG       true

#if USE_TEENSYPP_PIN_MAP && defined(ARDUINO_AT90USB1286_TEENSYPP)  // start of "AT90USB1286_TEENSYPP" pin definitions

  #if !ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
    #error "please enable (un-comment out) the AT90USBxx_TEENSYPP_ASSIGNMENTS flag in FASTIO.h "
  #endif

  //
  // Limit Switches
  //
  #define X_STOP_PIN          47
  #define Y_STOP_PIN          20 // Rev A moved this to the E_STOP if the on board SD card is enabled
                                 // Rev B schenmatic says this pin only goes to the Y_STOP
  #define Z_STOP_PIN          36

  //
  // Steppers
  //
  #define X_STEP_PIN          28
  #define X_DIR_PIN           29
  #define X_ENABLE_PIN        19

  #define Y_STEP_PIN          30
  #define Y_DIR_PIN           31
  #define Y_ENABLE_PIN        18

  #define Z_STEP_PIN          32
  #define Z_DIR_PIN           33
  #define Z_ENABLE_PIN        17

  #define E0_STEP_PIN         34
  #define E0_DIR_PIN          35
  #define E0_ENABLE_PIN       13

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN           1   // Analog Input
  #define TEMP_BED_PIN         0   // Analog Input

  //
  // Heaters / Fans
  //
  #define HEATER_0_PIN        15 // Extruder
//  #define HEATER_1_PIN        44  // used by LCD
//  #define HEATER_2_PIN        45  // used by LCD
  #define HEATER_BED_PIN      14

  #define FAN_PIN             16  //


  //
  // Misc. Functions
  //
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    #define FILWIDTH_PIN       2   // Analog Input
  #endif

  //
  // on board SD card support -  Rev B only
  //
  #define SD_DETECT_PIN       27
  #define MISO_PIN            23
  #define MOSI_PIN            22
  #define SCK_PIN             21
  #define SDSS                26 // Rev A pin numbering said 20 but Rev B schematic says 8 only goes to Y-STOP


  //
  // LCD / Controller
  //
  #if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)
    // we have no buzzer installed
    #define BEEPER_PIN        -1

    // LCD Pins
    #if ENABLED(LCD_I2C_PANELOLU2)  // Printrboard Adapter compatible pin numbering
      #define BTN_EN1          3 // RX1 - fastio.h pin mapping 27
      #define BTN_EN2          2 // TX1 - fastio.h pin mapping 26
      #define BTN_ENC         41 // A3 - fastio.h pin mapping 43
      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        #undef FILWIDTH_PIN
        #define FILWIDTH_PIN   4  // analog  - move to pin 8 (TCK) on SV1 connector on Printrboard Adapter
      #endif
      #undef  SDSS
      #define SDSS            40 // A2 Teensy 40 use SD card on Panelolu2
    #else
      #define BTN_EN1         10
      #define BTN_EN2         11
      #define BTN_ENC         12 // the click
    #endif // LCD_I2C_PANELOLU2

    // not connected to a pin
    #undef  SD_DETECT_PIN
    #define SD_DETECT_PIN     -1

    #define LCD_PINS_RS        9 // Teensy 9
    #define LCD_PINS_ENABLE    8 // Teensy 8
    #define LCD_PINS_D4        7 // Teensy 7
    #define LCD_PINS_D5        6 // Teensy 6
    #define LCD_PINS_D6        5 // Teensy 5
    #define LCD_PINS_D7        4 // Teensy 4

  #endif // ULTRA_LCD && NEWPANEL

  #if ENABLED(VIKI2) || ENABLED(miniVIKI)
    // FastIO
    #define BEEPER_PIN         8 // use beeper on VIKI
    // Pins for DOGM SPI LCD Support
    #define DOGLCD_A0         42 // Teensy 42
    #define DOGLCD_CS         43 // Teensy 43
    #define LCD_SCREEN_ROT_180

    // The encoder and click button (FastIO Pins)
    #define BTN_EN1            2
    #define BTN_EN2            3
    #define BTN_ENC           45

    #undef  SDSS
    #define SDSS              44 // error in Rev A - had SDSS and BTN_ENC tied to FastIO 47
                                 // this error was copied from the table in the VIKI 2 wiring
                                 // diagram published by Panucatt (maker of VIKI)
    #undef  SD_DETECT_PIN
    #define SD_DETECT_PIN     11 // Rev A had -1 (another error in VIKI 2 table)

    #define STAT_LED_RED_PIN  12 // Teensy 12
    #define STAT_LED_BLUE_PIN 10 // Teensy 10

    // Graphic in VIKI 2 wiring diagram has the panel mounted (red) button tied to reset. If
    // desired it could be moved to another pin and then the kill pin assigned to it.
    //#define KILL_PIN           9 // JP11 pin 11 (PE1)
  #endif

  //
  // M3, M4 & M5 (spindle control) support
  //
  #if ENABLED(SPINDLE_ENABLE)   // assumes dedicated to CNC - use heater pins for spindle control
    #undef HEATER_0_PIN
    #undef HEATER_BED_PIN
    #define HEATER_0_PIN       25  // need to have heater pins or else Marlin won't compile so
    #define HEATER_BED_PIN     24  // assign them to unconnected ports
    #define SPINDLE_DIR_PIN    15  // must have a pull up
    #define SPINDLE_ENABLE_PIN  0  // should have a pull up
    #define SPINDLE_SPEED_PIN  14  // must have a pull up
  #endif

  //
  // Case light
  //
  #if !PIN_EXISTS(CASE_LIGHT)
    #define CASE_LIGHT_PIN      1
  #endif


// ===========================================================================================
#elif defined(ARDUINO_AT90USB1286_STANDARD)   // start of "AT90USB1286_STANDARD" pin definitions

  #if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)
    #error "please disable (comment out) the AT90USBxx_TEENSYPP_ASSIGNMENTS flag in FASTIO.h "
  #endif

  //
  // Limit Switches
  //
  #define X_STOP_PIN          35
  #define Y_STOP_PIN           8 // Rev A moved this to 37 (E_STOP) if the on board SD card is enabled
                                 // Rev B schenmatic says this pin only goes to the Y_STOP
  #define Z_STOP_PIN          36

  //
  // Steppers
  //
  #define X_STEP_PIN           0
  #define X_DIR_PIN            1
  #define X_ENABLE_PIN        39

  #define Y_STEP_PIN           2
  #define Y_DIR_PIN            3
  #define Y_ENABLE_PIN        38

  #define Z_STEP_PIN           4
  #define Z_DIR_PIN            5
  #define Z_ENABLE_PIN        23

  #define E0_STEP_PIN          6
  #define E0_DIR_PIN           7
  #define E0_ENABLE_PIN       19

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN           1   // Analog Input
  #define TEMP_BED_PIN         0   // Analog Input

  //
  // Heaters / Fans
  //
  #define HEATER_0_PIN        21 // Extruder
//  #define HEATER_1_PIN        46  // used by LCD
//  #define HEATER_2_PIN        47  // used by LCD
  #define HEATER_BED_PIN      20

  #define FAN_PIN             22  // Teensy 16


  //
  // Misc. Functions
  //
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    #define FILWIDTH_PIN       2   // Analog Input
  #endif

  //
  // on board SD card support -  Rev B only
  //
  #define SD_DETECT_PIN       15
  #define MISO_PIN            11
  #define MOSI_PIN            10
  #define SCK_PIN              9
  #define SDSS                14 // Rev A pin numbering said 8 but Rev B schematic says 8 only goes to Y-STOP


  //
  // LCD / Controller
  //
  #if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)
    // we have no buzzer installed
    #define BEEPER_PIN        -1

    // LCD Pins
    #if ENABLED(LCD_I2C_PANELOLU2)  // Printrboard Adapter compatible pin numbering
      #define BTN_EN1         27 // RX1 - fastio.h pin mapping 27
      #define BTN_EN2         26 // TX1 - fastio.h pin mapping 26
      #define BTN_ENC         43 // A3 - fastio.h pin mapping 43
      #if ENABLED(FILAMENT_WIDTH_SENSOR)
        #undef FILWIDTH_PIN
        #define FILWIDTH_PIN   4  // analog  - move to pin 8 (TCK) on SV1 connector on Printrboard Adapter
      #endif
      #undef  SDSS
      #define SDSS            42 // A2 Teensy 40 use SD card on Panelolu2
    #else
      #define BTN_EN1         16
      #define BTN_EN2         17
      #define BTN_ENC         18 // the click
    #endif // LCD_I2C_PANELOLU2

    // not connected to a pin
    #undef  SD_DETECT_PIN
    #define SD_DETECT_PIN     -1

    #define LCD_PINS_RS       33 // Teensy 9
    #define LCD_PINS_ENABLE   32 // Teensy 8
    #define LCD_PINS_D4       31 // Teensy 7
    #define LCD_PINS_D5       30 // Teensy 6
    #define LCD_PINS_D6       29 // Teensy 5
    #define LCD_PINS_D7       28 // Teensy 4

  #endif // ULTRA_LCD && NEWPANEL

  #if ENABLED(VIKI2) || ENABLED(miniVIKI)
    // FastIO
    #define BEEPER_PIN        32 // use beeper on VIKI
    // Pins for DOGM SPI LCD Support
    #define DOGLCD_A0         44 // Teensy 42
    #define DOGLCD_CS         45 // Teensy 43
    #define LCD_SCREEN_ROT_180

    // The encoder and click button (FastIO Pins)
    #define BTN_EN1           26
    #define BTN_EN2           27
    #define BTN_ENC           47

    #undef  SDSS
    #define SDSS              46 // error in Rev A - had SDSS and BTN_ENC tied to FastIO 47
                                 // this error was copied from the table in the VIKI 2 wiring
                                 // diagram published by Panucatt (maker of VIKI)
    #undef  SD_DETECT_PIN
    #define SD_DETECT_PIN     17 // Rev A had -1 (another error in VIKI 2 table)

    #define STAT_LED_RED_PIN  18 // Teensy 12
    #define STAT_LED_BLUE_PIN 16 // Teensy 10

    // Graphic in VIKI 2 wiring diagram has the panel mounted (red) button tied to reset. If
    // desired it could be moved to another pin and then the kill pin assigned to it.
    //#define KILL_PIN          33 // JP11 pin 11 (PE1)
  #endif

  //
  // M3, M4 & M5 (spindle control) support
  //
  #if ENABLED(SPINDLE_ENABLE)   // assumes dedicated to CNC - use heater pins for spindle control
    #undef HEATER_0_PIN
    #undef HEATER_BED_PIN
    #define HEATER_0_PIN       13  // need to have heater pins or else Marlin won't compile so
    #define HEATER_BED_PIN     12  // assign them to unconnected ports
    #define SPINDLE_DIR_PIN    21  // must have a pull up
    #define SPINDLE_ENABLE_PIN 24  // should have a pull up
    #define SPINDLE_SPEED_PIN  20  // must have a pull up
  #endif

  //
  // Case light
  //
  #if !PIN_EXISTS(CASE_LIGHT)
    #define CASE_LIGHT_PIN     25
  #endif

// ===========================================================================================
#else   // exact copy of Rev A

  /**
   * Printrboard pin assignments (AT90USB1286)
   * Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
   * http://www.pjrc.com/teensy/teensyduino.html
   * See http://reprap.org/wiki/Printrboard for more info
   */

  #ifndef __AVR_AT90USB1286__
    #error "Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu."
  #endif

  #if ENABLED(AT90USBxx_TEENSYPP_ASSIGNMENTS)  // use Teensyduino Teensy++2.0 pin assignments instead of Marlin traditional.
    #error "These Printrboard assignments depend on traditional Marlin assignments, not AT90USBxx_TEENSYPP_ASSIGNMENTS in fastio.h"
  #endif

  #define BOARD_NAME         "Printrboard"

  #define USBCON 1286  // Disable MarlinSerial etc.
  #define LARGE_FLASH        true

  // Disable JTAG pins so they can be used for the Extrudrboard
  #define DISABLE_JTAG       true

  //
  // Limit Switches
  //
  #define X_STOP_PIN         35
  #if ENABLED(SDSUPPORT)
    #define Y_STOP_PIN       37 // Move Ystop to Estop socket
  #else
    #define Y_STOP_PIN        8 // Ystop in Ystop socket
  #endif
  #define Z_STOP_PIN         36

  //
  // Steppers
  //
  #define X_STEP_PIN          0
  #define X_DIR_PIN           1
  #define X_ENABLE_PIN       39

  #define Y_STEP_PIN          2
  #define Y_DIR_PIN           3
  #define Y_ENABLE_PIN       38

  #define Z_STEP_PIN          4
  #define Z_DIR_PIN           5
  #define Z_ENABLE_PIN       23

  #define E0_STEP_PIN         6
  #define E0_DIR_PIN          7
  #define E0_ENABLE_PIN      19

  //
  // Temperature Sensors
  //
  #define TEMP_0_PIN          1   // Analog Input
  #define TEMP_BED_PIN        0   // Analog Input

  //
  // Heaters / Fans
  //
  #define HEATER_0_PIN       21 // Extruder
  #define HEATER_1_PIN       46
  #define HEATER_2_PIN       47
  #define HEATER_BED_PIN     20

  // If soft or fast PWM is off then use Teensyduino pin numbering, Marlin
  // fastio pin numbering otherwise
  #if ENABLED(FAN_SOFT_PWM) || ENABLED(FAST_PWM_FAN)
    #define FAN_PIN          22
  #else
    #define FAN_PIN          16
  #endif

  //
  // Misc. Functions
  //
  #define SDSS               26
  #define FILWIDTH_PIN        2   // Analog Input

  //
  // LCD / Controller
  //
  #if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)
    // we have no buzzer installed
    #define BEEPER_PIN -1

    // LCD Pins
    #if ENABLED(LCD_I2C_PANELOLU2)
      #define BTN_EN1 27 // RX1 - fastio.h pin mapping 27
      #define BTN_EN2 26 // TX1 - fastio.h pin mapping 26
      #define BTN_ENC 43 // A3 - fastio.h pin mapping 43
      #define SDSS    40 // use SD card on Panelolu2 (Teensyduino pin mapping)
    #else
      #define BTN_EN1 16
      #define BTN_EN2 17
      #define BTN_ENC 18 // the click
    #endif // LCD_I2C_PANELOLU2

    // not connected to a pin
    #define SD_DETECT_PIN -1

    #define LCD_PINS_RS 9
    #define LCD_PINS_ENABLE 8
    #define LCD_PINS_D4 7
    #define LCD_PINS_D5 6
    #define LCD_PINS_D6 5
    #define LCD_PINS_D7 4

  #endif // ULTRA_LCD && NEWPANEL

  #if ENABLED(VIKI2) || ENABLED(miniVIKI)
    // FastIO
    #define BEEPER_PIN 32
    // Pins for DOGM SPI LCD Support
    #define DOGLCD_A0  42 // Non-FastIO
    #define DOGLCD_CS  43 // Non-FastIO
    #define LCD_SCREEN_ROT_180

    // The encoder and click button (FastIO Pins)
    #define BTN_EN1 26
    #define BTN_EN2 27
    #define BTN_ENC 47

    #define SDSS 45
    #define SD_DETECT_PIN -1 // FastIO (Manual says 72 I'm not certain cause I can't test)

    #define STAT_LED_RED_PIN  12 // Non-FastIO
    #define STAT_LED_BLUE_PIN 10 // Non-FastIO

  #endif
#endif


/**
 *  Schematic, pin maps & pin definitions
 *
 *                        alternate  schematic                               Rev A                        Rev B
 *  Teensy  FASTIO  port  functions  name           other    ISP  JP2  JP11  name                         name
 *  D28     D0      A0               X-STEP                                  X_STEP_PIN                   X_STEP_PIN
 *  D29     D1      A1               X-DIR                                   X_DIR_PIN                    X_DIR_PIN
 *  D30     D2      A2               Y-STEP                                  Y_STEP_PIN                   Y_STEP_PIN
 *  D31     D3      A3               Y-DIR                                   Y_DIR_PIN                    Y_DIR_PIN
 *  D32     D4      A4               Z-STEP                                  Z_STEP_PIN                   Z_STEP_PIN
 *  D33     D5      A5               Z-DIR                                   Z_DIR_PIN                    Z_DIR_PIN
 *  D34     D6      A6               E-STEP                                  E0_STEP_PIN                  E0_STEP_PIN
 *  D35     D7      A7               E-DIR                                   E0_DIR_PIN                   E0_DIR_PIN
 *  D20     D8      B0    SS         Y-STOP         X                        Y_STOP_PIN****               Y_STOP_PIN
 *  D21     D9      B1    SCK        SCLK           SD CARD   X                                           SCK_PIN
 *  D22     D10     B2    MOSI       MOSI           SD CARD   X                                           MOSI_PIN
 *  D23     D11     B3    MISO       MISO           SD CARD   X                                           MISO_PIN
 *  D24     D12     B4    PWM2A      not connected                                                        HEATER_BED_PIN
 *  D25     D13     B5               not connected                                                        HEATER_0_PIN
 *  D26     D14     B6               SDCS           SD CARD                                               SDSS
 *  D27     D15     B7    PWM0A**    SDCD           SD CARD                                               SD_DETECT_PIN
 *  D10     D16     C0               PC0                                X    BTN_EN1, STAT_LED_BLUE_PIN   BTN_EN1, STAT_LED_BLUE_PIN
 *  D11     D17     C1               PC1                                X    BTN_EN2                      BTN_EN2, SD_DETECT_PIN
 *  D12     D18     C2               PC2                                X    BTN_ENC, STAT_LED_RED_PIN    BTN_ENC, STAT_LED_RED_PIN
 *  D13     D19     C3               E-ENABLE                                E0_ENABLE_PIN                E0_ENABLE_PIN
 *  D14     D20     C4    PWM3C      HOTBED         X***                     HEATER_BED_PIN               HEATER_BED_PIN, SPINDLE_SPEED_PIN
 *  D15     D21     C5    PWM3B      HOTEND         X***                     HEATER_0_PIN                 HEATER_0_PIN, SPINDLE_DIR_PIN
 *  D16     D22     C6    PWM3A      PWM-FAN        X***                     FAN_PIN                      FAN_PIN
 *  D17     D23     C7               Z-ENABLE                                Z_ENABLE_PIN                 Z_ENABLE_PIN
 *  D0      D24     D0    PWM0B      PDO-SCL                       X                                      SPINDLE_ENABLE_PIN
 *  D1      D25     D1    PWM2B      PD1-SDA                       X                                      CASE_LIGHT_PIN
 *  D2      D26     D2               PD2-RX1                       X         BTN_EN1, BTN_EN2, SDSS       BTN_EN1, BTN_EN2
 *  D3      D27     D3               PD3-TX1                       X         BTN_EN1, BTN_EN2             BTN_EN1, BTN_EN2
 *  D4      D28     D4               PD4                                X    LCD_PINS_D7                  LCD_PINS_D7
 *  D5      D29     D5               PD5                                X    LCD_PINS_D6                  LCD_PINS_D6
 *  D6      D30     D6               PD6                                X    LCD_PINS_D5                  LCD_PINS_D5
 *  D7      D31     D7               PD7                                X    LCD_PINS_D4                  LCD_PINS_D4
 *  D8      D32     E0               PE0                                X    BEEPER_PIN, LCD_PINS_ENABLE  BEEPER_PIN, LCD_PINS_ENABLE
 *  D9      D33     E1               PE1                                X    LCD_PINS_RS                  KILL_PIN, LCD_PINS_RS
 *  D46*    D34     E2               ALE
 *  D47*    D35     E3               X-STOP         X                        X_STOP_PIN                   X_STOP_PIN
 *  D36     D36     E4               Z-STOP         X                        Z_STOP_PIN                   Z_STOP_PIN
 *  D37     D37     E5               E-STOP         X                        Y_STOP_PIN
 *  D18     D38     E6               Y-ENABLE                                Y_ENABLE_PIN                 Y_ENABLE_PIN
 *  D19     D39     E7               X-ENABLE                                X_ENABLE_PIN                 X_ENABLE_PIN
 *  D38     D40     F0    A0         B-THERM        4.7K                     TEMP_BED_PIN                 TEMP_BED_PIN
 *  D39     D41     F1    A1         E-THERM        4.7K                     TEMP_0_PIN                   TEMP_0_PIN
 *  D40     D42     F2    A2         A2                            X         FILWIDTH_PIN, SDSS           FILWIDTH_PIN, SDSS
 *  D41     D43     F3    A3         A3                            X         BTN_ENC                      BTN_ENC
 *  D42     D44     F4    A4         TCK                           X         DOGLCD_A0                    FILWIDTH_PIN, DOGLCD_A0
 *  D43     D45     F5    A5         TMS                           X         SDSS, DOGLCD_CS              DOGLCD_CS
 *  D44     D46     F6    A6         TDO                           X         HEATER_1_PIN                 HEATER_1_PIN, SDSS
 *  D45     D47     F7    A7         TDI                           X         HEATER_2_PIN, BTN_ENC        HEATER_2_PIN, BTN_ENC

 *
 *
 *    * not in Teensyduino pin map but is in the AT90USB1286_TEENSYPP pin map
 *    ** not available Teensyduino pin map but is available in the AT90USB1286_TEENSYPP pin map
 *    *** open collector with no pullup
 *    **** in Rev A the Y-STOP function is moved to the E-STOP connector if the SD card is enabled
 */




/**
 *  PIN DEFINITION CROSS REFERENCE
 *
 *  name in             name in            Rev A  Rev A     Rev A
 *  Rev B               Rev A               pin   used in   used in   FASTIO  Teensy
 *  section             section              #    FASTIO ?  Teensy ?
 *  BEEPER_PIN          BEEPER_PIN         32        X                32      8
 *  BTN_EN1             BTN_EN1            16        X                16      2
 *  BTN_EN1             BTN_EN1            26        X                26      3
 *  BTN_EN1             BTN_EN1            27        X                27      10
 *  BTN_EN2             BTN_EN2            17        X                17      2
 *  BTN_EN2             BTN_EN2            26        X                26      3
 *  BTN_EN2             BTN_EN2            27        X                27      11
 *  BTN_ENC             BTN_ENC            18        X                18      12
 *  BTN_ENC             BTN_ENC            43        X                43      41
 *  BTN_ENC             BTN_ENC            47        X                47      45
 *  CASE_LIGHT_PIN                                                    24      1
 *  DOGLCD_A0           DOGLCD_A0          42                  X      44      42
 *  DOGLCD_CS           DOGLCD_CS          43                  X      45      43
 *  E0_DIR_PIN          E0_DIR_PIN         7         X                7       35
 *  E0_ENABLE_PIN       E0_ENABLE_PIN      19        X                19      13
 *  E0_STEP_PIN         E0_STEP_PIN        6         X                6       34
 *  FAN_PIN             FAN_PIN            16                  X      22      16
 *                      FAN_PIN            22        X
 *  FILWIDTH_PIN        FILWIDTH_PIN       42        X                42      40
 *  FILWIDTH_PIN                                                      44      42
 *  HEATER_0_PIN                                                      13      25
 *  HEATER_0_PIN        HEATER_0_PIN       21        X                21      15
 *  HEATER_1_PIN        HEATER_1_PIN       46        X                46      44
 *  HEATER_2_PIN        HEATER_2_PIN       47        X                47      45
 *  HEATER_BED_PIN                                                    12      24
 *  HEATER_BED_PIN      HEATER_BED_PIN     20        X                20      14
 *  KILL_PIN                                                          33      9
 *  LCD_PINS_D4         LCD_PINS_D4        7                   X      31      7
 *  LCD_PINS_D5         LCD_PINS_D5        6                   X      30      6
 *  LCD_PINS_D6         LCD_PINS_D6        5                   X      29      5
 *  LCD_PINS_D7         LCD_PINS_D7        4                   X      28      4
 *  LCD_PINS_ENABLE     LCD_PINS_ENABLE    8                   X      32      8
 *  LCD_PINS_RS         LCD_PINS_RS        9                   X      33      9
 *  MISO_PIN                                                          11      23
 *  MOSI_PIN                                                          10      22
 *  SCK_PIN                                                           9       21
 *  SD_DETECT_PIN                                                     15      11
 *  SD_DETECT_PIN                                                     17      27
 *  SDSS                SDSS               26                  X      14      26
 *  SDSS                SDSS               40                  X      42      40
 *  SDSS                SDSS               45        X                46      44
 *  SPINDLE_DIR_PIN                                                   21      15
 *  SPINDLE_ENABLE_PIN                                                24      0
 *  SPINDLE_SPEED_PIN                                                 20      14
 *  STAT_LED_BLUE_PIN   STAT_LED_BLUE_PIN  10                  X      16      10
 *  STAT_LED_RED_PIN    STAT_LED_RED_PIN   12                  X      18      12
 *  TEMP_0_PIN          TEMP_0_PIN         41        X                41      39
 *  TEMP_BED_PIN        TEMP_BED_PIN       40        X                40      38
 *  X_DIR_PIN           X_DIR_PIN          1         X                1       29
 *  X_ENABLE_PIN        X_ENABLE_PIN       39        X                39      19
 *  X_STEP_PIN          X_STEP_PIN         0         X                0       28
 *  X_STOP_PIN          X_STOP_PIN         35        X                35      47
 *  Y_DIR_PIN           Y_DIR_PIN          3         X                3       31
 *  Y_ENABLE_PIN        Y_ENABLE_PIN       38        X                38      18
 *  Y_STEP_PIN          Y_STEP_PIN         2         X                2       30
 *  Y_STOP_PIN          Y_STOP_PIN         8         X                8       20
 *                      Y_STOP_PIN         37        X
 *  Z_DIR_PIN           Z_DIR_PIN          5         X                5       33
 *  Z_ENABLE_PIN        Z_ENABLE_PIN       23        X                23      17
 *  Z_STEP_PIN          Z_STEP_PIN         4         X                4       32
 *  Z_STOP_PIN          Z_STOP_PIN         36        X                36      36
 */
